#ifndef GHOST_CUSTOM_DRAWER2_H
#define GHOST_CUSTOM_DRAWER2_H

#ifdef IGRAPHICS_NANOVG

#include <GraphControl12.h>

class GhostTrack2;
class GhostCustomDrawer2 : public GraphCustomDrawer
{
public:
    struct State
    {
        bool mBarActive;
        BL_FLOAT mBarPos;
        
        bool mSelectionActive;
        BL_FLOAT mSelection[4];
        
        bool mPlayBarActive;
        BL_FLOAT mPlayBarPos;
    };
    
    GhostCustomDrawer2(GhostTrack2 *track,
                       BL_FLOAT x0, BL_FLOAT y0, BL_FLOAT x1, BL_FLOAT y1,
                       State *state);
    
    virtual ~GhostCustomDrawer2() {}
    
    State *GetState();
    
    // The graph will destroy it automatically
    bool IsOwnedByGraph() override { return true; }
    
    // Draw after everything
    void PostDraw(NVGcontext *vg, int width, int height) override;
    
    //
    void ClearBar();
    
    void ClearSelection();
    
    void SetBarPos(BL_FLOAT pos);
    BL_FLOAT GetBarPos();
    
    void SetBarActive(bool flag);
    bool IsBarActive();
    
    void SetSelection(BL_FLOAT x0, BL_FLOAT y0,
                      BL_FLOAT x1, BL_FLOAT y1);
    void SetSelectionActive(bool flag);
    bool GetSelectionActive();
    
    void GetSelection(BL_FLOAT *x0, BL_FLOAT *y0,
                      BL_FLOAT *x1, BL_FLOAT *y1);
    
    //void UpdateZoomSelection(BL_FLOAT zoomChange);
    void UpdateZoom(BL_FLOAT zoomChange);
    
    bool IsSelectionActive();
    
    BL_FLOAT GetPlayBarPos();
    void SetPlayBarPos(BL_FLOAT pos, bool activate);
    
    bool IsPlayBarActive();
    void SetPlayBarActive(bool flag);
    
    // Normalized inside selection
    void SetSelPlayBarPos(BL_FLOAT pos);

    bool NeedRedraw() override;
        
protected:
    void DrawBar(NVGcontext *vg, int width, int height);
    void DrawSelection(NVGcontext *vg, int width, int height);
    void DrawPlayBar(NVGcontext *vg, int width, int height);
    
    //
    BL_FLOAT mBounds[4];

    GhostTrack2 *mTrack;
    
    State *mState;

    bool mNeedRedraw;
};

#endif

#endif
