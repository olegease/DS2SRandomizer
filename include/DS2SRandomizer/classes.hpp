#ifndef DS2SRANDOMIZER_CLASS_HPP
#define DS2SRANDOMIZER_CLASS_HPP

#include "classes/alter.hpp"
#include "classes/menutext.hpp"
#include "classes/stats.hpp"

#include <array>
#include <string>

namespace ds2srand::classes {

    class Original {
        struct Names {
            Stats const &stats;
            std::string_view name;
            uint8_t index{ };
            Names( Stats const &s, std::string_view n, uint8_t i ) : stats{ s }, name{ n }, index{ i } { }
        };
    public:
        inline static Stats Warrior  { Stats::Tank{ 7u, 6u,15u }, Stats::Dual{ 6u,11u, 5u }, Stats::Mage{ 5u, 5u, 5u } };
        inline static Stats Knight   { Stats::Tank{12u, 7u,11u }, Stats::Dual{ 6u, 8u, 9u }, Stats::Mage{ 4u, 3u, 6u } };
        inline static Stats Swordsman{ Stats::Tank{ 4u, 4u, 9u }, Stats::Dual{ 8u,16u, 6u }, Stats::Mage{ 6u, 7u, 5u } };
        inline static Stats Bandit   { Stats::Tank{ 9u,11u, 9u }, Stats::Dual{ 7u,14u, 3u }, Stats::Mage{ 2u, 1u, 8u } };
        inline static Stats Cleric   { Stats::Tank{10u, 8u,11u }, Stats::Dual{ 3u, 5u, 4u }, Stats::Mage{10u, 4u,12u } };
        inline static Stats Sorcerer { Stats::Tank{ 5u, 5u, 3u }, Stats::Dual{ 6u, 7u, 8u }, Stats::Mage{12u,14u, 4u } };
        inline static Stats Explorer { Stats::Tank{ 7u, 9u, 6u }, Stats::Dual{ 6u, 6u,12u }, Stats::Mage{ 7u, 5u, 5u } };
        inline static Stats Deprived { Stats::Tank{ 6u, 6u, 6u }, Stats::Dual{ 6u, 6u, 6u }, Stats::Mage{ 6u, 6u, 6u } };

        inline static std::array< Names, 8 > const array {
            Names{ Warrior,   MenuText::warrior.name,   0 },
            Names{ Knight,    MenuText::knight.name,    1 },
            Names{ Swordsman, MenuText::swordsman.name, 2 },
            Names{ Bandit,    MenuText::bandit.name,    3 },
            Names{ Cleric,    MenuText::cleric.name,    4 },
            Names{ Sorcerer,  MenuText::sorcerer.name,  5 },
            Names{ Explorer,  MenuText::explorer.name,  6 },
            Names{ Deprived,  MenuText::deprived.name,  7 }
        };
    };

    inline auto scatter( ) -> Alter::array_view {
        Alter::array_view names{ };
        MenuText menutext{ };
        for ( auto &orig_class : Original::array ) {
            std::random_device rd;
            std::mt19937_64 gen{ rd( ) };
            Stats randstats( 9u, 8u, gen );
            auto group = randstats.group( );
            std::string_view classname = Alter::replacements[(int)group][orig_class.index];
            menutext.override_bytes( orig_class.index, classname );
            names[orig_class.index] = classname;
        }
        return names;
    }

    inline void restore( ) {
        MenuText menutext{ };
        menutext.restore( );
    }
} // namespace ds2srand::classes

#endif//DS2SRANDOMIZER_CLASS_HPP

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
