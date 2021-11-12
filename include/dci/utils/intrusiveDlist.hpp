/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include <type_traits>
#include <utility>

namespace dci::utils
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T, class Tag = void>
    struct IntrusiveDlistElement
    {
        IntrusiveDlistElement(T* next=nullptr, T* prev=nullptr);

        T* _next{};
        T* _prev{};
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T, class Tag>
    IntrusiveDlistElement<T, Tag>* intrusiveDlistElementCast(T* e) requires std::is_base_of_v<IntrusiveDlistElement<T, Tag>, T>;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T, class Tag>
    T* intrusiveDlistElementCast(IntrusiveDlistElement<T, Tag>* e, T* stubForADL = nullptr) requires std::is_base_of_v<IntrusiveDlistElement<T, Tag>, T>;

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    struct NullRemoveCleaner
    {
        template <class... T>
        void operator()(const T&...) {}
    };

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class T, class Tag = void, class RemoveCleaner=NullRemoveCleaner>
    class IntrusiveDlist
        : private RemoveCleaner
    {
        IntrusiveDlist(const IntrusiveDlist&) = delete;
        void operator=(const IntrusiveDlist&) = delete;

    public:
        IntrusiveDlist();
        IntrusiveDlist(T* element);
        IntrusiveDlist(RemoveCleaner&& removeCleaner);
        IntrusiveDlist(T* element, RemoveCleaner&& removeCleaner);
        ~IntrusiveDlist();

    public:
        bool empty() const;
        T* first() const;
        T* last() const;
        std::pair<T*, T*> range() const;
        bool contains(T* element) const;
        void push(T* element);
        void remove(T* element);

        void clear();

        template <class F>
        void each(F&& f);

        template <class F>
        void each(F&& f) const;

        template <class F>
        void flush(F&& f);

    private:
        IntrusiveDlistElement<T, Tag> _first;//TODO: оставить один, оформить как цикл
        IntrusiveDlistElement<T, Tag> _last;
    };
}

#include "intrusiveDlist.ipp"
