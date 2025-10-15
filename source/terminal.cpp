#include <DS2SRandomizer/classes.hpp>
#include <modules/item_rando.hpp>
#include <modules/randomizer.hpp>

#include <cstdlib>
#include <iostream>
#include <set>
#include <string>

namespace app {
    using Commands = std::set< std::string_view >;
    inline Commands const commands{ "class", "enemy", "items" };

    struct Args {
        std::string_view command{ };
        std::set< std::string_view > options{ };
        Args( int argc, char* argv[] ) {
            if ( argc < 2 ) return;
            int options_from = 1;
            if ( argv[1][0] != '-' ) {
                if ( !commands.contains( argv[1] ) ) {
                    std::cerr << "Unknown command: " << argv[1] << std::endl;
                    throw std::runtime_error( "Unknown command" );
                }
                command = argv[1];
                options_from = 2;
            }
            for ( int i = options_from; i < argc; ++i ) options.emplace( argv[i] );
        }
    };
}

namespace app::classes {
    using namespace ds2srand::classes;
}

namespace app::enemy {
    using namespace randomizer;
}

namespace app::items {
    using namespace item_rando;
}

int main( int argc, char *argv[] ) try {
    std::cout << "ds2srand\n";

    app::Args args{ argc, argv };
    if ( args.options.contains( "-h" ) || args.options.contains( "--help" ) ) {
        std::cout << "Usage: ds2srand [command] [options]\n";
        std::cout << "Commands: (enemy and items by default\n";
        std::cout << "\tclass:           Scatter character class names and stats\n";
        std::cout << "\tenemy:           Enemies related options\n";
        std::cout << "\titems:           Items related options\n";
        std::cout << "Options:\n";
        std::cout << "\t-h, --help       Show this help message\n";
        std::cout << "\t-r, --restore    Restore default " << args.command << " parameters\n";
        std::cout << "\t--optimal200     [class command only] Set all original classes to optimal 200 soul level\n";
        std::cout << std::endl;
        return EXIT_SUCCESS;
    }

    auto check_command = [&args]( std::string_view command ) {
        return args.command.empty( ) || args.command == command;
    };

    if ( args.options.contains( "-r" ) || args.options.contains( "--restore" ) ) {
        if ( check_command( "class" ) ) {
            std::cout << "Restoring default parameters for character classes" << std::endl;
            app::classes::restore( );
        }

        if ( check_command( "enemy" ) ) {
            std::cout << "Restoring default parameters for enemies" << std::endl;
            app::enemy::restore_default_params(false);
        }

        if ( check_command( "items" ) ) {
            std::cout << "Restoring default parameters for items" << std::endl;
            app::items::restore_default_params();
        }

        return EXIT_SUCCESS;
    }

    if ( check_command( "class" ) ) {
        if ( args.options.contains( "--optimal200" ) ) {
            std::cout << "Setting all original classes to optimal 200 soul level" << std::endl;
            app::classes::optimal200( );
        } else {
            std::cout << "Scattering character class names and stats" << std::endl;
            auto names = app::classes::scatter( );
            std::cout << "Class names:";
            for ( auto const &name : names ) std::cout << " " << name;
            std::cout << std::endl;
        }
    }

    if ( check_command( "enemy" ) ) {
        app::enemy::Data dataEnemy;
        app::enemy::load_data( dataEnemy );
        app::enemy::randomize( dataEnemy, false );
        app::enemy::free_stuff( dataEnemy );
    }

    if ( check_command( "items" ) ) {
        app::items::IRData irdata;
        app::items::load_randomizer_data( irdata );
        app::items::randomize_items( irdata, false );
        app::items::free_rando_data( irdata );
    }

    return EXIT_SUCCESS;
} catch (std::exception const &e) {
    std::cerr << "std::exception: " << e.what( ) << std::endl;
    return EXIT_FAILURE;
} catch (...) {
    std::cerr << "...: Unknown exception" << std::endl;
    return EXIT_FAILURE;
} // main( ) try

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
