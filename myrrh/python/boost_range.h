/**
@file

Copyright John Reid 2008, 2013

Code to help iterate over python objects in C++.

*/

#ifndef MYRRH_PYTHON_BOOST_RANGE_H_
#define MYRRH_PYTHON_BOOST_RANGE_H_

#ifdef _MSC_VER
# pragma once
#endif //_MSC_VER

#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/iterator/indirect_iterator.hpp>
#include <boost/range.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/pointee.hpp>
#include <boost/python.hpp>




namespace myrrh {
namespace python {



/**
Iterates over a python sequence that supports __getitem__.
*/
class py_iterator
: public boost::iterator_facade<
	py_iterator, //Derived
	boost::python::object, //Value
	boost::random_access_traversal_tag, //CategoryOrTraversal
	boost::python::object //Reference
> {
	typedef boost::python::object object;
	typedef std::ptrdiff_t ptrdiff_t;

private:
	object _seq; //the sequence we are iterating over.
	ptrdiff_t _idx; //the index into that sequence.

public:
	py_iterator() { }

    explicit py_iterator( object seq, ptrdiff_t idx )
      : _seq( seq )
	  , _idx( idx )
    { }

private:
	friend class boost::iterator_core_access;

    void increment() { ++_idx; }
    void decrement() { --_idx; }
    void advance( ptrdiff_t n ) { _idx += n; }
    ptrdiff_t distance_to( py_iterator const& other ) const { return other._idx - this->_idx; }

    bool equal( py_iterator const& other ) const {
        return this->_seq.ptr() == other._seq.ptr() && this->_idx == other._idx;
    }

    object dereference() const { return _seq[_idx]; }
};


typedef py_iterator py_iterator; ///< For backwards-compatibility



/**
 * An iterator pointing to the beginning of the __getitem__ sequence.
 */
inline py_iterator py_seq_begin( boost::python::object seq ) {
	return py_iterator( seq, 0 );
}

/**
 * An iterator pointing to the end of the __getitem__ sequence.
 */
inline py_iterator py_seq_end( boost::python::object seq ) {
	return py_iterator( seq, boost::python::len( seq ) );
}


/**
 * A unary extract function.
 */
template< typename Target >
struct extract_fn {
	typedef typename boost::python::extract< Target >::result_type result_type;

	result_type operator()( PyObject* o ) const { return boost::python::extract< Target >( o )(); }
	result_type operator()( boost::python::object const& o ) const { return boost::python::extract< Target >( o )(); }
};




/**
 * A transforming iterator that uses boost::python::extract to extract values from its underlying iterator's values.
 */
template< typename Target, typename UnderlyingIt >
boost::transform_iterator<
	extract_fn< Target >,
	UnderlyingIt
>
make_extract_iterator( UnderlyingIt const & it )
{
	return boost::transform_iterator< extract_fn< Target >, UnderlyingIt >( it, extract_fn< Target >() );
}




/**
 * Makes a boost range that represents the python sequence and extracts values as it goes.
 */
template< typename Target >
boost::transformed_range< extract_fn< Target >, boost::python::object >
make_boost_range( boost::python::object seq )
{
	return boost::adaptors::transform( seq, extract_fn< Target >() );
}



} //namespace python
} //namespace myrrh






//
// Make boost.python objects function as boost.ranges
//
namespace boost {

//
// Specialize metafunctions. We must include the range.hpp header.
// We must open the boost namespace.
//

template<>
struct range_mutable_iterator< python::object >
{
	typedef myrrh::python::py_iterator type;
};

template<>
struct range_const_iterator< python::object >
{
	typedef myrrh::python::py_iterator type;
};





namespace python {

//
// The required functions for boost.range.
//

inline myrrh::python::py_iterator range_begin( object & x )
{
	return myrrh::python::py_seq_begin( x );
}

inline myrrh::python::py_iterator range_begin( const object & x )
{
	return myrrh::python::py_seq_begin( x );
}

inline myrrh::python::py_iterator range_end( object & x )
{
	return myrrh::python::py_seq_end( x );
}

inline myrrh::python::py_iterator range_end( const object & x )
{
	return myrrh::python::py_seq_end( x );
}

} // namespace python
} // namespace boost






#endif //MYRRH_PYTHON_BOOST_RANGE_H_
