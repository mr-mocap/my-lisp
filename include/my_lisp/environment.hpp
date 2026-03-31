#pragma once

#include <my_lisp/package.hpp>
#include <my_lisp/packagecollection.hpp>
#include <memory>


class Environment
{
public:
    Environment() = delete;

    // Global Environment Constructor
    Environment(std::weak_ptr<Package>  current,
                PackageCollection      &pc)
        :
        m_current_package( std::move(current) ),
        m_packagecollection( pc )
    {
    }

    Environment(Environment            *parent_env,
                std::weak_ptr<Package>  current,
                PackageCollection      &pc)
        :
        m_parent_environment( parent_env ),
        m_current_package( std::move(current) ),
        m_packagecollection( pc )
    {
    }

    Environment make_child_environment() const
    {
        return Environment(const_cast<Environment *>(this), m_current_package, m_packagecollection);
    }

    Environment *parent_environment() const noexcept { return m_parent_environment; }

    const PackageCollection &package_collection() const noexcept { return m_packagecollection; }
          PackageCollection &package_collection()       noexcept { return m_packagecollection; }

    bool is_global() const noexcept { return !m_parent_environment; }

    PackagePtr current_package() const noexcept
    {
        return m_current_package.lock();
    }

    PackagePtr current_package() noexcept
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

    OptionalSymbol find_symbol(StringView symbol_name) const
    {
        // If there is no current package, we can't look up the symbol_name
        if ( m_current_package.expired() )
            return std::nullopt;

        if (auto sym = current_package()->find_symbol(symbol_name) )
            return sym;
        
        for ( StringView iUsedPackageName : current_package()->uses_packages() )
        {
            const PackagePtr current_used_pkg = package_collection().find_package( iUsedPackageName );

            if ( !current_used_pkg )
                continue; // If the used package doesn't exist, just skip it and keep looking through the other used packages

            if (auto sym = current_used_pkg->find_symbol(symbol_name))
                if ( current_used_pkg->is_exported_name(symbol_name) )
                    return sym;
        }

        if ( !is_global() )
            return parent_environment()->find_symbol(symbol_name);
        
        return std::nullopt;
    }

    StringView symbol_name(Symbol symbol) const
    {
        if ( m_current_package.expired() )
            return StringView();

        if ( symbol.home_package.empty() )
            return StringView();

        const PackagePtr package_for_symbol = m_packagecollection.find_package( symbol.home_package );

        if ( !package_for_symbol )
            return StringView();

        return package_for_symbol->symbol_name( symbol );
    }
protected:
    Environment            *m_parent_environment = nullptr;
    std::weak_ptr<Package>  m_current_package;
    PackageCollection      &m_packagecollection;
};
