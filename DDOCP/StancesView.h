// StancesView.h
//
#pragma once
#include "Resource.h"
#include <vector>
#include "Character.h"
#include "StanceButton.h"
#include "InfoTip.h"
#include "WeaponGroup.h"
#include <memory>

struct SliderItem
{
public:
    UINT m_sliderControlId;
    std::string m_name;
    std::unique_ptr<CStatic> m_label;
    std::unique_ptr<CSliderCtrl> m_slider;
    size_t m_creationCount;
    int m_position;
    int m_sliderMin;
    int m_sliderMax;
    SliderItem() :
            m_sliderControlId(0),
            m_creationCount(0),
            m_position(0),
            m_sliderMin(0),
            m_sliderMax(0)
    {
    }
    void CreateControls()
    {
        m_label = std::make_unique<CStatic>();
        m_slider = std::make_unique<CSliderCtrl>();
    };
};

class CStancesView :
    public CFormView,
    public CharacterObserver
{
    public:
        enum { IDD = IDD_STANCES_VIEW };
        DECLARE_DYNCREATE(CStancesView)

#ifdef _DEBUG
        virtual void AssertValid() const;
    #ifndef _WIN32_WCE
    virtual void Dump(CDumpContext& dc) const;
#endif
#endif
        virtual void OnInitialUpdate();
        const std::vector<CStanceButton *> & ActiveUserStances() const;
        const std::vector<CStanceButton *> & ActiveAutoStances() const;
        const CStanceButton * GetStance(const std::string & stanceName) const;
        bool IsStanceActive(const std::string & name, WeaponType wt) const;
        const SliderItem * GetSlider(const std::string & name) const;
    protected:
        CStancesView();           // protected constructor used by dynamic creation
        virtual ~CStancesView();

        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg LRESULT OnNewDocument(WPARAM wParam, LPARAM lParam);
        afx_msg void OnMouseMove(UINT nFlags, CPoint point);
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
        afx_msg void OnHScroll(UINT sbCode, UINT nPos, CScrollBar * pScrollbar);
        DECLARE_MESSAGE_MAP()

    private:
        // Character observers
        virtual void UpdateNewStance(Character * charData, const Stance & stance) override;
        virtual void UpdateRevokeStance(Character * charData, const Stance & stance) override;
        virtual void UpdateStanceActivated(Character * charData, const std::string & stanceName) override;
        virtual void UpdateStanceDeactivated(Character * charData, const std::string & stanceName) override;
        // updates used for sliders
        virtual void UpdateEnhancementEffect(Character * charData, const std::string & enhancementName,  const EffectTier & effect) override;
        virtual void UpdateEnhancementEffectRevoked(Character * charData, const std::string & enhancementName, const EffectTier & effect) override;
        virtual void UpdateFeatEffect(Character * charData, const std::string & featName,  const Effect & effect) override;
        virtual void UpdateFeatEffectRevoked(Character * charData, const std::string & featName, const Effect & effect) override;
        virtual void UpdateItemEffect(Character * charData, const std::string & itemName,  const Effect & effect) override;
        virtual void UpdateItemEffectRevoked(Character * charData, const std::string & itemName, const Effect & effect) override;

        void PositionWindow(CWnd * pWnd, CRect * itemRect, int maxX);

        void GetStance(const std::string & stanceName, const std::vector<CStanceButton *> & items, const CStanceButton **) const;
        void CreateStanceWindows();
        bool AddStance(
                const Stance & stance,
                std::vector<CStanceButton *> & items);
        void AddStance(const Stance & stance);
        void RevokeStance(
                const Stance & stance,
                std::vector<CStanceButton *> & items);
        void RevokeStance(const Stance & stance);
        void DestroyStances(std::vector<CStanceButton *> & items);
        void DestroyAllStances();

        void ShowTip(const CStanceButton & item, CRect itemRect);
        void HideTip();
        void SetTooltipText(const CStanceButton & item, CPoint tipTopLeft, CPoint tipAlternate);
        void UpdateSlidersForEffect(const Effect & effect, bool bApply);
        std::vector<SliderItem>::iterator CreateSliderForEffect(const Effect& effect);
        std::vector<SliderItem>::iterator GetSlider(const Effect & effect);
        std::vector<SliderItem>::iterator GetSlider(UINT controlId);
        void AddToWeaponGroup(const Effect & effect);
        void RemoveFromWeaponGroup(const Effect & effect);
        void StanceActivated(
                std::vector<CStanceButton *> & items,
                const std::string & stanceName,
                bool enforceSingleActive = true);
        void StanceDeactivated(
                std::vector<CStanceButton *> & items,
                const std::string & stanceName);
        void AddActiveStances(
                const std::vector<CStanceButton *> & items,
                std::vector<CStanceButton *> * stances) const;

        CDocument * m_pDocument;
        Character * m_pCharacter;
        std::vector<SliderItem> m_sliders;
        CStatic m_staticHiddenSizer;
        CStatic m_userStances;
        CStatic m_userStancesArcane;
        CStatic m_userStancesDivine;
        CStatic m_userStancesMartial;
        CStatic m_userStancesPrimal;
        CStatic m_userStancesIconic;
        CStatic m_userStancesMajorForm;
        CStatic m_userStancesMantle;
        CStatic m_userStancesImbue;
        CStatic m_autoStances;
        std::vector<CStanceButton *> m_userStancebuttons;
        std::vector<CStanceButton *> m_userStancebuttonsArcane;
        std::vector<CStanceButton *> m_userStancebuttonsDivine;
        std::vector<CStanceButton *> m_userStancebuttonsMartial;
        std::vector<CStanceButton *> m_userStancebuttonsPrimal;
        std::vector<CStanceButton *> m_userStancebuttonsIconic;
        std::vector<CStanceButton *> m_userStancebuttonsMajorForm;
        std::vector<CStanceButton *> m_userStancebuttonsMantle;
        std::vector<CStanceButton *> m_userStancebuttonsImbue;
        std::vector<CStanceButton *> m_autoStancebuttons;
        CInfoTip m_tooltip;
        bool m_showingTip;
        bool m_tipCreated;
        const CStanceButton * m_pTooltipItem;
        int m_nextStanceId;
        int m_nextSliderId;
        std::vector<WeaponGroup> m_weaponGroups;
};
