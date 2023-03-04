/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include <dci/cmt/future.hpp>
#include <dci/cmt/promise.hpp>
#include <dci/sbs/signal.hpp>

namespace dci::utils
{
    template <class... Args>
    class S2f
        : private cmt::Promise<std::tuple<Args...>>
        , public cmt::Future<std::tuple<Args...>>
        , private sbs::Owner
    {
    public:
        S2f(sbs::Signal<void, Args...> s)
            : cmt::Promise<std::tuple<Args...>>{}
            , cmt::Future<std::tuple<Args...>>{cmt::Promise<std::tuple<Args...>>::future()}
        {
            s += this * [this](Args&&...args)
            {
                flush();
                this->resolveValue(std::forward_as_tuple(std::forward<decltype(args)>(args)...));
            };
        }

        ~S2f() noexcept
        {
            flush();
        }
    };
}
