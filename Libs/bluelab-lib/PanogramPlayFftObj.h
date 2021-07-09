//
//  PanogramPlayFftObj.h
//  BL-Ghost
//
//  Created by Pan on 02/06/18.
//
//

#ifndef __BL_Ghost__PanogramPlayFftObj__
#define __BL_Ghost__PanogramPlayFftObj__

#include <deque>
using namespace std;

#include <bl_queue.h>

#include "FftProcessObj16.h"

#include <HistoMaskLine2.h>

class HistoMaskLine2;

class PanogramPlayFftObj : public ProcessObj
{
public:
    enum Mode
    {
        BYPASS,
        PLAY,
        RECORD
    };
    
    PanogramPlayFftObj(int bufferSize, int oversampling, int freqRes,
                       BL_FLOAT sampleRate);
    
    virtual ~PanogramPlayFftObj();
    
    void ProcessFftBuffer(WDL_TypedBuf<WDL_FFT_COMPLEX> *ioBuffer,
                          const WDL_TypedBuf<WDL_FFT_COMPLEX> *scBuffer = NULL);
    
    void Reset(int bufferSize, int oversampling,
               int freqRes, BL_FLOAT sampleRate) override;
    
    void SetMode(Mode mode);

    // Selection
    void SetNormSelection(BL_FLOAT x0, BL_FLOAT y0, BL_FLOAT x1, BL_FLOAT y1);

    void GetNormSelection(BL_FLOAT selection[4]);
    
    void SetSelectionEnabled(bool flag); //
    
    // Play
    void RewindToStartSelection();

    void RewindToNormValue(BL_FLOAT value);
    
    bool SelectionPlayFinished();
    
    BL_FLOAT GetPlayPosition();
    
    // Get normalized pos of play selection,
    // normalization is done inside selection
    BL_FLOAT GetSelPlayPosition();

    //
    void SetNumCols(int numCols);
    
    void SetIsPlaying(bool flag);
    void SetHostIsPlaying(bool flag);
    
    //
    void AddMaskLine(const HistoMaskLine2 &maskLine);
    
protected:
    void GetDataLine(const bl_queue<WDL_TypedBuf<BL_FLOAT> > &inData,
                     WDL_TypedBuf<BL_FLOAT> *data, int lineCount);

    void GetDataLineMask(const bl_queue<WDL_TypedBuf<BL_FLOAT> > &inData,
                         WDL_TypedBuf<BL_FLOAT> *data, int lineCount);
    
    // Shift a little, to have the sound played eactly when the bar passes
    // on the sound
    bool ShiftXPlayBar(int *xValue);
    bool ShiftXSelection(BL_FLOAT *xValue);
    
    //
    long mLineCount;
    
    bool mSelectionEnabled;
    BL_FLOAT mDataSelection[4];
    
    bool mSelectionPlayFinished;
    
    Mode mMode;
    
    // There are 4 magns and phases for one call to Process() if oversampling is 4
    
    //deque<WDL_TypedBuf<BL_FLOAT> > mCurrentMagns;
    //deque<WDL_TypedBuf<BL_FLOAT> > mCurrentPhases;
    //deque<HistoMaskLine2> mMaskLines;

    bl_queue<WDL_TypedBuf<BL_FLOAT> > mCurrentMagns;
    bl_queue<WDL_TypedBuf<BL_FLOAT> > mCurrentPhases;
    bl_queue<HistoMaskLine2> mMaskLines;
    
    int mNumCols;
    
    // Play button
    bool mIsPlaying;
    
    // Host play
    bool mHostIsPlaying;

private:
    // Tmp buffer
    WDL_TypedBuf<BL_FLOAT> mTmpBuf0;
    WDL_TypedBuf<BL_FLOAT> mTmpBuf1;
    WDL_TypedBuf<WDL_FFT_COMPLEX> mTmpBuf2;
    WDL_TypedBuf<BL_FLOAT> mTmpBuf3;
    WDL_TypedBuf<BL_FLOAT> mTmpBuf4;
    WDL_TypedBuf<BL_FLOAT> mTmpBuf5;
    WDL_TypedBuf<BL_FLOAT> mTmpBuf6;
    WDL_TypedBuf<BL_FLOAT> mTmpBuf7;
};

#endif /* defined(__BL_Ghost__PanogramPlayFftObj__) */
