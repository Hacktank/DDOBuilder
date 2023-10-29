// ActiveEffect.h
//
// these are the effects in the list for any given breakdown
#pragma once

#include "BonusTypes.h"
#include "BreakdownTypes.h"
#include "ClassTypes.h"
#include "DamageReductionTypes.h"
#include "Dice.h"
#include "EnergyTypes.h"
#include "WeaponTypes.h"

class Character;
enum DividerType
{
    DT_none,
    DT_statBonus,
    DT_fullAbility,
};

enum ActiveEffectType
{
    ET_unknown,
    ET_amount,
    ET_amountVector,
    ET_amountPerLevel,
    ET_amountPerAp,
    ET_dice,
    ET_amountVectorPerClassLevel,
    ET_immunity,
    ET_DR,
    ET_dicePerClassLevel,
};

class ActiveEffect
{
    public:
        explicit ActiveEffect();
        ActiveEffect(
                BonusType bonusType,
                const std::string & name,
                size_t stacks,
                double amount,
                const std::string & tree);  // standard amount
        ActiveEffect(
                BonusType bonusType,
                const std::string & name,
                double amount,
                const std::string & tree);  // amount per AP
        ActiveEffect(
                BonusType bonusType,
                const std::string & name,
                size_t stacks,
                const Dice & dice,
                const std::string & tree);
        ActiveEffect(
                BonusType bonusType,
                const std::string & name,
                ClassType classType,
                const Dice & dice,
                const std::string & tree);
        ActiveEffect(
                BonusType bonusType,
                const std::string & name,
                size_t stacks,
                const std::vector<double> & amounts);
        ActiveEffect(
                BonusType bonusType,
                const std::string & name,
                ClassType classType,
                const std::vector<double> & amounts);
        ActiveEffect(
                BonusType bonusType,
                const std::string & name,
                double amountPerLevel,
                size_t stacks,
                ClassType classType);
        ActiveEffect(
                BonusType bonusType,
                const std::string & name,
                const std::vector<std::string> & immunities,
                size_t stacks);
        ActiveEffect(
                BonusType bonusType,
                const std::string & name,
                double amount,
                const std::list<DamageReductionType> & drTypes,
                size_t stacks);

        ActiveEffectType Type() const;
        BonusType Bonus() const;
        bool IsAmountPerAP() const;
        CString Name() const;
        CString Stacks() const;
        CString AmountAsText(double multiplier) const;
        CString AmountAsPercent() const;
        double TotalAmount(bool allowTruncate) const;
        void AddFeat(const std::string & featName);
        void AddStance(const std::string & stance);
        const std::vector<std::string> & Stances() const;
        void AddAnyOfStance(const std::string & stance);
        const std::vector<std::string> & AnyOfStances() const;
        void AddNoneOfStance(const std::string & stance);
        const std::vector<std::string> & NoneOfStances() const;
        const std::string & Tree() const;
        bool IsActive(const Character * pCharacter, WeaponType wt) const;
        void SetIsPercentage(bool isPercentage);
        bool IsPercentage() const;
        void SetPercentageValue(double amount) const;
        bool IsItemEffect() const;
        void SetIsItemEffect();
        void SetDivider(double divider, DividerType type);
        Dice GetDice() const;

        std::string Description() const;

        // for comparison and removal of duplicate entries
        bool operator<=(const ActiveEffect & other) const;
        bool operator==(const ActiveEffect & other) const;
        void AddStack();
        bool RevokeStack();
        void SetStacks(size_t count);
        size_t NumStacks() const;
        void SetClassLevel(size_t count);
        // optional
        void SetAmount(double amount);
        void SetEnergy(EnergyType type);
        void SetBreakdownDependency(BreakdownType bt);
        bool HasBreakdownDependency(BreakdownType bt) const;
        bool BasedOnClassLevel(ClassType type) const;
        void SetWholeNumbersOnly();
        void SetWeapon(WeaponType wt);  // used when an effect has WeaponClass_FocusGroup
        void SetStacksControl(const std::string & control, Character * pCharacter);
    private:
        ActiveEffectType m_type;
        double Amount() const;
        BonusType m_bonusType;
        std::string m_effectName;
        size_t m_numStacks;
        double m_amount;
        std::vector<double> m_amounts;
        Dice m_dice;
        bool m_bHasEnergy;                  // true if m_energy is valid
        EnergyType m_energy;
        std::vector<std::string> m_stances; // non blank if only active when a specific stance is active
        std::vector<std::string> m_anyOfStances; // non blank if only active when a specific stance is active
        std::vector<std::string> m_noneOfStances; // non blank if only active when a specific stance is not active
        std::string m_tree;                 // non blank if total depends on number of AP spent in tree
        BreakdownType m_bt;                 // specific breakdown we may be dependent on
        std::vector<std::string> m_feats;   // non blank if only active when a specific feat is trained
        std::vector<std::string> m_immunities;
        std::list<DamageReductionType> m_drTypes;
        // amount per trained level
        double m_amountPerLevel;
        size_t m_levelStacks;
        ClassType m_class;
        bool m_bIsPercentage;
        mutable double m_percentageAmount;
        bool m_bWholeNumbersOnly;
        bool m_bHasWeaponType;
        WeaponType m_weaponType;
        mutable bool m_clearValue;
        bool m_bIsItemEffect;
        double m_divider;
        DividerType m_dividerType;
        bool m_bHasStacksControl;
        std::string m_stacksControl;
        Character * m_pCharacter;
};
