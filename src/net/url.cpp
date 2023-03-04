/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include <dci/utils/net/url.hpp>
#include <dci/utils/net/ip.hpp>
#include <dci/utils/dbg.hpp>
#include <iostream>

namespace dci::utils::net::url
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool valid(const std::string& v)
    {
        using namespace std::literals;
        std::size_t pos = v.find("://"sv);
        return pos > 0 && pos < v.size();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    const std::string_view scheme(const std::string& v)
    {
        using namespace std::literals;

        std::size_t pos = v.find("://"sv);

        if(pos != v.npos)
        {
            return std::string_view{v.data(), pos};
        }

        return {};
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    const std::string_view authority(const std::string& v)
    {
        using namespace std::literals;

        std::size_t pos = v.find("://"sv);

        if(pos != v.npos)
        {
            return std::string_view{v.data()+pos+3, v.size()-pos-3};
        }

        return {};
    }

    namespace
    {
        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        bool isCover(const ip::Address4& baseIp4, std::string_view target)
        {
            ip::Port port;
            ip::Address4 targetIp4;
            if(ip::fromString(target, targetIp4, port))
            {
                return ip::isCover(ip::scope(baseIp4), ip::scope(targetIp4));
            }

            ip::Address6 targetIp6;
            ip::LinkId linkId;
            if(ip::fromString(target, targetIp6, linkId, port))
            {
                return ip::Scope::wan6 == ip::scope(targetIp6);
            }

            //парс не удался, предполагаю что там доменное имя
            return true;
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        bool isCover(const ip::Address6& baseIp6, ip::LinkId baseLinkId, std::string_view target)
        {
            ip::Port port;
            ip::Address6 targetIp6;
            ip::LinkId targetLinkId;
            if(ip::fromString(target, targetIp6, targetLinkId, port))
            {
                ip::Scope baseScope = ip::scope(baseIp6);
                ip::Scope targetScope =  ip::scope(targetIp6);

                if(ip::Scope::link6 == baseScope && ip::Scope::link6 == targetScope)
                {
                    return baseLinkId == targetLinkId;
                }

                return ip::isCover(baseScope, targetScope);
            }

            ip::Address4 targetIp4;
            if(ip::fromString(target, targetIp4, port))
            {
                return ip::Scope::wan4 == ip::scope(targetIp4);
            }

            //парс не удался, предполагаю что там доменное имя
            return true;
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        bool isWan(std::string_view target)
        {
            ip::Port port;
            ip::Address4 ip4;
            if(ip::fromString(target, ip4, port))
            {
                return ip::Scope::wan4 == ip::scope(ip4);
            }

            ip::Address6 ip6;
            ip::LinkId linkId;
            if(ip::fromString(target, ip6, linkId, port))
            {
                return ip::Scope::wan6 == ip::scope(ip6);
            }

            //парс не удался, предполагаю что там доменное имя
            return true;
        }

        /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
        bool isCoverImpl(const std::string& base, const std::string& target)
        {
            using namespace std::literals;

            std::string_view schemeBase = scheme(base);
            if(schemeBase.empty()) return false;

            std::string_view schemeTarget = scheme(target);
            if(schemeTarget.empty()) return false;

            if("inproc"sv == schemeBase)
            {
                return true;
            }

            if("local"sv == schemeBase)
            {
                if("inproc"sv == schemeTarget) return false;
                return true;
            }

            if(schemeBase.starts_with("tcp"sv))
            {
                if("inproc"sv == schemeTarget) return false;
                if("local"sv == schemeTarget) return false;

                std::string_view authorityBase = authority(base);
                ip::Port port;

                if("tcp"sv == schemeBase || "tcp4"sv == schemeBase)
                {
                    ip::Address4 ip4;
                    if(ip::fromString(authorityBase, ip4, port))
                    {
                        return isCover(ip4, authority(target));
                    }
                }

                if("tcp"sv == schemeBase || "tcp6"sv == schemeBase)
                {
                    ip::Address6 ip6;
                    ip::LinkId linkId;
                    if(ip::fromString(authorityBase, ip6, linkId, port))
                    {
                        return isCover(ip6, linkId, authority(target));
                    }
                }

                return isWan(authority(target));
            }

            return false;
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool isCover(const std::string& base, const std::string& target)
    {
        bool res = isCoverImpl(base, target);
        return res;
    }
}
