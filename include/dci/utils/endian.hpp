/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include <type_traits>
#include <bit>

namespace dci::utils::endian
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::endian from, std::endian to, class T>
    T convert(T v) requires std::is_scalar_v<T>;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T> T l2b(T v) requires std::is_scalar_v<T>;
    template <class T> T b2l(T v) requires std::is_scalar_v<T>;
    template <class T> T n2b(T v) requires std::is_scalar_v<T>;
    template <class T> T b2n(T v) requires std::is_scalar_v<T>;
    template <class T> T l2n(T v) requires std::is_scalar_v<T>;
    template <class T> T n2l(T v) requires std::is_scalar_v<T>;
}

#include "endian.ipp"
