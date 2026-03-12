#pragma once

#include <my_lisp/package.hpp>
#include <memory>


class Environment
{
public:
    Environment() = default;

    Environment(Environment            *parent_env,
                std::weak_ptr<Package>  current)
        :
        m_parent_environment(parent_env),
        m_current_package( std::move(current) )
    {
    }

    Environment(std::weak_ptr<Package> current)
        :
        m_current_package( std::move(current) )
    {
    }

    Environment *parent_environment() const noexcept { return m_parent_environment; }

    std::shared_ptr<Package> current_package() const noexcept
    {
        return m_current_package.lock();
    }

#if 0
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
#endif
protected:
    Environment            *m_parent_environment = nullptr;
    std::weak_ptr<Package>  m_current_package;
};
