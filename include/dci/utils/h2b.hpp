/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "api.hpp"
#include <cstring>
#include <cstdint>
#include <vector>
#include <array>
#include <type_traits>
#include "hexEndian.hpp"

namespace dci::utils
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool API_DCI_UTILS h2b(const void* h, std::size_t hsize, void* b, HexEndian he = HexEndian::little);

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class C, class... CC>
    bool h2b(const void* h, std::size_t hsize, std::vector<C, CC...>& b, HexEndian he = HexEndian::little) requires std::is_standard_layout_v<C>;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class C, class... CC>
    bool h2b(const char* csz, std::vector<C, CC...>& b, HexEndian he = HexEndian::little) requires std::is_standard_layout_v<C>;
 \
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class C = std::uint8_t, class... CC>
    std::vector<C, CC...> h2b(const void* h, std::size_t hsize, HexEndian he = HexEndian::little) requires std::is_standard_layout_v<C>;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class C = std::uint8_t, class... CC>
    std::vector<C, CC...> h2b(const char* csz, HexEndian he = HexEndian::little) requires std::is_standard_layout_v<C>;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class C = std::uint8_t, std::size_t N>
    bool h2b(const char* csz, std::array<C, N>& buf, HexEndian he = HexEndian::little) requires std::is_standard_layout_v<C>;
}

#include "h2b.ipp"
