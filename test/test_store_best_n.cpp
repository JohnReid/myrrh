/**
 * Copyright John Reid 2011
 *
 * \file
 * \brief Code to test store_n_best algorithm.
 *
 */

#define BOOST_TEST_MODULE store n best test
#include <boost/test/unit_test.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/range/algorithm/find.hpp>
#include <boost/range/algorithm/count.hpp>

#include <myrrh/store_n_best.h>

#include <iostream>

using namespace myrrh;
using namespace std;
using boost::lambda::_1;


BOOST_AUTO_TEST_CASE( simple_store )
{
    typedef std::vector< unsigned > vec;

    const vec ns = boost::assign::list_of( 0 )( 4 )( 6 )( 8 )( 3 )( 5 );

    vec best;
    best.reserve( 3 );
    boost::for_each( ns, make_store_n_best( 3, best ) );
    //boost::for_each( best, cout << _1 << '\n' );

    BOOST_CHECK_EQUAL( boost::size( best ), 3 );
    BOOST_CHECK( boost::end( best ) != boost::find( best, 8 ) );
    BOOST_CHECK( boost::end( best ) != boost::find( best, 6 ) );
    BOOST_CHECK( boost::end( best ) != boost::find( best, 5 ) );
    BOOST_CHECK( boost::end( best ) == boost::find( best, 4 ) );
    BOOST_CHECK( boost::end( best ) == boost::find( best, 3 ) );
    BOOST_CHECK( boost::end( best ) == boost::find( best, 0 ) );
}


BOOST_AUTO_TEST_CASE( store_few )
{
    typedef std::vector< unsigned > vec;

    const vec ns = boost::assign::list_of( 0 )( 4 )( 6 );

    vec best;
    boost::for_each( ns, make_store_n_best( 20, best ) );
    //boost::for_each( best, cout << _1 << '\n' );

    BOOST_CHECK( boost::end( best ) != boost::find( best, 4 ) );
    BOOST_CHECK( boost::end( best ) != boost::find( best, 6 ) );
    BOOST_CHECK( boost::end( best ) != boost::find( best, 0 ) );
    BOOST_CHECK_EQUAL( boost::size( best ), 3 );
}


BOOST_AUTO_TEST_CASE( store_duplicates )
{
    typedef std::vector< unsigned > vec;

    const vec ns = boost::assign::list_of( 0 )( 4 )( 6 )( 0 )( 4 )( 6 );

    vec best;
    boost::for_each( ns, make_store_n_best( 3, best ) );
    boost::for_each( best, cout << _1 << '\n' );

    BOOST_CHECK_EQUAL( 1, boost::count( best, 4 ) );
    BOOST_CHECK_EQUAL( 2, boost::count( best, 6 ) );
    BOOST_CHECK_EQUAL( 0, boost::count( best, 0 ) );
    BOOST_CHECK_EQUAL( boost::size( best ), 3 );
}


