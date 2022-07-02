/* This file is part of the the dci project. Copyright (C) 2013-2022 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include <dci/utils/dbg.hpp>
#include <cstdio>
#include <cstdlib>
#include <errno.h>

#ifdef _WIN32
#   include <Windows.h>
#endif

namespace dci::utils::dbg
{

#ifdef _WIN32
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    namespace
    {
        const char* program_invocation_short_name = []
        {
            std::size_t bufSize = 256;
            char* buf = reinterpret_cast<char*>(malloc(bufSize));
            while(bufSize == GetModuleFileNameA(NULL, buf, bufSize))
            {
                bufSize += 256;
                free(buf);
                buf = reinterpret_cast<char*>(malloc(bufSize));
            }

            return buf;
        }();
    }
#endif

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void API_DCI_UTILS warn(const char* msg, const char* file, unsigned int line, const char* function)
    {
        fprintf(stderr, "%s%s%s:%u: %s%s`%s'\n",
                program_invocation_short_name,
                program_invocation_short_name ? ": " : "",
                file, line,
                function ? function : "", function ? ": " : "",
                msg);

        fflush(stderr);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    [[noreturn]] void API_DCI_UTILS fatal(const char* msg, const char* file, unsigned int line, const char* function)
    {
        warn(msg, file, line, function);
        std::abort();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void API_DCI_UTILS assertWarn(const char* assertion, const char* file, unsigned int line, const char* function)
    {
        fprintf(stderr, "%s%s%s:%u: %s%sAssertion `%s' failed.\n",
                program_invocation_short_name,
                program_invocation_short_name ? ": " : "",
                file, line,
                function ? function : "", function ? ": " : "",
                assertion);

        fflush(stderr);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    [[noreturn]] void API_DCI_UTILS assertFatal(const char* assertion, const char* file, unsigned int line, const char* function)
    {
        assertWarn(assertion, file, line, function);
        std::abort();
    }
}
