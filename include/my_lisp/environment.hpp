#pragma once

#include <my_lisp/package.hpp>


class Environment
{
public:
    Environment()
        :
        m_packages( { Package( u8"KEYWORD" ) } )
    {
    }

    const Package *find_package(StringView package_name) const
    {
        auto it = std::ranges::find(m_packages, package_name, &Package::name);

        if (it == m_packages.end())
            return nullptr;
        return &*it;
    }

    Package *find_package(StringView package_name)
    {
        auto it = std::ranges::find(m_packages, package_name, &Package::name);

        if (it == m_packages.end())
            return nullptr;
        return &*it;
    }
protected:
    std::vector<Package> m_packages;
};
