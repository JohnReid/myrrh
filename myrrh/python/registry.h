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
 * Queries if the the type specified is in the registry. This can be used to avoid registering
 * the same type more than once.
 */
template< typename QueryT >
const boost::python::converter::registration *
get_registration() {
	namespace bp = boost::python;
	const bp::type_info info = boost::python::type_id< QueryT >();
	const bp::converter::registration * registration = boost::python::converter::registry::query( info );
	// need to check for m_to_python converter: see http://stackoverflow.com/a/13017303/959926
	if( registration != 0 && registration->m_to_python != 0 ) {
		return registration;
	} else {
		return 0;
	}
}

} //namespace myrrh
} //namespace python

#endif //MYRRH_PYTHON_REGISTRY_H_
