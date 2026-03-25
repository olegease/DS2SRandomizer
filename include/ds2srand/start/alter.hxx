#ifndef OLEGEASE_DS2SRAND_START_ALTER_HXX
#define OLEGEASE_DS2SRAND_START_ALTER_HXX

#include <array>
#include <string_view>

namespace ds2srand::start {
    struct Alter {
        using array_view = std::array< std::string_view, 8 >;
        inline static constexpr std::array< array_view, 8 > const replacements{
        // God - Good   | Neu - Neutral | Evi - Evil
        // Law - Lawful | Neu - Neutral | Cha - Chaotic
        // no depressed NeuNeu
        //               GodCha     NeuLaw    EviNeu       NeuCha    GodLaw    EviLaw      GodNeu      EviCha
        //override base "Warrior", "Knight", "Swordsman", "Bandit", "Cleric", "Sorcerer", "Explorer", "Deprived"
            array_view{ "King"   , "Hero"  , "Pyro"     , "Gyrm"  , "Monk"  , "Mage"    , "Bard"    , "Lich"     }, // group specific
            array_view{ "Warrior", "Knight", "Barbarian", "Hunter", "Herald", "Crusader", "Champion", "Deserter" }, // group tank->dual~=mage
            array_view{ "Samurai", "Fencer", "Swordsman", "Ranger", "Dancer", "Spectre" , "Wanderer", "Vagabond" }, // group dual->mage~=tank
            array_view{ "Acolyte", "Wizard", "Chieftain", "Arcane", "Priest", "Sorcerer", "Wraith"  , "Warlock"  }, // group mage->tank~=dual
            array_view{ "Duelist", "Slayer", "Gladiator", "Bandit", "Maiden", "Guardian", "Explorer", "Prisoner" }, // group tank~=dual->mage
            array_view{ "Invoker", "Warden", "Berserker", "Thief" , "Shaman", "Shadow"  , "Falconer", "Assassin" }, // group dual~=mage->tank
            array_view{ "Paladin", "Druid" , "Mercenary", "Rogue" , "Cleric", "Templar" , "Sentinel", "Demented" }, // group mage~=tank->dual
            array_view{ "Prophet", "Weaver", "Trickster", "Pirate", "Scion" , "Oracle"  , "Jester"  , "Deprived" }, // group tank~=dual~=mage
        // other variants: Amazon, Beast, Keeper, Marauder, Shade, Spirit, Soldier, Titan
        };
    };
} // namespace ds2srand::start

#endif//OLEGEASE_DS2SRAND_START_ALTER_HXX

// Ⓒ 2025-2026 Oleg'Ease'Kharchuk ᦒ
