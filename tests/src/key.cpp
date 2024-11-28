#include <catch2/catch_test_macros.hpp>
#include "src/key.cpp"

using namespace keyparser;

TEST_CASE( "Key operators '==' tests", "[key]" ) {
    CHECK( Key('a') == Key('a') );
    CHECK( Key("aa") == Key("aa") );
    CHECK( Key('a', "aa") == Key('a', "aa") );
}

TEST_CASE( "Key operators '!=' tests", "[key]" ) {
    CHECK( Key('a') != Key('b') );
    CHECK( Key('a') != Key("aa") );
    CHECK( Key("aa") != Key("aab") );

    CHECK( Key('a', "aa") != Key('a') );
    CHECK( Key('a', "aa") != Key("aa") );
    CHECK( Key('a', "aa") != Key('b', "aa") );
    CHECK( Key('a', "aa") != Key('a', "aab") );
}

TEST_CASE( "Key operators '^=' tests", "[key]" ) {
    bool result1 = Key('a') ^= Key('a');
    bool result2 = Key('a') ^= Key('b');
    bool result3 = Key('a', "aa") ^= Key('a');
    bool result4 = Key('b', "aa") ^= Key('a');
    bool result5 = Key('a', "aa") ^= Key('a', "aa");
    bool result6 = Key('b', "aa") ^= Key('a', "aa");
    bool result7 = Key('a', "ab") ^= Key('a', "aa");
    bool result8 = Key('a', "ab") ^= Key('b', "aa");

    CHECK( result1 );
    CHECK( result3 );
    CHECK( result5 );
    CHECK( result6 );
    CHECK( result7 );

    CHECK_FALSE( result2 );
    CHECK_FALSE( result4 );
    CHECK_FALSE( result8 );
}
