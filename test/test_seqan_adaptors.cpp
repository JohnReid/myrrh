/**
@file

Copyright John Reid 2008, 2013

*/

#ifdef _MSC_VER
# pragma warning( disable : 4244 )
#endif //_MSC_VER

#include "myrrh/seqan_boost_range_adaptors.h"

#include <boost/progress.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_monitor.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/iterator/counting_iterator.hpp>

using namespace boost;
using namespace boost::unit_test;
using namespace boost::assign;
using namespace std;


BOOST_AUTO_TEST_CASE( check_string_set_iterator )
{
    cout << "*********************** check_string_set_iterator()\n";
    typedef seqan::String< seqan::Dna5 >       string_t;            /**< The string type. */
    typedef seqan::StringSet< string_t >       string_set_t;        /**< The string set type. */

    string_set_t string_set;
    seqan::appendValue( string_set, "ACGTGGACTGCGCTAGGCTCGCGC" );
    BOOST_CHECK_EQUAL( 1u, seqan::length( string_set ) );

    BOOST_FOREACH( string_t & s, string_set ) {
        cout << seqan::length( s ) << "\n";
    }

    BOOST_FOREACH( const string_t & s, const_cast< const string_set_t & >( string_set ) ) {
        cout << seqan::length( s ) << "\n";
    }
}
