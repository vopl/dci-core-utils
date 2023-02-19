/* This file is part of the the dci project. Copyright (C) 2013-2022 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include <utility>
#include <new>
#include <exception>
#include <concepts>

namespace dci::utils
{
    template<class Fn>
    class AtScopeExit
    {
        static_assert(!std::is_reference_v<Fn>);

        static constexpr bool noexceptMoveCtor = noexcept(new(nullptr) Fn{std::declval<Fn>()});
        static constexpr bool noexceptCopyCtor = noexcept(new(nullptr) Fn{std::declval<const Fn&>()});
        static constexpr bool noexceptCall = noexcept(std::declval<Fn&>()());
        static constexpr bool noexceptDtor = noexcept(std::declval<Fn&>().~Fn());

    public:
        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        // std::terminate if construction failed
        AtScopeExit(Fn&& fn) noexcept(noexceptMoveCtor) requires(std::constructible_from<Fn, Fn&&>)
            : m_active{}
        {
            tripleCall([&]() noexcept(noexceptMoveCtor) {
                new(m_fnSpace) Fn{std::move(fn)};
                m_active = true;
            },
            [&]() noexcept {},
            [&]() noexcept {
                std::terminate();
            });
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        // std::terminate if construction failed
        AtScopeExit(AtScopeExit&& other) noexcept(noexceptMoveCtor && noexceptDtor) requires(std::constructible_from<Fn, Fn&&>)
            : m_active{}
        {
            if(!other.m_active)
                return;

            tripleCall([&]() noexcept(noexceptMoveCtor)
            {
                new(m_fnSpace) Fn{*other.pfn()};
                m_active = true;
            },
            [&]() noexcept(noexceptDtor)
            {
                other.release();
            },
            [&]() noexcept
            {
                std::terminate();
            });
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        // std::terminate if construction failed
        AtScopeExit& operator=(AtScopeExit&& other) noexcept(noexceptMoveCtor && noexceptDtor) requires(std::constructible_from<Fn, Fn&&>)
        {
            if(this == &other)
                return *this;

            release();

            if(!other.m_active)
                return *this;

            tripleCall([&]() noexcept(noexceptMoveCtor)
            {
                new(m_fnSpace) Fn{*other.pfn()};
                m_active = true;
            },
            [&]() noexcept(noexceptDtor)
            {
                other.release();
            },
            [&]() noexcept
            {
                std::terminate();
            });

            return *this;
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        // functor called if construction failed
        AtScopeExit(const Fn& fn) noexcept(noexceptCopyCtor) requires(std::constructible_from<Fn, const Fn&>)
            : m_active{}
        {
            tripleCall([&]() noexcept(noexceptCopyCtor)
            {
                new(m_fnSpace) Fn{fn};
                m_active = true;
            },
            [&]() noexcept {},
            [&]() noexcept(noexceptCall)
            {
                fn();
            });
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        // functor called if construction failed
        AtScopeExit(const AtScopeExit& other) noexcept(noexceptCopyCtor) requires(std::constructible_from<Fn, const Fn&>)
            : m_active{}
        {
            if(!other.m_active)
                return;

            tripleCall([&]() noexcept(noexceptCopyCtor)
            {
                new(m_fnSpace) Fn{const_cast<const Fn&>(*other.pfn())};
                m_active = true;
            },
            [&]() noexcept {},
            [&]() noexcept(noexceptCall && noexceptDtor)
            {
                other.execute();
            });
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        AtScopeExit& operator=(const AtScopeExit& other) noexcept(noexceptCopyCtor) requires(std::constructible_from<Fn, const Fn&>)
        {
            if(this == &other)
                return *this;

            release();

            if(!other.m_active)
                return *this;

            tripleCall([&]() noexcept(noexceptCopyCtor)
            {
                new(m_fnSpace) Fn{const_cast<const Fn&>(*other.pfn())};
                m_active = true;
            },
            [&]() noexcept {},
            [&]() noexcept(noexceptCall && noexceptDtor)
            {
                other.execute();
            });

            return *this;
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void release() noexcept(noexceptDtor)
        {
            if(!m_active)
                return;

            m_active = false;
            pfn()->~Fn();
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        void execute() noexcept(noexceptCall && noexceptDtor)
        {
            if(!m_active)
                return;

            tripleCall([&]() noexcept(noexceptCall)
            {
                (*pfn())();
            },
            [&]() noexcept(noexceptDtor)
            {
                m_active = false;
                pfn()->~Fn();
            },
            [&]() noexcept(noexceptDtor)
            {
                m_active = false;
                pfn()->~Fn();
            });
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        ~AtScopeExit() noexcept(noexceptCall && noexceptDtor)
        {
            execute();
        }

    private:
        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        template <class F, class Success, class Fail>
        static void tripleCall(F f, Success success, Fail fail) noexcept(noexcept(f()) && noexcept(success()))
        {
            if constexpr(noexcept(f()))
            {
                f();
                success();
            }
            else
            {
                struct FailDoer
                {
                    Fail& fail;
                    bool active{true};
                    ~FailDoer() noexcept(noexcept(fail())) { if(active) fail(); }
                } failDoer{fail};

                f();
                failDoer.active = false;
                success();
            }
        }

    private:
        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        Fn* pfn() noexcept
        {
            return std::launder(reinterpret_cast<Fn*>(m_fnSpace));
        }

    private:
        alignas(Fn) char m_fnSpace[sizeof(Fn)];
        bool m_active;
    };
}
