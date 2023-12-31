#ifndef __STEREO_DEREVERB__
#define __STEREO_DEREVERB__

#include "IPlug_include_in_plug_hdr.h"
#include "IGraphics_include_in_plug_hdr.h"

#include "IPlugMidi.h"

#include <BLProfiler.h>

#include <SecureRestarter.h>
#include <DemoModeManager.h>

using namespace iplug;
using namespace igraphics;

class GUIHelper12;
class FftProcessObj16;
class StereoDeReverbProcess;
class ParamSmoother2;
class StereoDeReverb final : public Plugin
{
 public:
    StereoDeReverb(const InstanceInfo &info);
    virtual ~StereoDeReverb();
  
    void OnReset() override;
    void OnParamChange(int paramIdx) override;

    void OnUIOpen() override;
    void OnUIClose() override;
    
    void ProcessBlock(iplug::sample **inputs,
                      iplug::sample **outputs, int nFrames) override;
    
 protected:
    IGraphics *MyMakeGraphics();
    void MyMakeLayout(IGraphics *pGraphics);
    
    void CreateControls(IGraphics *pGraphics);
    
    void InitNull();
    void InitParams();
    void Init(int oversampling, int freqRes);
    void ApplyParams();

    void UpdateLatency();
    
    //
    FftProcessObj16 *mFftObj;
    StereoDeReverbProcess *mDeReverbObj;
  
    // Output gain
    BL_FLOAT mOutGain;
    ParamSmoother2 *mOutGainSmoother;

    BL_FLOAT mThreshold;
    BL_FLOAT mMix;
    bool mReverbOnly;
    
    //
    DemoModeManager mDemoManager;
  
    // Secure starters
    SecureRestarter mSecureRestarter;
    
    bool mUIOpened;
    bool mControlsCreated;

    bool mIsInitialized;
    
    GUIHelper12 *mGUIHelper;

 private:
    // Tmp buffers
    vector<WDL_TypedBuf<BL_FLOAT> > mTmpBuf0;
    vector<WDL_TypedBuf<BL_FLOAT> > mTmpBuf1;
    vector<WDL_TypedBuf<BL_FLOAT> > mTmpBuf2;
    
    BL_PROFILE_DECLARE;
};

#endif
