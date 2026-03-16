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

    void create_standard_packages()
    {
        {
            PackagePtr standard_pkg = m_package_collection.make_package(u8"KEYWORD");

            m_package_collection.add_package(standard_pkg);
        }
        {
            PackagePtr cl_pkg = m_package_collection.make_package(u8"COMMON-LISP");

            m_package_collection.add_package(cl_pkg);
        }
        {
            PackagePtr clu_pkg = m_package_collection.make_package(u8"COMMON-LISP-USER");

            m_package_collection.add_package(clu_pkg);
        }
    }
};
