#ifndef DS2SRANDOMIZER_CLASSES_ALTER_HPP
#define DS2SRANDOMIZER_CLASSES_ALTER_HPP

#include <array>
#include <string>

namespace ds2srand::classes {
    struct Alter {
        using array_view = std::array< std::string_view, 8 >;
        inline static std::array< array_view, 8 > replacements{
        // God - Good   | Neu - Neutral | Evi - Evil
        // Law - Lawful | Neu - Neutral | Cha - Chaotic
        // no depressed NeuNeu
        //               GodCha     NeuLaw    EviNeu       NeuCha    GodLaw    EviLaw      GodNeu      EviCha
        //override base "Warrior", "Knight", "Swordsman", "Bandit", "Cleric", "Sorcerer", "Explorer", "Deprived"
            array_view{ "Titan  ", "Hero  ", "Shade    ", "Rogue ", "Monk  ", "Mage    ", "Bard    ", "Lich    " }, // group specific
            array_view{ "       ", "Beast ", "Trickster", "Pirate", "      ", "        ", "Traveler", "Deprived" }, // group tank~=dual~=mage
            array_view{ "Soldier", "Slayer", "Gladiator", "Bandit", "Scion ", "Guardian", "Explorer", "Prisoner" }, // group tank~=dual->mage
            array_view{ "Paladin", "Druid ", "Chieftain", "Ninja ", "Cleric", "        ", "Falconer", "Demented" }, // group tank~=mage->dual
            array_view{ "Templar", "Warden", "Berserker", "Thief ", "Shaman", "        ", "Sentinel", "Vagabond" }, // group dual~=mage->tank
            array_view{ "Warrior", "Knight", "Barbarian", "Hunter", "Giant ", "        ", "Champion", "Deserter" }, // group tank->dual~=mage
            array_view{ "Samurai", "Fencer", "Swordsman", "Ranger", "Keeper", "        ", "Wanderer", "Assassin" }, // group dual->tank~=mage
            array_view{ "Acolyte", "Wizard", "Mercenary", "Arcane", "Priest", "Sorcerer", "        ", "Warlock " }, // group mage->tank~=dual
        // variants: Angel, Archer, Dancer, Jester
        };
    };
} // namespace ds2srand::classes

#endif//DS2SRANDOMIZER_CLASSES_ALTER_HPP

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
