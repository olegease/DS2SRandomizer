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
            array_view{ "Titan"  , "Hero"  , "Shade"    , "Rogue" , "Monk"  , "Mage"    , "Bard"    , "Lich"     }, // group specific
            array_view{ "Prophet", "Weaver", "Trickster", "Pirate", "Scion" , "Oracle"  , "Jester"  , "Deprived" }, // group tank~=dual~=mage
            array_view{ "Duelist", "Slayer", "Gladiator", "Bandit", "Keeper", "Guardian", "Explorer", "Prisoner" }, // group tank~=dual->mage
            array_view{ "Paladin", "Druid" , "Mercenary", "Ninja" , "Cleric", "Templar" , "Sentinel", "Demented" }, // group tank~=mage->dual
            array_view{ "Invoker", "Warden", "Berserker", "Thief" , "Shaman", "Shadow"  , "Falconer", "Assassin" }, // group dual~=mage->tank
            array_view{ "Warrior", "Knight", "Barbarian", "Hunter", "Herald", "Crusader", "Champion", "Deserter" }, // group tank->dual~=mage
            array_view{ "Samurai", "Fencer", "Swordsman", "Ranger", "Dancer", "Spectre" , "Wanderer", "Vagabond" }, // group dual->tank~=mage
            array_view{ "Acolyte", "Wizard", "Chieftain", "Arcane", "Priest", "Sorcerer", "Wraith"  , "Warlock"  }, // group mage->tank~=dual
        // other variants: Amazon, Beast, Demigod, Giant, Maiden, Marauder, Spirit, Soldier
        };
    };
} // namespace ds2srand::start

#endif//OLEGEASE_DS2SRAND_START_ALTER_HXX

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
