#pragma once

#include <my_lisp/package.hpp>
#include <my_lisp/contract_helpers.hpp>
#include <my_lisp/text_io.hpp>
#include <vector>
#include <memory>
#include <format>

class PackageCollection
{
public:
    PackageCollection() = default;

    FundamentalType::PackagePtr make_package(FundamentalType::StringView package_name)
    {
        PRECONDITION( !find_package(package_name), "A package with the same name already exists in the package collection" );

        return std::make_shared<Package>( FundamentalType::String(package_name) );
    }

    const FundamentalType::PackagePtr find_package(FundamentalType::StringView package_name) const
    {
        auto it = std::ranges::find(m_packages, package_name, &Package::name);

        if (it == m_packages.end())
            return nullptr;
        return *it;
    }

    FundamentalType::PackagePtr find_package(FundamentalType::StringView package_name)
    {
        auto it = std::ranges::find(m_packages, package_name, &Package::name);

        if (it == m_packages.end())
            return nullptr;
        return *it;
    }

    FundamentalType::PackagePtr &add_package(FundamentalType::PackagePtr package)
    {
        PRECONDITION( package, "Cannot add a null package to the collection" );
        PRECONDITION( !find_package(package->name()), "A package with the same name already exists in the package collection" );

        return m_packages.emplace_back( std::move(package) );
    }
    
    FundamentalType::PackagePtr remove_package(FundamentalType::StringView package_name)
    {
        // Can't remove a package with an empty name, so just return a nullptr without doing anything
        if ( package_name.empty() )
            return nullptr;

        auto rng = std::ranges::remove(m_packages, package_name, &Package::name);

        if ( rng.empty() )
            return nullptr; // No package with the given name was found, so just return a nullptr

        ASSERT(rng.size() == 1);

        FundamentalType::PackagePtr removed_package = *rng.begin(); // Store the removed package to return it later

        m_packages.erase( rng.begin(), rng.end() ); // Erase the "removed" elements from the vector
        return removed_package;
    }

    FundamentalType::PackagePtr remove_package(FundamentalType::PackagePtr package)
    {
        // Can't remove a nullptr, so just return one without doing anything
        if ( !package )
            return nullptr;

        auto rng = std::ranges::remove(m_packages, package);

        if ( rng.empty() )
            return nullptr; // No package with the given pointer was found, so just return a nullptr

        ASSERT(rng.size() == 1);

        // I think it is better to treat these as raw pointers and not smart ones here.
        // Let's return the actual package removed and not the one from this method's parameter.
        FundamentalType::PackagePtr removed_package = *rng.begin(); // Store the removed package to return it later

        m_packages.erase( rng.begin(), rng.end() ); // Erase the "removed" elements from the vector
        return removed_package;
    }
protected:
    std::vector<FundamentalType::PackagePtr> m_packages;
};
