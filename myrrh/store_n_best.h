/*
 * store_n_largest.h
 *
 *  Created on: 7 Sep 2011
 *      Author: john
 */

#ifndef MYRRH_JR_7SEP2011_STORE_N_LARGEST_H_
#define MYRRH_JR_7SEP2011_STORE_N_LARGEST_H_

#include <vector>
#include <functional>
#include <algorithm>

namespace myrrh {


/**
 * Store n best values.
 */
template<
    typename Storage,
    typename StrictWeakOrdering = std::greater< typename Storage::value_type >
>
struct store_n_best {

    size_t _n; ///< Maximum number to store.
    size_t _num_stored; ///< Number stored at moment.
    Storage & _storage; ///< heap of up to n largest values.
    StrictWeakOrdering _ordering; ///< Ordering for heap.

    /// Constructor.
    store_n_best(
        size_t n,
        Storage & storage,
        StrictWeakOrdering ordering = StrictWeakOrdering() )
    : _n( n )
    , _num_stored( 0 )
    , _storage( storage )
    , _ordering( ordering )
    {
        _storage.clear();
    }

    /// Add one element to the collection. Returns true iff was added.
    inline
    bool
    operator()( const typename Storage::value_type & value ) {
        if( _storage.empty() || _num_stored < _n ) {
            _storage.push_back( value );
            ++_num_stored;
            std::push_heap( _storage.begin(), _storage.end(), _ordering );
            return true;
        } else if( _ordering( value, get_worst() ) ) {
            std::pop_heap( _storage.begin(), _storage.end(), _ordering );
            *( _storage.rbegin() ) = value;
            std::push_heap( _storage.begin(), _storage.end(), _ordering );
            return true;
        } else {
            return false;
        }
    }

    /// Are we at capacity? I.e. do we have n elements already?
    inline
    bool
    at_capacity() const {
        return _num_stored == _n;
    }

    /// Get the worst value. (assumes at least one element in storage)
    inline
    const typename Storage::value_type &
    get_worst() const {
        return _storage[ 0 ];
    }
};


/**
 * Makes a store_n_best object with default std::greater ordering.
 */
template<
    typename Storage
>
store_n_best< Storage, std::greater< typename Storage::value_type > >
make_store_n_best(
    size_t n,
    Storage & storage
) {
    return store_n_best< Storage, std::greater< typename Storage::value_type > >( n, storage );
}



} // namespace myrrh

#endif /* MYRRH_JR_7SEP2011_STORE_N_LARGEST_H_ */
