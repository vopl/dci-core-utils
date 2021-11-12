/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once
#include <boost/preprocessor/repetition/repeat.hpp>
#include <tuple>
#include <type_traits>

namespace dci::utils
{
    class None {};

    template <class Sequence, class Projections = void>
    struct SwitchExpander
    {
    private:

        template<class T> struct ProjectionsGenerator;

        template<class Integral, Integral... values> struct ProjectionsGenerator<std::integer_sequence<Integral, values...>>
        {
            using result = std::tuple<std::integral_constant<Integral, values>...>;

        };

        using Src = typename ProjectionsGenerator<Sequence>::result;
        using Dst = std::conditional_t<
            std::is_void_v<Projections>,
            Src,
            Projections
        >;
        static_assert(std::tuple_size<Src>::value == Sequence::size(), "internal error");
        static_assert(std::tuple_size<Src>::value == std::tuple_size<Dst>::value, "projections must be same size as sequence");

        template <std::size_t index> using SrcElement = std::tuple_element_t<index, Src>;
        template <std::size_t index> using DstElement = std::tuple_element_t<index, Dst>;

    public:
        template <class F, class DF>
        static auto exec(typename Sequence::value_type value, F&& f, DF&& df)
        {
            return Impl<Sequence::size(), F, DF>::exec(value, std::forward<F>(f), std::forward<DF>(df));
        }

        template <class F>
        static auto exec(typename Sequence::value_type value, F&& f)
        {
            return exec(
                value,
                std::forward<F>(f),
                [](auto){return std::result_of_t<F(DstElement<0>*)>();});
        }

    private:
        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        template <std::size_t amount, class F, class DF> struct Impl;



#define _98302384578923645975873452345_entry(z, n, text)  case SrcElement<n>::value: return f(static_cast<DstElement<n>*>(nullptr));

#define _98302384578923645975873452345(n)                                                               \
        template <class F, class DF>                                                                    \
        struct Impl<n, F, DF>                                                                           \
        {                                                                                               \
            static std::result_of_t<F(DstElement<0>*)> exec(typename Sequence::value_type value, F&& f, DF&& df) \
            {                                                                                           \
                switch(value)                                                                           \
                {                                                                                       \
                    BOOST_PP_REPEAT(n, _98302384578923645975873452345_entry, )                          \
                }                                                                                       \
                return df(value);                                                                       \
            }                                                                                           \
        };                                                                                              \
                                                                                                     /**/

        _98302384578923645975873452345(1)
        _98302384578923645975873452345(2)
        _98302384578923645975873452345(3)
        _98302384578923645975873452345(4)
        _98302384578923645975873452345(5)
        _98302384578923645975873452345(6)
        _98302384578923645975873452345(7)
        _98302384578923645975873452345(8)
        _98302384578923645975873452345(9)
        _98302384578923645975873452345(10)
        _98302384578923645975873452345(11)
        _98302384578923645975873452345(12)
        _98302384578923645975873452345(13)
        _98302384578923645975873452345(14)
        _98302384578923645975873452345(15)
        _98302384578923645975873452345(16)
        _98302384578923645975873452345(17)
        _98302384578923645975873452345(18)
        _98302384578923645975873452345(19)
        _98302384578923645975873452345(20)

#undef _98302384578923645975873452345_entry
#undef _98302384578923645975873452345

    };


}
