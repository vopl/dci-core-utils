/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#define likely(x) (x)
#define unlikely(x) (x)

//TODO: use std::unreachable instead
#define unreacheable() dci::utils::unreacheableImpl()

namespace dci::utils
{
    [[noreturn]] inline void unreacheableImpl()
    {
        // Uses compiler specific extensions if possible.
        // Even if no extension is used, undefined behavior is still raised by
        // an empty function body and the noreturn attribute.
#if defined(_MSC_VER) && !defined(__clang__) // MSVC
        __assume(false);
#else // GCC, Clang
        __builtin_unreachable();
#endif
    }
}

