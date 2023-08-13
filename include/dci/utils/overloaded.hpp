#pragma once

namespace dci::utils
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
}
