#include <catch2/catch_test_macros.hpp>
#include "src/parser.cpp"

using namespace keyparser;

TEST_CASE( "dumbParse check", "[parser]" ) {
    Args args = {};

    SECTION( "1" ) {
        args.insert(args.end(), {"1", "2"});
        
        REQUIRE_NOTHROW( Parser::dumbParse(args) );
        CHECK( Parser::dumbParse(args).argnum() == 2 );

        SECTION( "1-1" ) {
            args.insert(args.end(), {"-a", "1", "2", "3"});

            REQUIRE_NOTHROW( Parser::dumbParse(args) );
            CHECK( Parser::dumbParse(args).argnum() == 2 );
            CHECK( Parser::dumbParse(args).childs.back().argnum() == 3 );

            SECTION( "1-1-1" ) {
                args.insert(args.end(), {"-b", "4"});

                REQUIRE_NOTHROW( Parser::dumbParse(args) );
                CHECK( Parser::dumbParse(args).argnum() == 2 );
                CHECK( Parser::dumbParse(args).childs.front().argnum() == 3 );
                CHECK( Parser::dumbParse(args).childs.back().argnum() == 1 );
            }

            SECTION( "1-1-2" ) {
                args.insert(args.end(), {"--b", "4"});

                REQUIRE_NOTHROW( Parser::dumbParse(args) );
                CHECK( Parser::dumbParse(args).argnum() == 2 );
                CHECK( Parser::dumbParse(args).childs.back().argnum() == 3 );
                CHECK( Parser::dumbParse(args).childs.back().childs.back().argnum() == 1 );
            }
        }
    }
}

TEST_CASE( "upgradeTask check", "[parser]" ) {
    Parser parser;
    Args args = {};

    SECTION( "1" ) {
        args.insert(args.end(), {"1", "2"});
        
        REQUIRE_NOTHROW( parser.parse(args) );
        CHECK( parser.parse(args).argnum() == 2 );

        SECTION( "1-1" ) {
            parser.addKey(Key('a'), uint(2));
            args.insert(args.end(), {"-a", "1", "2", "3"});

            REQUIRE_NOTHROW( parser.parse(args) );
            CHECK( parser.parse(args).argnum() == 3 );
            CHECK( parser.parse(args).childs.back().argnum() == 2 );

            SECTION( "1-1-1" ) {
                parser.addKey(Key('b'), uint(0));
                args.insert(args.end(), {"-b", "4"});

                REQUIRE_NOTHROW( parser.parse(args) );
                CHECK( parser.parse(args).argnum() == 4 );
                CHECK( parser.parse(args).childs.front().argnum() == 2 );
                CHECK( parser.parse(args).childs.back().argnum() == 0 );
            }
        }
    }
}
