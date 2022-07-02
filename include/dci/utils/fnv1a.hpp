/* This file is part of the the dci project. Copyright (C) 2013-2022 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once
#include <cstdint>

namespace dci::utils
{
    template <class Char>
    constexpr std::uint64_t fnv1a(const Char* material, std::size_t len);

    template <class Container>
    constexpr std::uint64_t fnv1a(const Container& material);

    template <class LiteralChar, std::size_t N>
    constexpr std::uint64_t fnv1a(const LiteralChar (&material)[N]);
}

#include "fnv1a.ipp"
