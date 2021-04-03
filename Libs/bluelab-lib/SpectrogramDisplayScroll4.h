//
//  SpectrogramDisplayScroll4.h
//  BL-Chroma
//
//  Created by Pan on 14/06/18.
//
//

#ifndef __BL_Chroma__SpectrogramDisplayScroll4__
#define __BL_Chroma__SpectrogramDisplayScroll4__

#ifdef IGRAPHICS_NANOVG

#include <deque>
using namespace std;

#include <BLTypes.h>
#include <GraphControl12.h>

#include "IPlug_include_in_plug_hdr.h"


using namespace iplug;

// From SpectrogramDisplay
//
// Removed specificities that were for Ghost
// (MiniView, Background and Foreground spectrogram)
//
// SpectrogramDisplayScroll3: from SpectrogramDisplayScroll
// For GraphControl12
//

class BLSpectrogram4;
class NVGcontext;
class SpectrogramDisplayScroll4 : public GraphCustomDrawer
{
public:
    // delayPercent: delay that we bufferize, to fix when the data is a bit late
    // It is a percent of the spectrogram full width
    SpectrogramDisplayScroll4(Plugin *plug, BL_FLOAT delayPercent = 25.0);
    
    virtual ~SpectrogramDisplayScroll4();
    
    void Reset();
    
    // For InfrasonicViewer
    void ResetScroll();
    
    bool NeedUpdateSpectrogram();
    bool DoUpdateSpectrogram();
    
    void PreDraw(NVGcontext *vg, int width, int height) override;
    bool IsOwnedByGraph() override { return true; }
    
    // NeedRedraw() => always redraw!
    
    // Spectrogram
    void SetSpectrogram(BLSpectrogram4 *spectro,
                        BL_FLOAT left, BL_FLOAT top, BL_FLOAT right, BL_FLOAT bottom);
    
    void SetFftParams(int bufferSize, int overlapping, BL_FLOAT sampleRate);
    
    // Add and bufferize spectrogram line
    // the lines will be added progressively
    // (so with overlap > 1, the scrolling will be smoother)
    void AddSpectrogramLine(const WDL_TypedBuf<BL_FLOAT> &magns,
                            const WDL_TypedBuf<BL_FLOAT> &phases);
    
    void ShowSpectrogram(bool flag);
    void UpdateSpectrogram(bool flag);
    void UpdateColormap(bool flag);
    
    // For Logic
    void SetIsPlaying(bool flag);
    
    // Variable speed
    void SetSpeedMod(int speedMod);
    int GetSpeedMod();
    
    // We scale up a bit, to hide the borders
    BL_FLOAT GetScaleRatio();
    
protected:
    void AddSpectrogramLines(BL_FLOAT numLines);
    
    BL_FLOAT ComputeScrollOffsetPixels(int width);

    void ResetQueues();

    void RecomputeParams();
    
    // NanoVG
    NVGcontext *mVg;
    
    // Spectrogram
    BLSpectrogram4 *mSpectrogram;
    BL_FLOAT mSpectrogramBounds[4];
    
    int mNvgSpectroImage;
    WDL_TypedBuf<unsigned char> mSpectroImageData;
    
    bool mNeedUpdateSpectrogram;
    bool mNeedUpdateSpectrogramData;
    
    bool mNeedUpdateColormapData;
    bool mShowSpectrogram;
    
    // Colormap
    int mNvgColormapImage;
    WDL_TypedBuf<unsigned int> mColormapImageData;
    
    int mBufferSize;
    int mOverlapping;
    BL_FLOAT mSampleRate;
    unsigned long long mPrevSpectroLineNum;
    
    unsigned long long mPrevTimeMillis;
    
    // Offset for scrolling in "line" units
    BL_FLOAT mLinesOffset;
    
    // Add progressively spectrogram lines
    //
    // NOTE: can't really benefit from bl_queue
    // (we really need to pop, to decrease the list) sometimes
    deque<WDL_TypedBuf<BL_FLOAT> > mSpectroMagns;
    deque<WDL_TypedBuf<BL_FLOAT> > mSpectroPhases;
    
    BL_FLOAT mAddLineRemainder;
    
    // Members to avoid jumps in scrolling when restarting playback
    //
    
    // Get reference to plug, to know if the plug is currently playing
    Plugin *mPlug;
    bool mPrevIsPlaying;
    BL_FLOAT mPrevPixelOffset;
    
    bool mIsPlaying;
    
    // Variable speed
    int mSpeedMod;

    //
    BL_FLOAT mDelayPercent;
    
    //
    BL_FLOAT mSpectroTime;
    long int mStartTimeMillis;

    BL_FLOAT mSpectroLineDurationSec;
    BL_FLOAT mSpectroTotalDurationSec;
    
private:
    WDL_TypedBuf<unsigned int> mTmpBuf0;
};

#endif // IGRAPHICS_NANOVG

#endif /* defined(__BL_Chroma__SpectrogramDisplayScroll4__) */