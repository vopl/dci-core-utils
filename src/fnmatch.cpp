/* This file is part of the the dci project. Copyright (C) 2013-2022 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include <dci/utils/fnmatch.hpp>

/*
 * This file contains code originally derrived from OpenBSD fnmatch.c
 *
 * Copyright (c) 1989, 1993, 1994
 *      The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Guido van Rossum.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Function fnmatch() as specified in POSIX 1003.2-1992, section B.6.
 * Compares a filename or pathname to a pattern.
 */

#include <cctype>
#include <cstring>

namespace dci::utils
{
    namespace
    {
        const char EOS = '\0';

        const int RANGE_MATCH   = 1;
        const int RANGE_NOMATCH = 0;
        const int RANGE_ERROR   = -1;

        const int FNM_NOMATCH   = 1;
        //const int FNM_NOSYS     = 2;
        const int FNM_NORES     = 3;

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        int rangematch(const char* pattern, char test, int flags, const char **newp)
        {
            int negate, ok;
            char c, c2;

            /*
                    * A bracket expression starting with an unquoted circumflex
                    * character produces unspecified results (IEEE 1003.2-1992,
                    * 3.13.2). This implementation treats it like '!', for
                    * consistency with the regular expression syntax.
                    * J.T. Conklin (conklin@ngai.kaleida.com)
                    */
            if ((negate = (*pattern == '!' || *pattern == '^')) != 0)
                ++pattern;

            if (flags & fnmCaseFold)
                test = static_cast<char>(tolower(static_cast<unsigned char>(test)));

            /*
                    * A right bracket shall lose its special meaning and represent
                    * itself in a bracket expression if it occurs first in the list.
                    * -- POSIX.2 2.8.3.2
                    */
            ok = 0;
            c = *pattern++;
            do {
                if (c == '\\' && !(flags & fnmNoEscape))
                    c = *pattern++;
                if (c == EOS)
                    return (RANGE_ERROR);
                if ((c == '/' || c == '\\') && (flags & fnmPathName))
                    return (RANGE_NOMATCH);
                if ((flags & fnmCaseFold))
                    c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
                if (*pattern == '-'
                    && (c2 = *(pattern+1)) != EOS && c2 != ']') {
                    pattern += 2;
                    if (c2 == '\\' && !(flags & fnmNoEscape))
                        c2 = *pattern++;
                    if (c2 == EOS)
                        return (RANGE_ERROR);
                    if (flags & fnmCaseFold)
                        c2 = static_cast<char>(tolower(static_cast<unsigned char>(c2)));
                    if (c <= test && test <= c2)
                        ok = 1;
                } else if (c == test)
                    ok = 1;
            } while ((c = *pattern++) != ']');

            *newp = pattern;
            return (ok == negate ? RANGE_NOMATCH : RANGE_MATCH);
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        const char* findSlash(const char* str)
        {
            const char* a = strchr(str, '/');
            const char* b = strchr(str, '\\');
            if(!a)
                return b;
            if(!b)
                return a;
            return a < b ? a : b;
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        int fnmatchx(const char* pattern, const char* string, int flags, size_t recurs)
        {
            const char* stringstart;
            const char* newp;
            char c, test;
            int recurs_flags = flags & ~fnmPeriod;

            if (recurs-- == 0)
                return FNM_NORES;

            for (stringstart = string;;)
                switch (c = *pattern++) {
                case EOS:
                    if ((flags & fnmLeadingDir) && (*string == '/' || *string == '\\'))
                        return (0);
                    return (*string == EOS ? 0 : FNM_NOMATCH);
                case '?':
                    if (*string == EOS)
                        return (FNM_NOMATCH);
                    if ((*string == '/' || *string == '\\') && (flags & fnmPathName))
                        return (FNM_NOMATCH);
                    if (*string == '.' && (flags & fnmPeriod) &&
                        (string == stringstart ||
                         ((flags & fnmPathName) && (*(string - 1) == '/' || *(string - 1) == '\\'))))
                        return (FNM_NOMATCH);
                    ++string;
                    break;
                case '*':
                    c = *pattern;

                    /* Let '**' override PATHNAME match for this segment.
                         * It will be restored if/when we recurse below.
                         */
                    if (c == '*') {
                        c = *++pattern;
                        /* star-star-slash is at the end, match by default */
                        if (c == EOS)
                            return 0;
                        /* Double-star must be at end or between slashes */
                        if ((c != '/' && c != '\\'))
                            return (FNM_NOMATCH);

                        c = *++pattern;
                        do {
                            int e = fnmatchx(pattern, string, recurs_flags, recurs);
                            if (e != FNM_NOMATCH)
                                return e;
                            string = findSlash(string);
                        } while (string++);

                        /* If we get here, we didn't find a match */
                        return FNM_NOMATCH;
                    }

                    if (*string == '.' && (flags & fnmPeriod) &&
                        (string == stringstart ||
                         ((flags & fnmPathName) && (*(string - 1) == '/' || *(string - 1) == '\\'))))
                        return (FNM_NOMATCH);

                    /* Optimize for pattern with * at end or before /. */
                    if (c == EOS) {
                        if (flags & fnmPathName)
                            return ((flags & fnmLeadingDir) ||
                                    findSlash(string) == nullptr ?
                                        0 : FNM_NOMATCH);
                        else
                            return (0);
                    } else if ((c == '/' || c == '\\') && (flags & fnmPathName)) {
                        if ((string = findSlash(string)) == nullptr)
                            return (FNM_NOMATCH);
                        break;
                    }

                    /* General case, use recursion. */
                    while ((test = *string) != EOS) {
                        int e;

                        e = fnmatchx(pattern, string, recurs_flags, recurs);
                        if (e != FNM_NOMATCH)
                            return e;
                        if ((test == '/' || test == '\\') && (flags & fnmPathName))
                            break;
                        ++string;
                    }
                    return (FNM_NOMATCH);
                case '[':
                    if (*string == EOS)
                        return (FNM_NOMATCH);
                    if ((*string == '/' || *string == '\\') && (flags & fnmPathName))
                        return (FNM_NOMATCH);
                    if (*string == '.' && (flags & fnmPeriod) &&
                        (string == stringstart ||
                         ((flags & fnmPathName) && (*(string - 1) == '/' || *(string - 1) == '\\'))))
                        return (FNM_NOMATCH);

                    switch (rangematch(pattern, *string, flags, &newp)) {
                    case RANGE_ERROR:
                        /* not a good range, treat as normal text */
                        goto normal;
                    case RANGE_MATCH:
                        pattern = newp;
                        break;
                    case RANGE_NOMATCH:
                        return (FNM_NOMATCH);
                    }
                    ++string;
                    break;
                case '\\':
                    if (!(flags & fnmNoEscape)) {
                        if ((c = *pattern++) == EOS) {
                            c = '\\';
                            --pattern;
                        }
                    }
                    [[fallthrough]];
                default:
normal:
                    if (c != *string && !((flags & fnmCaseFold) &&
                                          (tolower(static_cast<unsigned char>(c)) ==
                                           tolower(static_cast<unsigned char>(*string)))))
                        return (FNM_NOMATCH);
                    ++string;
                    break;
                }
            /* NOTREACHED */
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool fnmatch(const char* pattern, const char* string, int flags)
    {
        return 0 == fnmatchx(pattern, string, flags, 64);
    }
}
