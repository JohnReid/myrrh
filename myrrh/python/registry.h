/**
@file

Copyright John Reid 2013

*/

#ifndef MYRRH_PYTHON_REGISTRY_H_
#define MYRRH_PYTHON_REGISTRY_H_

#ifdef _MSC_VER
# pragma once
#endif //_MSC_VER

#include <boost/type_traits/remove_const.hpp>

namespace myrrh {
namespace python {


/**
 * Queries if the the type specified is in the registry. This can be used to
 * avoid registering the same type more than once.
 */
template< typename QueryT >
const boost::python::converter::registration *
get_registration( bool check_empty_reg = true ) {
    namespace bp = boost::python;
    const bp::type_info info = boost::python::type_id< typename boost::remove_const< QueryT >::type >();
    const bp::converter::registration * registration = boost::python::converter::registry::query( info );
    // need to check for m_to_python converter: see http://stackoverflow.com/a/13017303/959926
    if( registration != 0 ) {
        if( check_empty_reg && registration->m_class_object == 0 ) {
            return 0;
        } else {
            return registration;
        }
    } else {
        return 0;
    }
}


/**
 * Uses CRTP pattern to ensure exposure of an exposed type if not already
 * exposed. Also adds the type as an attribute to another class/scope.
 */
template< typename Derived, bool check_empty_reg = true >
struct ensure_exposer
{
    const boost::python::converter::registration *
    get_exposed_registration() {
        return get_registration< typename Derived::exposed_type >( check_empty_reg );
    }

    /** Make sure the exposed type is exposed. */
    const boost::python::converter::registration *
    ensure_exposed() {
        const boost::python::converter::registration * registration =
            get_exposed_registration();
        if( ! registration ) {
            static_cast< Derived * >( this )->expose();
            registration = get_exposed_registration();
        }
        return registration;
    }


    /** Make sure the exposed type is exposed and add as a named attribute
     * to another class. */
    template< typename Class >
    const boost::python::converter::registration *
    ensure_exposed_and_add_as_attr( Class & _class, const char * name ) {
        return add_as_attr( _class, name, ensure_exposed() );
    }


    /** Add the exposed type as a named attribute to another class. */
    template< typename Class >
    const boost::python::converter::registration *
    add_as_attr(
        Class & _class,
        const char * name,
        const boost::python::converter::registration * registration = 0
    ) {
        namespace py = boost::python;
        if( ! registration ) {
            registration = get_exposed_registration();
        }
        if( registration ) {
            _class.attr( name ) = py::handle<>( registration->m_class_object );
        }
        return registration;
    }
};


} //namespace myrrh
} //namespace python


#endif //MYRRH_PYTHON_REGISTRY_H_
