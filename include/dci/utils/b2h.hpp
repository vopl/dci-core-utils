/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "api.hpp"
#include <cstring>
#include <string>
#include <cstdint>
#include <vector>
#include <array>
#include "hexEndian.hpp"

namespace dci::utils
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void API_DCI_UTILS b2h(const void* b, std::size_t bsize, void* h, HexEndian he = HexEndian::little);

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    std::string API_DCI_UTILS b2h(const void* b, std::size_t bsize, HexEndian he = HexEndian::little);

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class C = std::uint8_t, class... CC>
    std::string b2h(const std::vector<C, CC...>& b, HexEndian he = HexEndian::little);

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class C = std::uint8_t, std::size_t N>
    std::string b2h(const std::array<C, N>& b, HexEndian he = HexEndian::little);
}

#include "b2h.ipp"
