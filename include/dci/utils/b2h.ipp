/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "b2h.hpp"
//#include <dci/utils/dbg.hpp>

namespace dci::utils
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class C, class... CC>
    std::string b2h(const std::vector<C, CC...>& b, HexEndian he)
    {
        return b2h(b.data(), b.size()*sizeof(C), he);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class C, std::size_t N>
    std::string b2h(const std::array<C, N>& b, HexEndian he)
    {
        return b2h(b.data(), b.size()*sizeof(C), he);
    }
}
