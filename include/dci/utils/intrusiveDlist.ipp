/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "intrusiveDlist.hpp"
#include <dci/utils/dbg.hpp>

namespace dci::utils
{
    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class Tag>
    IntrusiveDlistElement<T, Tag>::IntrusiveDlistElement(T* next, T* prev)
        : _next{next}
        , _prev{prev}
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class Tag>
    IntrusiveDlistElement<T, Tag>* intrusiveDlistElementCast(T* e) requires std::is_base_of_v<IntrusiveDlistElement<T, Tag>, T>
    {
        return static_cast<IntrusiveDlistElement<T, Tag> *>(e);
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class Tag>
    T* intrusiveDlistElementCast(IntrusiveDlistElement<T, Tag>* e, T* stubForADL) requires std::is_base_of_v<IntrusiveDlistElement<T, Tag>, T>
    {
        (void)stubForADL;
        return static_cast<T *>(e);
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class Tag, class RemoveCleaner>
    IntrusiveDlist<T, Tag, RemoveCleaner>::IntrusiveDlist()
        : _first{intrusiveDlistElementCast<T, Tag>(&_last), nullptr}
        , _last{nullptr, intrusiveDlistElementCast<T, Tag>(&_first)}
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class Tag, class RemoveCleaner>
    IntrusiveDlist<T, Tag, RemoveCleaner>::IntrusiveDlist(T* element)
        : _first{element, nullptr}
        , _last{nullptr, element}
    {
        auto idee = intrusiveDlistElementCast<T, Tag>(element);
        idee->_prev = &_first;
        idee->_next = &_last;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class Tag, class RemoveCleaner>
    IntrusiveDlist<T, Tag, RemoveCleaner>::IntrusiveDlist(RemoveCleaner&& removeCleaner)
        : _first{intrusiveDlistElementCast<T, Tag>(&_last), nullptr}
        , _last{nullptr, intrusiveDlistElementCast<T, Tag>(&_first)}
        , RemoveCleaner{std::forward<RemoveCleaner>(removeCleaner)}
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class Tag, class RemoveCleaner>
    IntrusiveDlist<T, Tag, RemoveCleaner>::IntrusiveDlist(T* element, RemoveCleaner&& removeCleaner)
        : _first{element, nullptr}
        , _last{nullptr, element}
        , RemoveCleaner{std::forward<RemoveCleaner>(removeCleaner)}
    {
        auto idee = intrusiveDlistElementCast<T, Tag>(element);
        idee->_prev = &_first;
        idee->_next = &_last;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class Tag, class RemoveCleaner>
    IntrusiveDlist<T, Tag, RemoveCleaner>::~IntrusiveDlist()
    {
        dbgAssert(!_first._prev && !_last._next);
        dbgAssert((intrusiveDlistElementCast<T, Tag>(_first._next) == &_last));
        dbgAssert((&_first == intrusiveDlistElementCast<T, Tag>(_last._prev)));
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class Tag, class RemoveCleaner>
    bool IntrusiveDlist<T, Tag, RemoveCleaner>::empty() const
    {
        return intrusiveDlistElementCast<T, Tag>(_first._next) == &_last;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class Tag, class RemoveCleaner>
    T* IntrusiveDlist<T, Tag, RemoveCleaner>::first() const
    {
        if(empty())
        {
            return nullptr;
        }

        return _first._next;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class Tag, class RemoveCleaner>
    T* IntrusiveDlist<T, Tag, RemoveCleaner>::last() const
    {
        if(empty())
        {
            return nullptr;
        }

        return _last._prev;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class Tag, class RemoveCleaner>
    std::pair<T*, T*> IntrusiveDlist<T, Tag, RemoveCleaner>::range() const
    {
        T* first = _first._next;
        T* end = intrusiveDlistElementCast<T, Tag>(const_cast<IntrusiveDlistElement<T, Tag> *>(&_last));
        return std::make_pair(
                    first,
                    end);
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class Tag, class RemoveCleaner>
    bool IntrusiveDlist<T, Tag, RemoveCleaner>::contains(T* element) const
    {
        IntrusiveDlistElement<T, Tag>* first = intrusiveDlistElementCast<T, Tag>(element);
        dbgAssert(&_first != element);

        //IntrusiveDlistElement<T, Tag>* last = element;

        while(first->_prev) first = intrusiveDlistElementCast<T, Tag>(first->_prev);
        //while(last->_next) last = intrusiveDlistElementCast<T, Tag>(last->_next);

        return &_first == first ;//&& &_last == last;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class Tag, class RemoveCleaner>
    void IntrusiveDlist<T, Tag, RemoveCleaner>::push(T* element)
    {
        auto idee = intrusiveDlistElementCast<T, Tag>(element);

        dbgAssert(!idee->_next);
        dbgAssert(!idee->_prev);

        idee->_next = intrusiveDlistElementCast<T, Tag>(&_last);
        idee->_prev = _last._prev;
        intrusiveDlistElementCast<T, Tag>(_last._prev)->_next = element;
        _last._prev = element;
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class Tag, class RemoveCleaner>
    void IntrusiveDlist<T, Tag, RemoveCleaner>::remove(T* element)
    {
        dbgAssert(contains(element));

        auto idee = intrusiveDlistElementCast<T, Tag>(element);

        intrusiveDlistElementCast<T, Tag>(idee->_prev)->_next = idee->_next;
        intrusiveDlistElementCast<T, Tag>(idee->_next)->_prev = idee->_prev;

        idee->_prev = nullptr;
        idee->_next = nullptr;
        RemoveCleaner::operator ()(element);
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class Tag, class RemoveCleaner>
    void IntrusiveDlist<T, Tag, RemoveCleaner>::clear()
    {
        flush([](T*){});
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class Tag, class RemoveCleaner>
    template <class F>
    void IntrusiveDlist<T, Tag, RemoveCleaner>::each(F&& f)
    {
        T* element = _first._next;
        T* end = intrusiveDlistElementCast<T, Tag>(&_last);
        while(element != end)
        {
            T* next = intrusiveDlistElementCast<T, Tag>(element)->_next;
            f(element);
            element = next;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class Tag, class RemoveCleaner>
    template <class F>
    void IntrusiveDlist<T, Tag, RemoveCleaner>::each(F&& f) const
    {
        T* element = _first._next;
        T* end = intrusiveDlistElementCast<T, Tag>(const_cast<IntrusiveDlistElement<T> *>(&_last));
        while(element != end)
        {
            T* next = intrusiveDlistElementCast<T, Tag>(element)->_next;
            f(const_cast<const T *>(element));
            element = next;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    template <class T, class Tag, class RemoveCleaner>
    template <class F>
    void IntrusiveDlist<T, Tag, RemoveCleaner>::flush(F&& f)
    {
        T* element = _first._next;
        T* end = intrusiveDlistElementCast<T, Tag>(&_last);
        _first._next = intrusiveDlistElementCast<T, Tag>(&_last);
        _last._prev = intrusiveDlistElementCast<T, Tag>(&_first);
        while(element != end)
        {
            T* next = intrusiveDlistElementCast<T, Tag>(element)->_next;
            intrusiveDlistElementCast<T, Tag>(element)->_next = nullptr;
            intrusiveDlistElementCast<T, Tag>(element)->_prev = nullptr;
            f(element);
            RemoveCleaner::operator ()(element);
            element = next;
        }
    }

}
