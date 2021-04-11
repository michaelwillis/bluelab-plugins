#ifndef PLUG_BYPASS_DETECTOR_H
#define PLUG_BYPASS_DETECTOR_H

// No so many host seem to support detection of plug bypass
// So use a custom object, to be touched in ProcessBlock(),
// and to be asked in OnIdle()
class PlugBypassDetector
{
 public:
    // 200 ms should be ok for 22050Hz, buffer size 2048
    // NOTE: 200ms is not enough, we have false positives
    PlugBypassDetector(int delayMs = 500/*200*/);
    virtual ~PlugBypassDetector();

    void TouchFromAudioThread();
    void SetTransportPlaying(bool flag);

    // Detects if OnIdle() is called lately
    // as the audio thread is still processing
    void TouchFromIdleThread();
    bool PlugIsBypassed();

 protected:
    int mDelayMs;
    
    long int mPrevAudioTouchTimeStamp;
    long int mPrevIdleTouchTimeStamp;
    
    bool mIsPlaying;
};

#endif