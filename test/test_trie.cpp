/**
@file

Copyright John Reid 2006, 2013

*/

#include "myrrh/trie.h"

#include <boost/progress.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_monitor.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/range.hpp>

using namespace myrrh::trie;
using namespace boost;
using namespace boost::unit_test;
using namespace boost::assign;
using namespace std;


typedef trie< char, unsigned > _trie;

struct test_output_fn
{
    unsigned total;

    test_output_fn() : total( 0 ) { }

    template< typename Prefix >
    void operator()( _trie::node_ptr node, const Prefix & prefix )
    {
#ifdef _DEBUG
        using namespace boost;
        cout << node->value << " ";
        BOOST_FOREACH( typename range_value< Prefix >::type c, prefix ) cout << c;
        cout << "\n";
#endif //_DEBUG

        total += node->value;
    }
};

BOOST_AUTO_TEST_CASE( check_trie )
{
    const std::vector< std::string > strings = list_of
        ( "acgt" )
        ( "gcta" )
        ( "aat" )
        ( "a" )
        ( "gat" )
        ;

    _trie t;
    BOOST_FOREACH( const std::string & s, strings )
    {
        _trie::node_ptr insert_node = t.insert( s.begin(), s.end() );
        _trie::node_ptr found_node = t.rsearch( s.begin(), s.end() );
        BOOST_CHECK( insert_node != 0 );
        BOOST_CHECK( insert_node->value == 0 );
        BOOST_CHECK( insert_node == found_node );
        ++( insert_node->value );
    }
    BOOST_FOREACH( const std::string & s, make_iterator_range( strings.rbegin(), strings.rend() ) )
    {
        _trie::node_ptr insert_node = t.insert( s.begin(), s.end(), 10 );
        _trie::node_ptr found_node = t.rsearch( s.begin(), s.end() );
        BOOST_CHECK( insert_node != 0 );
        BOOST_CHECK( insert_node->value == 1 );
        BOOST_CHECK( insert_node == found_node );
    }

    test_output_fn o_fn;
    BOOST_CHECK_EQUAL( strings.size(), t.traverse_prefixes( o_fn ).total );
}
