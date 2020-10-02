//
//  USTWidthAdjuster6.h
//  UST
//
//  Created by applematuer on 8/1/19.
//
//

#ifndef __UST__USTWidthAdjuster6__
#define __UST__USTWidthAdjuster6__

#include <vector>
#include <deque>

#ifndef M_PI
#define M_PI 3.14159265358979323846264
#endif

using namespace std;

#include "IPlug_include_in_plug_hdr.h"

// More intuitive smoothing

// USTWidthAdjuster3:
// - FIX buffer size problem
// - Decimation to manage high sample rates
//
// - USTWidthAdjuster4: manage to avoid any correlation problem
// anytime (test with SPAN mid/side + SideMinder)
//
// - USTWidthAdjuster5: update the width and check at each sample
// (instead of at each buffer)
// (not working well, debug...)
//
// - USTWidthAdjuster6: Use USTCorrelationComputer2
//
class USTCorrelationComputer2;
class USTStereoWidener;
class CParamSmooth;

class USTWidthAdjuster6
{
public:
    USTWidthAdjuster6(BL_FLOAT sampleRate);
    
    virtual ~USTWidthAdjuster6();
    
    void Reset(BL_FLOAT sampleRate);
    
    bool IsEnabled();
    void SetEnabled(bool flag);
    
    void SetSmoothFactor(BL_FLOAT smoothFactor);
    void SetWidth(BL_FLOAT width);
    
    void Update(BL_FLOAT l, BL_FLOAT r);
    
    BL_FLOAT GetLimitedWidth() const;
    
    //void DBG_SetCorrSmoothCoeff(BL_FLOAT smooth);
    
protected:
    void UpdateWidth();
    
    BL_FLOAT ComputeFirstGoodCorrWidth();
    BL_FLOAT ComputeCorrelationAux(BL_FLOAT width);
    
    void ComputeHistorySize(BL_FLOAT sampleRate);
    
    //
    bool mIsEnabled;
    
    BL_FLOAT mSmoothFactor;
    

    // Tmp width, for smoothing
    BL_FLOAT mCurrentSmoothWidth;

    // Width smooth
    CParamSmooth *mWidthSmoother;

    
    // Used to avoid jumps in the final width
    // Can happen if we reduce very quickly the stero width
    CParamSmooth *mFinalWidthSmoother;
    
    // Width from the user
    BL_FLOAT mUserWidth;
    
    BL_FLOAT mSampleRate;
    
    // Current detector for out of correlation
    USTCorrelationComputer2 *mCorrComputerNeg;
    
    // Tmp correlation computer, for finding the fist good width
    USTCorrelationComputer2 *mCorrComputerNegAux;
    
    USTStereoWidener *mStereoWidener;
    
    BL_FLOAT mPrevCorrelation;
    
    deque<BL_FLOAT> mSamplesHistory[2];
    long mHistorySize;
};

#endif /* defined(__UST__USTWidthAdjuster6__) */
