//
//  BL_BatFftObj4.h
//  BL-Ghost
//
//  Created by Pan on 02/06/18.
//
//

#ifndef __BL_Pano__BatFftObj4__
#define __BL_Pano__BatFftObj4__

#include "FftProcessObj16.h"

// From ChromaFftObj
//
// From BatFftObj2 (directly)
//
class BLSpectrogram3;
class SpectrogramDisplay;
class HistoMaskLine2;

class SourceLocalisationSystem2;

class BatFftObj4 : public MultichannelProcess
{
public:
    BatFftObj4(int bufferSize, int oversampling, int freqRes,
                   BL_FLOAT sampleRate);
    
    virtual ~BatFftObj4();
    
    void ProcessInputFft(vector<WDL_TypedBuf<WDL_FFT_COMPLEX> * > *ioFftSamples,
                         const vector<WDL_TypedBuf<WDL_FFT_COMPLEX> > *scBuffer);
    
    void Reset(int bufferSize, int oversampling, int freqRes, BL_FLOAT sampleRate);
    
    BLSpectrogram3 *GetSpectrogram();
    
    void SetSpectrogramDisplay(SpectrogramDisplay *spectroDisplay);
    
    void SetSharpness(BL_FLOAT sharpness);
    
    void SetTimeSmooth(BL_FLOAT smooth);
    
    void SetFftSmooth(BL_FLOAT smooth);
    
    void SetEnabled(bool flag);
    
protected:
    void ComputeCoords(const WDL_TypedBuf<BL_FLOAT> &localization,
                       WDL_TypedBuf<BL_FLOAT> *coords);

    void ComputeAmplitudes(const WDL_TypedBuf<BL_FLOAT> &localization,
                           WDL_TypedBuf<BL_FLOAT> *amps);

    //
    void ApplySharpnessXY(vector<WDL_TypedBuf<BL_FLOAT> > *lines);
    void ReverseXY(vector<WDL_TypedBuf<BL_FLOAT> > *lines);
    
    void ApplySharpness(vector<WDL_TypedBuf<BL_FLOAT> > *lines);
    
    void ApplySharpness(WDL_TypedBuf<BL_FLOAT> *line);
    
    void TimeSmooth(vector<WDL_TypedBuf<BL_FLOAT> > *lines);

    //
    void GetLocalization(const WDL_TypedBuf<WDL_FFT_COMPLEX> &samples0,
                         const WDL_TypedBuf<WDL_FFT_COMPLEX> &samples1,
                         WDL_TypedBuf<BL_FLOAT> *localization);
    
    void ComputeLocalization2D(const WDL_TypedBuf<BL_FLOAT> &localizationX,
                               const WDL_TypedBuf<BL_FLOAT> &localizationY,
                               WDL_TypedBuf<BL_FLOAT> *localization2D);
    
    void ComputeCoords2D(int width, int height,
                         WDL_TypedBuf<BL_FLOAT> *coordsX,
                         WDL_TypedBuf<BL_FLOAT> *coordsY);

    
    BLSpectrogram3 *mSpectrogram;
    
    SpectrogramDisplay *mSpectroDisplay;
    
    long mLineCount;
    
    // Sharpness
    WDL_TypedBuf<BL_FLOAT> mSmoothWin;
    
    BL_FLOAT mSharpness;
    
    BL_FLOAT mTimeSmooth;
    vector<WDL_TypedBuf<BL_FLOAT> > mPrevLines;
    
    BL_FLOAT mFftSmooth;
    WDL_TypedBuf<BL_FLOAT> mPrevMagns[2];
    WDL_TypedBuf<BL_FLOAT> mPrevScMagns[2];
    
    SourceLocalisationSystem2 *mSourceLocSystem;
    
    //
    int mBufferSize;
    int mOverlapping;
    int mFreqRes;
    int mSampleRate;
    
    // For FIX_BAD_DISPLAY_HIGH_SAMPLERATES
    int mAddLineCount;
    
    bool mIsEnabled;
};

#endif /* defined(__BL_BL_Pano__BL_BatFftObj4__) */
