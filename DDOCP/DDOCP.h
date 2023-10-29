// DDOCP.h
//
#pragma once
#include <list>
#include "Augment.h"
#include "EnhancementTree.h"
#include "Feat.h"
#include "GuildBuff.h"
#include "Item.h"
#include "OptionalBuff.h"
#include "Spell.h"
#include "SetBonus.h"

#ifndef __AFXWIN_H__
    #error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"
#include "CustomContextMenuManager.h"

class CDDOCPApp :
    public CWinAppEx
{
    public:
        CDDOCPApp();

        UINT  m_nAppLook;
        BOOL  m_bHiColorIcons;
        int m_originalShowState;

        virtual BOOL InitInstance();
        virtual int ExitInstance();

        const std::vector<Feat> & AllFeats() const;
        const std::vector<Feat> & StandardFeats() const;
        const std::vector<Feat> & HeroicPastLifeFeats() const;
        const std::vector<Feat> & RacialPastLifeFeats() const;
        const std::vector<Feat> & IconicPastLifeFeats() const;
        const std::vector<Feat> & EpicPastLifeFeats(const std::string & sphere) const;
        const std::vector<Feat> & SpecialFeats() const;
        const std::vector<Feat> & UniversalTreeFeats() const;
        const std::vector<Feat> & DestinyTreeFeats() const;
        const std::vector<Feat> & FavorFeats() const;

        const std::vector<EnhancementTree> & EnhancementTrees() const;
        const std::vector<Spell> & Spells() const;
        const std::vector<Item> & Items() const;
        const std::vector<Augment> & Augments() const;
        const std::vector<GuildBuff> & GuildBuffs() const;
        const std::vector<OptionalBuff> & OptionalBuffs() const;
        const std::vector<SetBonus> & SetBonuses() const;
        const std::vector<std::string> & IgnoreList() const;
        void UpdateIgnoreList(const std::vector<std::string> & itemList);


        virtual void PreLoadState();
        virtual void LoadCustomState();
        virtual void SaveCustomState();

        afx_msg void OnAppAbout();
        virtual BOOL LoadWindowPlacement(CRect& rectNormalPosition, int& nFflags, int& nShowCmd) override;
        DECLARE_MESSAGE_MAP()

    private:
        void LoadData();
        void LoadFeats(const std::string & path);
        void LoadEnhancements(const std::string & path);
        void LoadSpells(const std::string & path);
        void LoadItems(const std::string & path);
        void LoadAugments(const std::string & path);
        void LoadGuildBuffs(const std::string & path);
        void LoadOptionalBuffs(const std::string & path);
        void LoadSetBonuses(const std::string & path);
        void LoadIgnoreList(const std::string & path);
        void VerifyFeats();     // verify that all feats are consistent
        void SeparateFeats();   // breaks them out into groups based on type
        void VerifyLoadedData();
        void VerifyEnhancements();     // verify that all enhancements are consistent
        void VerifyAugments();     // verify that all augments are consistent
        void VerifySpells();    // verify all spells are consistent
        void VerifyItems();    // verify all items are consistent
        void VerifyOptionalBuffs();    // verify all are consistent
        void VerifySetBonuses();    // verify all are consistent
        CCustomContextMenuManager m_ourMenuManager; // construction of object replaces default implementation created in InitContextMenuManager
        // global data loaded at start up
        std::vector<Feat> m_allFeats;
        std::vector<Feat> m_standardFeats;
        std::vector<Feat> m_heroicPastLifeFeats;
        std::vector<Feat> m_racialPastLifeFeats;
        std::vector<Feat> m_iconicPastLifeFeats;
        std::vector<Feat> m_epicPastLifeFeatsArcane;
        std::vector<Feat> m_epicPastLifeFeatsDivine;
        std::vector<Feat> m_epicPastLifeFeatsMartial;
        std::vector<Feat> m_epicPastLifeFeatsPrimal;
        std::vector<Feat> m_specialFeats;
        std::vector<Feat> m_universalTreeFeats;
        std::vector<Feat> m_destinyTreeFeats;
        std::vector<Feat> m_favorFeats;
        std::vector<EnhancementTree> m_enhancementTrees;
        std::vector<Spell> m_spells;
        std::vector<Item> m_items;
        std::vector<Augment> m_augments;
        std::vector<GuildBuff> m_guildBuffs;
        std::vector<OptionalBuff> m_optionalBuffs;
        std::vector<SetBonus> m_setBonuses;
        std::vector<std::string> m_ignoreList;
};

extern CDDOCPApp theApp;
