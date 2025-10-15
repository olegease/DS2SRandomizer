#ifndef OLEGEASE_DS2SRAND_START_HXX
#define OLEGEASE_DS2SRAND_START_HXX

#include <ds2srand/start/alter.hxx>
#include <ds2srand/start/menutext.hxx>
#include <ds2srand/start/stats.hxx>

#include <array>
#include <string>

namespace ds2srand::start {
    // TODO seperate interface and implementation, e.g move to source/classes.cpp
    inline auto scatter( ) -> Alter::array_view {
        Alter::array_view names{ };
        MenuText menutext{ };
        StatsData statsdata{ };
        std::random_device rd;
        std::mt19937_64 gen{ rd( ) };
        for ( auto &orig_class : Original::array ) {
            Stats randstats( 9u, 8u, gen );
            auto group = randstats.group( );
            std::string_view classname = Alter::replacements[(int)group][orig_class.index];
            menutext.override_bytes( orig_class.index, classname );
            statsdata.write( orig_class.index, randstats );
            names[orig_class.index] = classname;
        }
        return names;
    }
    // TODO split to private menutext and statsdata restore functions
    inline void restore( ) {
        MenuText menutext{ };
        StatsData statsdata{ };
        menutext.restore( );
        statsdata.restore( );
    }

    inline auto optimal200( ) -> void {
        restore( ); // TODO only need to restore menutext
        StatsData statsdata{ };
        for ( auto &orig_class : Original::array ) {
            Stats stats = Optimal200::array[ orig_class.index ].stats;
            statsdata.write( orig_class.index, stats );
        }
    }
} // namespace ds2srand::start

#endif//OLEGEASE_DS2SRAND_START_HXX

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
