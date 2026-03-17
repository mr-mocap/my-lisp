#pragma once

#include <my_lisp/packagecollection.hpp>
#include <my_lisp/environment.hpp>
#include <cassert>

class BasicLispSetup
{
public:

    Environment &environment()
    {
        assert( m_global_environment );

        return *m_global_environment;
    }

    void setup()
    {
        create_standard_packages();
        m_global_environment.emplace( m_package_collection.find_package(u8"COMMON-LISP-USER"),
                                      m_package_collection);
    }
protected:
    PackageCollection          m_package_collection;
    std::optional<Environment> m_global_environment;

    void create_standard_packages();
};
