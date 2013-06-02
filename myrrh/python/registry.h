/**
@file

Copyright John Reid 2013

*/

#ifndef MYRRH_PYTHON_REGISTRY_H_
#define MYRRH_PYTHON_REGISTRY_H_

#ifdef _MSC_VER
# pragma once
#endif //_MSC_VER

namespace myrrh {
namespace python {

/**
 * Queries if the the type specified is in the registry.
 */
template< typename QueryT >
bool
in_registry() {
	namespace bp = boost::python;
	bp::type_info info = boost::python::type_id< QueryT >();
	bp::converter::registration * registration = boost::python::converter::registry::query( info );
	return registration != 0;
}

} //namespace myrrh
} //namespace python

#endif //MYRRH_PYTHON_REGISTRY_H_
