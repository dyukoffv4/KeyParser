#include <catch2/catch_test_macros.hpp>
#include "src/key.cpp"

using namespace keyparser;

TEST_CASE( "Key operators '==' and '!=' tests", "[key]" ) {
    REQUIRE( Key('a') == Key('a') );
    REQUIRE( Key('a') != Key('b') );
    REQUIRE( Key('a') != Key("aa") );
    REQUIRE( Key("aa") == Key("aa") );
    REQUIRE( Key("aa") != Key("aab") );
    REQUIRE( Key('a', "aa") != Key('a') );
    REQUIRE( Key('a', "aa") != Key("aa") );
    REQUIRE( Key('a', "aa") == Key('a', "aa") );
    REQUIRE( Key('a', "aa") != Key('b', "aa") );
    REQUIRE( Key('a', "aa") != Key('a', "aab") );
}
