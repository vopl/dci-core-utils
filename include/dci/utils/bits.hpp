/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include <cstdint>
#include <type_traits>

namespace dci::utils::bits
{
    template <class Integral> constexpr std::size_t bitsof(Integral = Integral()) noexcept requires std::is_integral<Integral>::value;

    template <class Integral> constexpr std::size_t count0Least(Integral x) noexcept requires std::is_integral<Integral>::value;
    template <class Integral> constexpr std::size_t count0Most (Integral x) noexcept requires std::is_integral<Integral>::value;
    template <class Integral> constexpr std::size_t count1Least(Integral x) noexcept requires std::is_integral<Integral>::value;
    template <class Integral> constexpr std::size_t count1Most (Integral x) noexcept requires std::is_integral<Integral>::value;
}

#include "bits.ipp"
