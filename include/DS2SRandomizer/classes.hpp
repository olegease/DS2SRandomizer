#ifndef DS2SRANDOMIZER_CLASS_HPP
#define DS2SRANDOMIZER_CLASS_HPP

#include <array>
#include <cstdint>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>

namespace DS2SRandomizer::classes {
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
        OverrideBytes const warrior  { 0x11CE, "Warrior" };
        OverrideBytes const knight   { 0x11DE, "Knight" };
        OverrideBytes const bandit   { 0x11EC, "Bandit" };
        OverrideBytes const cleric   { 0x11FA, "Cleric" };
        OverrideBytes const sorcerer { 0x1208, "Sorcerer" };
        OverrideBytes const explorer { 0x121A, "Explorer" };
        OverrideBytes const swordsman{ 0x122C, "Swordsman" };
        OverrideBytes const deprived { 0x1240, "Deprived" };
        std::array< OverrideBytes, 8 > const array {
            warrior, knight, bandit, cleric,
            sorcerer, explorer, swordsman, deprived
        };

        void override_bytes( OverrideBytes const &bytesclass, std::string_view name ) {
            for (unsigned i = 0u; i < bytesclass.name.size(); ++i) {
                file.seekp( bytesclass.offset + i * 2 );
                char c = i < name.size() ? name[i] : '\0';
                file.put( c );
            }
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
    };
}

#endif//DS2SRANDOMIZER_CLASS_HPP
