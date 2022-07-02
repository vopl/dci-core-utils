/* This file is part of the the dci project. Copyright (C) 2013-2022 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include <utility>
#include <type_traits>
#include <concepts>
#include <optional>


namespace dci::utils
{
    template <class F>
    class AtScopeExit
    {
        using Holder = std::optional<std::decay_t<F>>;
        Holder _holder;

    public:
        AtScopeExit()
            : _holder{}
        {
        }

        AtScopeExit(const F& f) requires(std::constructible_from<Holder, const F&>)
            : _holder{f}
        {
        }

        AtScopeExit(F&& f) requires(std::constructible_from<Holder, F&&>)
            : _holder{std::forward<F>(f)}
        {
        }

        AtScopeExit(const AtScopeExit& from) requires(std::constructible_from<Holder, const Holder&>)
            : _holder{from._holder}
        {
        }

        AtScopeExit(AtScopeExit&& from) requires(std::constructible_from<Holder, Holder&&>)
            : _holder{std::move(from._holder)}
        {
            from._holder.reset();
        }

        AtScopeExit& operator=(const AtScopeExit& from) requires(std::assignable_from<Holder, const Holder&>)
        {
            _holder = from._holder;
            return *this;
        }

        AtScopeExit& operator=(AtScopeExit&& from) requires(std::assignable_from<Holder, Holder&&>)
        {
            _holder = std::move(from._holder);
            from._holder.reset();
            return *this;
        }

        ~AtScopeExit()
        {
            if(_holder)
            {
                _holder.value()();
            }
        }
    };
}
