// BreakdownItem.h
//
// A standard base class that all items that can have breakdowns calculated
// for them inherit from to allow a common interface to the CBreakdownsView
#pragma once

#include "ActiveEffect.h"
#include "BreakdownTypes.h"
#include "Character.h"
#include "Effect.h"
#include "ObserverSubject.h"

namespace MfcControls
{
    class CTreeListCtrl;
};

enum ColumnOrder
{
    CO_Source = 0,
    CO_Stacks,
    CO_Value,
    CO_BonusType,

    CO_count
};

class CBreakdownsView;
class BreakdownItem;

class BreakdownObserver :
    public Observer<BreakdownItem>
{
    public:
        virtual void UpdateTotalChanged(BreakdownItem * item, BreakdownType type) {};
};

// this is the base class for all items that can have a break down done for them
// it provides the common interface that allows information from feats, enhancements
// and items to filter in and be collated.
class BreakdownItem :
    public CharacterObserver,
    public Subject<BreakdownObserver>,
    public BreakdownObserver
{
    public:
        BreakdownItem(
                BreakdownType type,
                MfcControls::CTreeListCtrl * treeList,
                HTREEITEM hItem);
        virtual ~BreakdownItem();

        virtual void SetCharacter(Character * charData, bool observe);

        BreakdownType Type() const;
        virtual CString Title() const = 0;
        virtual CString Value() const = 0;
        virtual double Multiplier() const;
        void Populate();            // updates the HTREEITEM
        void PopulateBreakdownControl(CListCtrl * pControl);
        virtual double Total() const;
        virtual double CappedTotal() const;

        void AddAbility(AbilityType ability);
        void RemoveFirstAbility(AbilityType ability);
        void AddAbility(AbilityType ability, const std::vector<std::string> & stances, WeaponType wt);
        void RemoveFirstAbility(AbilityType ability, const std::vector<std::string> & stances, WeaponType wt);

        void SetWeapon(WeaponType wt, size_t weaponCriticalMultiplier);
        WeaponType Weapon() const;

        double GetEffectValue(BonusType bt) const;
    protected:
        void AddOtherEffect(const ActiveEffect & effect);
        void AddFeatEffect(const ActiveEffect & effect);
        void AddEnhancementEffect(const ActiveEffect & effect);
        void AddItemEffect(const ActiveEffect & effect);

        void RevokeOtherEffect(const ActiveEffect & effect);
        void RevokeFeatEffect(const ActiveEffect & effect);
        void RevokeEnhancementEffect(const ActiveEffect & effect);
        void RevokeItemEffect(const ActiveEffect & effect);

        bool UpdateTreeItemTotals();
        bool UpdateTreeItemTotals(std::vector<ActiveEffect> * list);
        bool UpdateEffectAmounts(std::vector<ActiveEffect> * list, BreakdownType bt);
        bool UpdateEffectAmounts(std::vector<ActiveEffect> * list, ClassType type);

        virtual bool StacksByMultiplication() const { return false;};

        virtual void CreateOtherEffects() = 0;
        virtual bool AffectsUs(const Effect & effect) const = 0;

        virtual void AppendItems(CListCtrl * pControl) {};  // base does nothing

        Character * m_pCharacter;
        std::vector<ActiveEffect> m_otherEffects;       // these always stack
        std::vector<ActiveEffect> m_effects;       // these always stack
        std::vector<ActiveEffect> m_itemEffects;        // highest of a given type counts

        // CharacterObserver overrides
        virtual void UpdateClassChanged(Character * charData, ClassType classFrom, ClassType classTo, size_t level) override;
        virtual void UpdateStanceActivated(Character * charData, const std::string & stanceName) override;
        virtual void UpdateStanceDeactivated(Character * charData, const std::string & stanceName) override;
        virtual void UpdateFeatTrained(Character * charData, const std::string & featName) override;
        virtual void UpdateFeatRevoked(Character * charData, const std::string & featName) override;
        virtual void UpdateFeatEffect(Character * pCharacater, const std::string & featName,  const Effect & effect) override;
        virtual void UpdateFeatEffectRevoked(Character * pCharacater, const std::string & featName, const Effect & effect) override;
        virtual void UpdateEnhancementEffect(Character * charData, const std::string & enhancementName,  const EffectTier & effect) override;
        virtual void UpdateEnhancementEffectRevoked(Character * charData, const std::string & enhancementName, const EffectTier & effect) override;
        virtual void UpdateItemEffect(Character * charData, const std::string & itemName,  const Effect & effect) override;
        virtual void UpdateItemEffectRevoked(Character * charData, const std::string & itemName, const Effect & effect) override;
        virtual void UpdateAPSpentInTreeChanged(Character * charData, const std::string & treeName) override;

        // BreakdownObserver overrides (may be specialised in inheriting classes)
        virtual void UpdateTotalChanged(BreakdownItem * item, BreakdownType type) override;

        void NotifyTotalChanged();
        void SetHTreeItem(HTREEITEM hItem);

        std::vector<ActiveEffect> AllActiveEffects() const;
        void AddItems(CListCtrl * pControl);
    private:
        void AddActiveItems(const std::vector<ActiveEffect> & effects, CListCtrl * pControl, bool bShowMultiplier);
        void AddActivePercentageItems(const std::vector<ActiveEffect> & effects, CListCtrl * pControl);
        void AddDeactiveItems(const std::vector<ActiveEffect> & effects, CListCtrl * pControl, bool bShowMultiplier);
        virtual double SumItems(const std::vector<ActiveEffect> & effects, bool bApplyMultiplier) const;
        void AddEffect(std::vector<ActiveEffect> * effectList, const ActiveEffect & effect);
        void RevokeEffect(std::vector<ActiveEffect> * effectList, const ActiveEffect & effect);
        bool GetActiveEffect(Character * pCharacter, const std::string & name, const Effect & effect, ActiveEffect * activeEffect);
        MfcControls::CTreeListCtrl * m_pTreeList;
        HTREEITEM m_hItem;
        BreakdownType m_type;
    protected:
        AbilityType LargestStatBonus();
        double DoPercentageEffects(const std::vector<ActiveEffect> & effects, double total) const;
        void RemoveInactive(std::vector<ActiveEffect> * effects, std::vector<ActiveEffect> * inactiveEffects) const;
        void RemoveNonStacking(std::vector<ActiveEffect> * effects, std::vector<ActiveEffect> * nonStackingEffects) const;
        void RemoveTemporary(std::vector<ActiveEffect> * effects, std::vector<ActiveEffect> * temporaryEffects) const;
        struct AbilityStance
        {
            AbilityType ability;
            std::vector<std::string> stances;
            WeaponType weapon;
            bool operator==(const AbilityStance & other) const
            {
                return ability == other.ability
                        && stances == other.stances
                        && weapon == other.weapon;
            }
        };
        std::vector<AbilityStance> m_mainAbility; // ability types that are used for this breakdown (highest of those listed)
        std::vector<AbilityStance> m_baseAbility; // restore list on change of character
        bool m_bHasWeapon;
        WeaponType m_weapon;
        size_t m_weaponCriticalMultiplier;
        bool m_bAddEnergies;

        friend class BreakdownItemWeapon;
        friend class CBreakdownsView;
        friend class BreakdownItemEnergyCasterLevel;
        friend class BreakdownItemSchoolCasterLevel;
};
