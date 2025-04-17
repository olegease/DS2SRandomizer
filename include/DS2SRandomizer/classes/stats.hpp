#ifndef DS2SRANDOMIZER_CLASSES_STATS_HPP
#define DS2SRANDOMIZER_CLASSES_STATS_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <cstdint>
#include <random>
#include <string>

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
    public:
        // 20 is mandatory for current implementation to generate equal groups
        inline static unsigned lequal_percent_value = Leaqual_Percent_Default;
        using Tank = Tank_;
        using Dual = Dual_;
        using Mage = Mage_;
        enum class Group {
            Specific = 0x00000, Balanced,
            TankDual, TankMage, DualMage,
            PureTank, PureDual, PureMage,
        };
        Stats(uint8_t init) :
            tank_{ init, init, init },
            dual_{ init, init, init },
            mage_{ init, init, init }
        { }
        Stats( Tank tank, Dual dual, Mage mage ) : tank_{ tank }, dual_{ dual }, mage_{ mage } { }
        Stats( uint8_t init, uint8_t diff, std::uniform_random_bit_generator auto &generator ) {
            assert( 6 < init && init < 51 && init > diff );

            std::uniform_int_distribution< unsigned > spec{ 0, 99 };
            if ( spec( generator ) <= 8 ) {
                specific_ = true;
                Stats{ init };
                return;
            }

            if (diff == 0) {
                Stats{ init };
                return;
            };
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
            int sum = 0;
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
        }

        auto groupsum_tank( ) const { return tank_.sum_; }
        auto groupsum_dual( ) const { return dual_.sum_; }
        auto groupsum_mage( ) const { return mage_.sum_; }

        auto vigor( )        const { return tank_.vig_; }
        auto endurance( )    const { return dual_.end_; }
        auto vitality( )     const { return tank_.vit_; }
        auto strength( )     const { return tank_.str_; }
        auto dexterity( )    const { return dual_.dex_; }
        auto adaptability( ) const { return dual_.adp_; }
        auto attunement( )   const { return mage_.att_; }
        auto intelligence( ) const { return mage_.int_; }
        auto faith( )        const { return mage_.fth_; }

        Group group( ) const {
            if ( specific_ ) return Group::Specific;
            if ( lequal_group_balanced( ) ) return Group::Balanced;
            if ( lequal_group_tankdual( ) ) return Group::TankDual;
            if ( lequal_group_tankmage( ) ) return Group::TankMage;
            if ( lequal_group_dualmage( ) ) return Group::DualMage;
            if ( tank_.sum_ > dual_.sum_ && tank_.sum_ > mage_.sum_ ) return Group::PureTank;
            if ( dual_.sum_ > tank_.sum_ && dual_.sum_ > mage_.sum_ ) return Group::PureDual;
            if ( mage_.sum_ > tank_.sum_ && mage_.sum_ > dual_.sum_ ) return Group::PureMage;
            return Group::Specific;
        }

        std::string group_text( ) const {
            static std::array< std::string_view, 8 > texts = {
                "Specific", "Balanced",
                "Tank/Dual", "Tank/Mage", "Dual/Mage",
                "Tank", "Dual", "Mage"
            };
            return std::string{ texts[static_cast< unsigned >( group( ) )] };
        }
    }; // class Stats
} // namespace ds2srand::classes

#endif//DS2SRANDOMIZER_CLASSES_STATS_HPP

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
