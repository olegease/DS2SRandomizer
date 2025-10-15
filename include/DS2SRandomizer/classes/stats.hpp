#ifndef DS2SRANDOMIZER_CLASSES_STATS_HPP
#define DS2SRANDOMIZER_CLASSES_STATS_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <cstdint>
#include <fstream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

namespace ds2srand::classes {
    //
    class Stats {
        inline static constexpr unsigned Leaqual_Percent_Default = 20u;
        struct Tank_ {
            uint8_t vig_{ }, vit_{ }, str_{ };
            unsigned sum_{ };
            Tank_( ) = default;
            Tank_( uint8_t vig, uint8_t vit, uint8_t str ) :
                vig_{ vig }, vit_{ vit }, str_{ str },
                sum_{ static_cast< unsigned >( vig + vit + str ) }
            { }
        } tank_;
        struct Dual_ {
            uint8_t end_{ }, dex_{ }, adp_{ };
            unsigned sum_{ };
            Dual_( ) = default;
            Dual_( uint8_t end, uint8_t dex, uint8_t adp ) :
                end_{ end }, dex_{ dex }, adp_{ adp },
                sum_{ static_cast< unsigned >( end + dex + adp ) }
            { }
        } dual_;
        struct Mage_ {
            uint8_t att_{ }, int_{ }, fth_{ };
            unsigned sum_{ };
            Mage_( ) = default;
            Mage_( uint8_t att, uint8_t intv, uint8_t fth ) :
                att_{ att }, int_{ intv }, fth_{ fth },
                sum_{ static_cast< unsigned >( att + intv + fth ) }
            { }
        } mage_;
        bool specific_{ false };
        enum class Group_ : unsigned{
            Specific = 0x000u, Balanced,
            TankDual, TankMage, DualMage,
            PureTank, PureDual, PureMage,
        } group_;
        bool lequal_percent( unsigned lhs, unsigned rhs, unsigned percent = Leaqual_Percent_Default ) const {
            if (lhs == rhs) return true;
            unsigned diff = lhs > rhs ? lhs - rhs : rhs - lhs;
            unsigned max = lhs > rhs ? lhs : rhs;
            return (diff * 100 / max) <= percent;
        }
        bool lequal_group_balanced( ) const {
            return lequal_percent( tank_.sum_, dual_.sum_ )
                && lequal_percent( tank_.sum_, mage_.sum_ )
                && lequal_percent( dual_.sum_, mage_.sum_ );
        }
        bool lequal_group_tankdual( ) const {
            return lequal_percent( tank_.sum_, dual_.sum_ ) && std::min( tank_.sum_, dual_.sum_ ) > mage_.sum_;
        }
        bool lequal_group_tankmage( ) const {
            return lequal_percent( tank_.sum_, mage_.sum_ ) && std::min( tank_.sum_, mage_.sum_ ) > dual_.sum_;
        }
        bool lequal_group_dualmage( ) const {
            return lequal_percent( dual_.sum_, mage_.sum_ ) && std::min( dual_.sum_, mage_.sum_ ) > tank_.sum_;
        }
        Group_ group_impl( ) const {
            if ( specific_ ) return Group_::Specific;
            if ( lequal_group_balanced( ) ) return Group_::Balanced;
            if ( lequal_group_tankdual( ) ) return Group_::TankDual;
            if ( lequal_group_tankmage( ) ) return Group_::TankMage;
            if ( lequal_group_dualmage( ) ) return Group_::DualMage;
            if ( tank_.sum_ > dual_.sum_ && tank_.sum_ > mage_.sum_ ) return Group_::PureTank;
            if ( dual_.sum_ > tank_.sum_ && dual_.sum_ > mage_.sum_ ) return Group_::PureDual;
            if ( mage_.sum_ > tank_.sum_ && mage_.sum_ > dual_.sum_ ) return Group_::PureMage;
            return Group_::Specific;
        }
    public:
        // 20 is mandatory for current implementation to generate equal groups
        inline static unsigned lequal_percent_value = Leaqual_Percent_Default;
        using Tank = Tank_;
        using Dual = Dual_;
        using Mage = Mage_;
        using Group = Group_;

        Stats(uint8_t init) :
            tank_{ init, init, init },
            dual_{ init, init, init },
            mage_{ init, init, init },
            group_{ Group::Balanced }
        { }
        Stats( Tank tank, Dual dual, Mage mage ) :
            tank_{ tank },
            dual_{ dual },
            mage_{ mage },
            group_{ group_impl( ) }
        { }
        Stats( uint8_t init, uint8_t diff, std::uniform_random_bit_generator auto &generator ) {
            assert( 6 < init && init < 51 && init > diff && diff != 0 );

            std::uniform_int_distribution< unsigned > spec{ 0, 99 };
            specific_ = spec( generator ) <= 8;

            std::array< uint8_t, 9 > stats;
            stats.fill( init );
            auto gen_sign = [&generator]( ) -> int8_t {
                std::uniform_int_distribution< unsigned > dist_sign( 0, 1 );
                return dist_sign( generator ) ? 1 : -1;
            };
            auto gen_diff = [&generator, &diff]( ) -> uint8_t {
                std::uniform_int_distribution< unsigned > dist_diff( 0, diff );
                return static_cast< uint8_t >( dist_diff( generator ) );
            };
            int curr = 0;
            unsigned sum = 0u;
            int8_t sign = gen_sign( );
            for ( auto &stat : stats ) {
                int8_t gend = gen_diff( ) * sign;
                stat += gend;
                curr += gend;
                sum += stat;
                if ( curr > 0 ) sign = -1;
                else if ( curr < 0 ) sign = 1;
                else sign = gen_sign( );
            }
            #ifndef NDEBUG
            auto init_sum = init * stats.size( );
            #endif
            assert( sum - diff <= init_sum && init_sum <= sum + diff );
            std::shuffle( stats.begin( ), stats.end( ), generator );

            tank_ = Tank_{ stats[0], stats[2], stats[4] };
            dual_ = Dual_{ stats[1], stats[5], stats[6] };
            mage_ = Mage_{ stats[3], stats[7], stats[8] };
            group_ = group_impl( );
        }

        auto sum( ) const { return static_cast< uint16_t >( tank_.sum_ + dual_.sum_ + mage_.sum_ ); }
        auto soul_level( ) const { return static_cast< uint16_t >( sum( ) - 53u ); }
        auto groupsum_tank( ) const { return tank_.sum_; }
        auto groupsum_dual( ) const { return dual_.sum_; }
        auto groupsum_mage( ) const { return mage_.sum_; }

        auto vigor( )        const { return tank_.vig_; }
        auto endurance( )    const { return dual_.end_; }
        auto vitality( )     const { return tank_.vit_; }
        auto attunement( )   const { return mage_.att_; }
        auto strength( )     const { return tank_.str_; }
        auto dexterity( )    const { return dual_.dex_; }
        auto adaptability( ) const { return dual_.adp_; }
        auto intelligence( ) const { return mage_.int_; }
        auto faith( )        const { return mage_.fth_; }

        Group group( ) const { return group_; }

        std::string group_text( ) const {
            static std::array< std::string_view, 8 > texts = {
                "Specific", "Balanced",
                "Tank/Dual", "Tank/Mage", "Dual/Mage",
                "Tank", "Dual", "Mage"
            };
            return std::string{ texts[static_cast< unsigned >( group( ) )] };
        }
    }; // class Stats

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

    class Optimal200 {
        struct ArrayData {
            Stats const &stats;
            uint8_t index{ };
            ArrayData( Stats const &s, uint8_t i ) : stats{ s }, index{ i } { }
        };
    public:
        inline static Stats Warrior  { Stats::Tank{ 8u,64u,50u }, Stats::Dual{50u,12u, 9u }, Stats::Mage{50u, 5u, 5u } };
        inline static Stats Knight   { Stats::Tank{50u,48u,64u }, Stats::Dual{50u, 8u,20u }, Stats::Mage{ 4u, 3u, 6u } };
        inline static Stats Swordsman{ Stats::Tank{36u, 4u,16u }, Stats::Dual{40u,40u,64u }, Stats::Mage{40u, 8u, 5u } };
        inline static Stats Bandit   { Stats::Tank{64u,50u,25u }, Stats::Dual{ 7u,80u,16u }, Stats::Mage{ 2u, 1u, 8u } };
        inline static Stats Cleric   { Stats::Tank{48u, 8u,50u }, Stats::Dual{64u, 5u, 4u }, Stats::Mage{20u,18u,36u } };
        inline static Stats Sorcerer { Stats::Tank{20u, 5u, 3u }, Stats::Dual{10u, 7u, 8u }, Stats::Mage{94u,64u,42u } };
        inline static Stats Explorer { Stats::Tank{20u,15u, 6u }, Stats::Dual{ 8u,64u,80u }, Stats::Mage{50u, 5u, 5u } };
        inline static Stats Deprived { Stats::Tank{50u, 6u, 8u }, Stats::Dual{30u, 6u,33u }, Stats::Mage{50u, 6u,64u } };

        inline static std::array< ArrayData, 8 > const array {
            ArrayData{ Warrior,   0 },
            ArrayData{ Knight,    1 },
            ArrayData{ Swordsman, 2 },
            ArrayData{ Bandit,    3 },
            ArrayData{ Cleric,    4 },
            ArrayData{ Sorcerer,  5 },
            ArrayData{ Explorer,  6 },
            ArrayData{ Deprived,  7 }
        };
    };

    struct StatsData {
        std::fstream file;
        StatsData( ) : file{ "Param/PlayerStatusParam.param", std::ios::binary | std::ios::in | std::ios::out } {
            if ( !file.is_open( ) ) throw std::runtime_error( "Error opening PlayerStatusParam.param file" );
        }
        inline static constexpr std::array< uint8_t, 9 > const offsets{ 0x02u, 0x08u, 0x0Au, 0x0Cu, 0x0Eu, 0x10u, 0x12u, 0x14u, 0x16u };
        // order of stats as they appear in the param file
        struct StatsAdapter
        {
            std::array< uint8_t, offsets.size( ) > array{ };
            StatsAdapter( Stats const &stats) {
                array[0] = stats.vigor( );
                array[1] = stats.endurance( );
                array[2] = stats.attunement( );
                array[3] = stats.vitality( );
                array[4] = stats.strength( );
                array[5] = stats.dexterity( );
                array[6] = stats.intelligence( );
                array[7] = stats.faith( );
                array[8] = stats.adaptability( );
            }
        };
        struct OverrideBytes {
            unsigned offset;
            Stats const *stats;
        };

        inline static OverrideBytes const warrior  { 0x031Cu, &Original::Warrior };
        inline static OverrideBytes const knight   { 0x0444u, &Original::Knight };
        inline static OverrideBytes const bandit   { 0x056Cu, &Original::Bandit };
        inline static OverrideBytes const cleric   { 0x0694u, &Original::Cleric };
        inline static OverrideBytes const sorcerer { 0x07BCu, &Original::Sorcerer };
        inline static OverrideBytes const explorer { 0x08E4u, &Original::Explorer };
        inline static OverrideBytes const swordsman{ 0x0A0Cu, &Original::Swordsman };
        inline static OverrideBytes const deprived { 0x0B34u, &Original::Deprived };
        inline static std::array< OverrideBytes, 8 > const array {
            warrior, knight, swordsman, bandit, cleric, sorcerer, explorer, deprived
        };

        auto read( OverrideBytes const &bytesclass ) -> Stats {
            file.seekg( bytesclass.offset );
            std::vector< uint8_t > values{ };
            values.reserve( offsets.size( ) );
            for (auto offset : offsets) {
                file.seekg( bytesclass.offset + offset );
                auto b = static_cast< uint8_t >( file.get( ) );
                values.push_back( b );
            }
            return Stats{
                { Stats::Tank{ values[0], values[3], values[4] } },
                { Stats::Dual{ values[1], values[5], values[8] } },
                { Stats::Mage{ values[2], values[6], values[7] } }
            };
        }

        auto read( std::uint8_t index ) -> Stats { return read( array.at( index ) ); }

        void write( OverrideBytes const &bytesclass, Stats const &stats ) {
            StatsAdapter adapter{ stats };
            for (auto i = 0u; i < offsets.size( ); ++i ) {
                file.seekp( bytesclass.offset + offsets[i] );
                file.put( adapter.array[i] );
            }
            uint16_t sum = stats.soul_level( );
            file.seekp( bytesclass.offset );
            file.write( reinterpret_cast< char const * >( &sum ), sizeof( sum ) );
        }

        void write( std::uint8_t index, Stats const &stats ) { write( array.at( index ), stats ); }

        void restore( ) {
            for ( auto &bytes : array ) write( bytes, *bytes.stats );
        }
    };

} // namespace ds2srand::classes

#endif//DS2SRANDOMIZER_CLASSES_STATS_HPP

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
