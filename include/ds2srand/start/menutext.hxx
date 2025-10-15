#ifndef OLEGEASE_DS2SRAND_START_MENUTEXT_HXX
#define OLEGEASE_DS2SRAND_START_MENUTEXT_HXX

#include <array>
#include <cinttypes>
#include <fstream>

namespace ds2srand::start {
    // This class is used to override the names of the classes
    struct MenuText {
        std::fstream file;
        MenuText( ) : file{ "menu/text/english/common.fmg", std::ios::binary | std::ios::in | std::ios::out } {
            if ( !file.is_open( ) ) {
                throw std::runtime_error( "Error opening common.fmg file" );
            }
        }
        struct OverrideBytes {
            unsigned offset;
            std::string_view name;
        };
        inline static OverrideBytes const warrior  { 0x11CC, "Warrior" };
        inline static OverrideBytes const knight   { 0x11DC, "Knight" };
        inline static OverrideBytes const bandit   { 0x11EA, "Bandit" };
        inline static OverrideBytes const cleric   { 0x11F8, "Cleric" };
        inline static OverrideBytes const sorcerer { 0x1206, "Sorcerer" };
        inline static OverrideBytes const explorer { 0x1218, "Explorer" };
        inline static OverrideBytes const swordsman{ 0x122A, "Swordsman" };
        inline static OverrideBytes const deprived { 0x123E, "Deprived" };

        inline static std::array< OverrideBytes, 8 > const array {
            warrior, knight, swordsman, bandit,
            cleric, sorcerer, explorer, deprived
        };

        void override_bytes( OverrideBytes const &bytesclass, std::string_view name ) {
            for (unsigned i = 0u; i < bytesclass.name.size(); ++i) {
                file.seekp( bytesclass.offset + i * 2 );
                char c = i < name.size() ? name[i] : '\0';
                file.put( c );
            }
        }

        void override_bytes( uint8_t index, std::string_view name ) {
            if ( index >= array.size( ) ) throw std::out_of_range( "Index out of range" );
            override_bytes( array[ index ], name);
        }

        std::string read_bytes( OverrideBytes const &bytesclass ) {
            std::string name{ };
            name.reserve( bytesclass.name.size( ) );
            for ( unsigned i = 0u; i < bytesclass.name.size( ); ++i ) {
                file.seekg( bytesclass.offset + i * 2 );
                auto c = static_cast< char >( file.get( ) );
                if (c == '\0') break;
                name += c;
            }
            return name;
        }

        void restore( ) { for ( auto const &c : array ) override_bytes( c, c.name ); }
    }; // struct MenuText

} // namespace ds2srand::start

#endif//OLEGEASE_DS2SRAND_START_MENUTEXT_HXX

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
