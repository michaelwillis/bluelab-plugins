//
//  Spectrum.cpp
//  Denoiser
//
//  Created by Apple m'a Tuer on 16/05/17.
//
//

#include "Utils.h"
#include "ColorMap.h"
#include "PPMFile.h"

#include "BLSpectrogram.h"

#define MAX_PATH 512

#define MIN_DB -120.0
#define EPS_DB 1e-15

BLSpectrogram::BLSpectrogram(int height, int maxCols)
{
    mHeight = height;
    
    mMaxCols = maxCols;
    
    mMagnsMultiplier = 1.0;
    mPhasesMultiplier = 1.0;
    mPhasesMultiplier2 = 1.0;
    
    mGain = 1.0;
    
    mDisplayMagns = true;
    
    mDisplayPhasesX = false;
    mDisplayPhasesY = false;
    
    unsigned char col0[3] = { 0, 255, 0 };
    
    unsigned char col1[3] = { 255, 0, 0 };
    //unsigned char col1[3] = { 255, 128, 0 };
    
    mColorMap = new ColorMap(col0, col1);
    
    //mColorMap->SavePPM("colormap.ppm");
    
#if CLASS_PROFILE
    mTimer.Reset();
    mTimerCount = 0;
#endif
}

BLSpectrogram::~BLSpectrogram() {}

void
BLSpectrogram::SetMultipliers(double magnMult, double phaseMult)
{
    mMagnsMultiplier = magnMult;
    mPhasesMultiplier = phaseMult;
}

void
BLSpectrogram::SetGain(double gain)
{
    mGain = gain;
}

void
BLSpectrogram::SetDisplayMagns(bool flag)
{
    mDisplayMagns = flag;
}


void
BLSpectrogram::SetDisplayPhasesX(bool flag)
{
    mDisplayPhasesX = flag;
}

void
BLSpectrogram::SetDisplayPhasesY(bool flag)
{
    mDisplayPhasesY = flag;
}

void
BLSpectrogram::Reset()
{
    mMagns.clear();
    mPhases.clear();
}

int
BLSpectrogram::GetNumCols()
{
    return mMagns.size();
}

int
BLSpectrogram::GetMaxNumCols()
{
    return mMaxCols;
}


int
BLSpectrogram::GetHeight()
{
    return mHeight;
}

BLSpectrogram *
BLSpectrogram::Load(const char *fileName)
{
    // TODO
    return NULL;
}

void
BLSpectrogram::Save(const char *filename)
{
    // Magns
    char fullFilenameMagns[MAX_PATH];
    sprintf(fullFilenameMagns, "/Volumes/HDD/Share/%s-magns", filename);
    
    FILE *magnsFile = fopen(fullFilenameMagns, "w");
    
    for (int j = 0; j < mMagns.size(); j++)
    {
        const WDL_TypedBuf<double> &magns = mMagns[j];
        
        for (int i = 0; i < mHeight; i++)
        {
            fprintf(magnsFile, "%g ", magns.Get()[i]);
        }
        
        fprintf(magnsFile, "\n");
    }
    fclose(magnsFile);
    
    // Phases
    char fullFilenamePhases[MAX_PATH];
    sprintf(fullFilenamePhases, "/Volumes/HDD/Share/%s-phases", filename);
    
    FILE *phasesFile = fopen(fullFilenamePhases, "w");
    
    for (int j = 0; j < mPhases.size(); j++)
    {
        const WDL_TypedBuf<double> &phases = mPhases[j];
        
        for (int i = 0; i < mHeight; i++)
        {
            fprintf(phasesFile, "%g ", phases.Get()[i]);
        }
        
        fprintf(phasesFile, "\n");
    }
    
    fclose(phasesFile);
}

BLSpectrogram *
BLSpectrogram::LoadPPM(const char *filename,
                      double magnsMultiplier, double phasesMultiplier)
{
    char fullFilename[MAX_PATH];
    sprintf(fullFilename, "/Volumes/HDD/Share/BlueLabAudio-Debug/%s", filename);
    
    PPMFile::PPMImage *image = PPMFile::ReadPPM(fullFilename);
    
    BLSpectrogram *result = ImageToSpectrogram(image, false,
                                               magnsMultiplier, phasesMultiplier);
    
    return result;
}

void
BLSpectrogram::SavePPM(const char *filename)
{
    SavePPM(filename, 255);
}

BLSpectrogram *
BLSpectrogram::LoadPPM16(const char *filename,
                         double magnsMultiplier, double phasesMultiplier)
{
    char fullFilename[MAX_PATH];
    sprintf(fullFilename, "/Volumes/HDD/Share/BlueLabAudio-Debug/%s", filename);
    
    PPMFile::PPMImage *image = PPMFile::ReadPPM16(fullFilename);
    
    BLSpectrogram *result = ImageToSpectrogram(image, true,
                                               magnsMultiplier, phasesMultiplier);
    
    return result;
}

void
BLSpectrogram::SavePPM16(const char *filename)
{
    SavePPM(filename, 65535);
}

BLSpectrogram *
BLSpectrogram::LoadPPM32(const char *filename)
{
    // Magns
    char magnsFullFilename[MAX_PATH];
    sprintf(magnsFullFilename, "/Volumes/HDD/Share/BlueLabAudio-Debug/%s-magns.ppm", filename);
    
    PPMFile::PPMImage *magnsImage = PPMFile::ReadPPM16(magnsFullFilename);
    
    // Phases
    char phasesFullFilename[MAX_PATH];
    sprintf(phasesFullFilename, "/Volumes/HDD/Share/BlueLabAudio-Debug/%s-phases.ppm", filename);
    
    PPMFile::PPMImage *phasesImage = PPMFile::ReadPPM16(phasesFullFilename);
    
    BLSpectrogram *result = ImagesToSpectrogram(magnsImage, phasesImage);
    
    return result;
}

void
BLSpectrogram::SavePPM32(const char *filename)
{
    if (mMagns.empty())
        return;
    
    int maxValue = 65000;
    
    //UnwrapPhases();
    
    // Magns
    char magnsFullFilename[MAX_PATH];
    sprintf(magnsFullFilename, "/Volumes/HDD/Share/BlueLabAudio-Debug/%s-magns.ppm", filename);
    FILE *magnsFile = fopen(magnsFullFilename, "w");
    
    // Header
    fprintf(magnsFile, "P3\n");
    fprintf(magnsFile, "%ld %d\n", mMagns.size(), mMagns[0].GetSize());
    fprintf(magnsFile, "%d\n", maxValue);
    
    // Phases
    char phasesFullFilename[MAX_PATH];
    sprintf(phasesFullFilename, "/Volumes/HDD/Share/BlueLabAudio-Debug/%s-phases.ppm", filename);
    FILE *phasesFile = fopen(phasesFullFilename, "w");
    
    // Header
    fprintf(phasesFile, "P3\n");
    fprintf(phasesFile, "%ld %d\n", mPhases.size(), mPhases[0].GetSize());
    fprintf(phasesFile, "%d\n", maxValue);
    
    
    // Data
    for (int i = mHeight - 1; i >= 0 ; i--)
    {
        for (int j = 0; j < mMagns.size(); j++)
        {
            const WDL_TypedBuf<double> &magns = mMagns[j];
            const WDL_TypedBuf<double> &phases = mPhases[j];
            
            float magnValue = magns.Get()[i];
            fprintf(magnsFile, "%d %d 0\n", ((short *)&magnValue)[0], ((short *)&magnValue)[1]);
            
            float phaseValue = phases.Get()[i];
            fprintf(phasesFile, "%d %d 0\n", ((short *)&phaseValue)[0], ((short *)&phaseValue)[1]);
        }
        
        fprintf(magnsFile, "\n");
        fprintf(phasesFile, "\n");
    }
    fclose(magnsFile);
    fclose(phasesFile);
}

void
BLSpectrogram::AddLine(const WDL_TypedBuf<double> &magns,
                       const WDL_TypedBuf<double> &phases)
{
    if ((magns.GetSize() < mHeight) ||
        (phases.GetSize() < mHeight))
    {
        return;
    }
    
    WDL_TypedBuf<double> magns0 = magns;
    WDL_TypedBuf<double> phases0 = phases;
    
    if ((magns.GetSize() > mHeight) ||
        (phases.GetSize() > mHeight))
    {
        Utils::DecimateSamples(&magns0, magns,
                               ((double)magns.GetSize())/mHeight);
        
        Utils::DecimateSamples(&phases0, phases,
                               ((double)phases.GetSize())/mHeight);
    }
    
    mMagns.push_back(magns);
    mPhases.push_back(phases);
    
    if (mMaxCols > 0)
    {
        if (mMagns.size() > mMaxCols)
        {
            mMagns.pop_front();
        }
        
        if (mPhases.size() > mMaxCols)
        {
            mPhases.pop_front();
        }
    }
}

bool
BLSpectrogram::GetLine(int index,
                     WDL_TypedBuf<double> *magns,
                     WDL_TypedBuf<double> *phases)
{
    if ((index >= mMagns.size()) ||
        (index >= mPhases.size()))
        return false;
        
    *magns = mMagns[index];
    *phases = mPhases[index];
        
    return true;
}

void
BLSpectrogram::UnwrapPhases()
{
    UnwrapPhases(&mPhases);
}

void
BLSpectrogram::GetImageDataRGBA(int width, int height, unsigned char *buf)
{
#if CLASS_PROFILE
    mTimer.Start();
#endif
    
    // Empty the buffer
    // Because the spectrogram may be not totally full
    memset(buf, 0, width*height*4);
    
    // Set alpha to 255
    for (int i = 0; i < width*height; i++)
    {
        buf[i*4 + 3] = 255;
    }
    
    // Unwrap copy of phases
    //deque<WDL_TypedBuf<double> > phasesUnW = mPhases;
    vector<WDL_TypedBuf<double> > phasesUnW;
    
    //if (mDisplayPhases)
    
    //UnwrapPhases2(&phasesUnW, mDisplayPhasesX, mDisplayPhasesY);
    UnwrapPhases3(mPhases, &phasesUnW, mDisplayPhasesX, mDisplayPhasesY);
    
    int maxValue = 255;
    
    // Data
    for (int j = 0; j < mMagns.size(); j++)
    {
        const WDL_TypedBuf<double> &magns = mMagns[j];
        const WDL_TypedBuf<double> &phases = phasesUnW[j];
        
        for (int i = mHeight - 1; i >= 0 ; i--)
        {
            double magnValue = magns.Get()[i];
            double phaseValue = phases.Get()[i];
            
            if (mColorMap == NULL)
            {
                // Increase
                magnValue = sqrt(magnValue);
                magnValue = sqrt(magnValue);
                
                double magnColor = magnValue*mMagnsMultiplier*(double)maxValue;
                if (magnColor > maxValue)
                    magnColor = maxValue;
            
                double phaseColor = phaseValue*mPhasesMultiplier*(double)maxValue;
                if (phaseColor > maxValue)
                    phaseColor = maxValue;
            
                int imgIdx = (j + i*height)*4;
                buf[imgIdx] = 0;
                buf[imgIdx + 1] = (int)phaseColor;
                buf[imgIdx + 2] = (int)magnColor;
                buf[imgIdx + 3] = 255;
            }
            else
                // Apply colormap
            {
                // Magns
                unsigned char magnColor[3] = { 0, 0, 0 };
                if (mDisplayMagns)
                {
                    double magnMult = magnValue;
                
                    // Increase contrast
                    //magnMult = magnMult*magnMult;
                    //magnMult = magnMult*magnMult;
                
                    //magnMult = sqrt(magnMult);
                    //magnMult = sqrt(magnMult);
                
                    magnMult *= mGain;
                
                    //magnMult = Utils::AmpToDB(magnMult, EPS_DB, MIN_DB);
                    //magnMult = (magnMult - MIN_DB)/(-MIN_DB);
                
                    mColorMap->GetColor(magnMult, magnColor);
                }
                
                // Phases
                double phaseColor = 0.0;
                if (mDisplayPhasesX || mDisplayPhasesY)
                {
                    double phaseMult = phaseValue*mPhasesMultiplier*mPhasesMultiplier2;
                    
                    phaseColor = phaseMult*(double)maxValue;
                    if (phaseColor > maxValue)
                        phaseColor = maxValue;
                }
                
                int imgIdx = (j + i*height)*4;
                
                buf[imgIdx] = (int)phaseColor;;
                buf[imgIdx + 1] = magnColor[1];
                buf[imgIdx + 2] = magnColor[0];
                buf[imgIdx + 3] = 255;
            }
        }
    }
    
#if CLASS_PROFILE
    mTimer.Stop();
    mTimerCount++;
    
    if (mTimerCount % 50 == 0)
    {
        long t = mTimer.Get();
        
        char message[512];
        sprintf(message, "elapsed: %ld ms\n", t);
        Debug::AppendMessage("profile.txt", message);
        
        mTimer.Reset();
    }
#endif
}

void
BLSpectrogram::UnwrapPhases(deque<WDL_TypedBuf<double> > *ioPhases)
{
    if (ioPhases->empty())
        return;
    
#if 1
    // Unwrap for vertical lines
    for (int j = 0; j < ioPhases->size(); j++)
    {
        WDL_TypedBuf<double> &phases = (*ioPhases)[j];
        
        double prevPhase = phases.Get()[0];
        
        while(prevPhase < 0.0)
            prevPhase += 2.0*M_PI;
        
        for (int i = 0; i < mHeight; i++)
        {
            double phase = phases.Get()[i];
            
            while(phase < prevPhase)
                phase += 2.0*M_PI;
            
            phases.Get()[i] = phase;
            
            prevPhase = phase;
        }
    }
#endif
    
#if 1
    // Unwrap for horizontal lines
    for (int i = 0; i < mHeight; i++)
    {
        WDL_TypedBuf<double> &phases0 = (*ioPhases)[0];
        double prevPhase = phases0.Get()[i];
        
        while(prevPhase < 0.0)
            prevPhase += 2.0*M_PI;
        
        for (int j = 0; j < mPhases.size(); j++)
        {
            WDL_TypedBuf<double> &phases = (*ioPhases)[j];
            
            double phase = phases.Get()[i];
            
            while(phase < prevPhase)
                phase += 2.0*M_PI;
            
            phases.Get()[i] = phase;
            
            prevPhase = phase;
        }
    }
#endif
}

void
BLSpectrogram::UnwrapPhases2(deque<WDL_TypedBuf<double> > *ioPhases,
                             bool horizontal, bool vertical)
{
    if (ioPhases->empty())
        return;
    
    if (horizontal && vertical)
        mPhasesMultiplier2 = 0.015;
    else
        mPhasesMultiplier2 = 1.0;
        
    if (horizontal)
    {
        // Unwrap for horizontal lines
        for (int i = 0; i < mHeight; i++)
        {
            WDL_TypedBuf<double> &phases0 = (*ioPhases)[0];
            double prevPhase = phases0.Get()[i];
        
            while(prevPhase < 0.0)
            {
                prevPhase += 2.0*M_PI;
            }
            
            double incr = 0.0;
            for (int j = 0; j < ioPhases->size(); j++)
            {
                WDL_TypedBuf<double> &phases = (*ioPhases)[j];
            
                double phase = phases.Get()[i];
                
                if (phase < prevPhase)
                    phase += incr;
                
                while(phase < prevPhase)
                {
                    phase += 2.0*M_PI;
                    incr += 2.0*M_PI;
                }
            
                phases.Get()[i] = phase;
            
                prevPhase = phase;
            }
        }
    }
    
    if (vertical)
    {
        // Unwrap for vertical lines
        for (int j = 0; j < ioPhases->size(); j++)
        {
            WDL_TypedBuf<double> &phases = (*ioPhases)[j];
            
            double prevPhase = phases.Get()[0];
            while(prevPhase < 0.0)
            {
                prevPhase += 2.0*M_PI;
            }
            
            double incr = 0.0;
            for (int i = 0; i < mHeight; i++)
            {
                double phase = phases.Get()[i];
                
                if (phase < prevPhase)
                    phase += incr;
                
                while(phase < prevPhase)
                {
                    phase += 2.0*M_PI;
                    
                    incr += 2.0*M_PI;
                }
                
                phases.Get()[i] = phase;
                
                prevPhase = phase;
            }
        }
    }
}

void
BLSpectrogram::UnwrapPhases3(const deque<WDL_TypedBuf<double> > &inPhases,
                             vector<WDL_TypedBuf<double> > *outPhases,
                             bool horizontal, bool vertical)
{
    if (inPhases.empty())
        return;
    
#if 0
    // Convert deque to vec
    vector<WDL_TypedBuf<double> > phasesVec;
    for (int i = 0; i < ioPhases->size(); i++)
    {
        WDL_TypedBuf<double> &line = (*ioPhases)[i];
        phasesVec.push_back(line);
    }
#endif
    
    outPhases->clear();
    
    // Convert deque to vec
    for (int i = 0; i < inPhases.size(); i++)
    {
        WDL_TypedBuf<double> line = inPhases[i];
        outPhases->push_back(line);
    }
    
    
    if (horizontal && vertical)
        mPhasesMultiplier2 = 0.015;
    else
        mPhasesMultiplier2 = 1.0;
    
    if (horizontal)
    {
        // Unwrap for horizontal lines
        for (int i = 0; i < mHeight; i++)
        {
            WDL_TypedBuf<double> &phases0 = (*outPhases)[0];
            double prevPhase = phases0.Get()[i];
            
            while(prevPhase < 0.0)
            {
                prevPhase += 2.0*M_PI;
            }
            
            double incr = 0.0;
            for (int j = 0; j < outPhases->size(); j++)
            {
                WDL_TypedBuf<double> &phases = (*outPhases)[j];
                
                double phase = phases.Get()[i];
                
                if (phase < prevPhase)
                    phase += incr;
                
                while(phase < prevPhase)
                {
                    phase += 2.0*M_PI;
                    incr += 2.0*M_PI;
                }
                
                phases.Get()[i] = phase;
                
                prevPhase = phase;
            }
        }
    }
    
    if (vertical)
    {
        // Unwrap for vertical lines
        for (int j = 0; j < outPhases->size(); j++)
        {
            WDL_TypedBuf<double> &phases = (*outPhases)[j];
            
            double prevPhase = phases.Get()[0];
            while(prevPhase < 0.0)
            {
                prevPhase += 2.0*M_PI;
            }
            
            double incr = 0.0;
            for (int i = 0; i < mHeight; i++)
            {
                double phase = phases.Get()[i];
                
                if (phase < prevPhase)
                    phase += incr;
                
                while(phase < prevPhase)
                {
                    phase += 2.0*M_PI;
                    
                    incr += 2.0*M_PI;
                }
                
                phases.Get()[i] = phase;
                
                prevPhase = phase;
            }
        }
    }
    
#if 0
    // Convert vec to deque
    ioPhases->clear();
    for (int i = 0; i < phasesVec.size(); i++)
    {
        WDL_TypedBuf<double> &line = phasesVec[i];
        ioPhases->push_back(line);
    }
#endif
}

void
BLSpectrogram::SavePPM(const char *filename, int maxValue)
{
    if (mMagns.empty())
        return;
    
    //
    //UnwrapPhases();
    
    char fullFilename[MAX_PATH];
    sprintf(fullFilename, "/Volumes/HDD/Share/BlueLabAudio-Debug/%s", filename);
    
    FILE *file = fopen(fullFilename, "w");
    
    // Header
    fprintf(file, "P3\n");
    fprintf(file, "%ld %d\n", mMagns.size(), mMagns[0].GetSize());
    fprintf(file, "%d\n", maxValue);
    
    // Data
    for (int i = mHeight - 1; i >= 0 ; i--)
    {
        for (int j = 0; j < mMagns.size(); j++)
        {
            const WDL_TypedBuf<double> &magns = mMagns[j];
            const WDL_TypedBuf<double> &phases = mPhases[j];
            
            double magnValue = magns.Get()[i];
            
            // Increase
            magnValue = sqrt(magnValue);
            magnValue = sqrt(magnValue);
            
            double magnColor = magnValue*mMagnsMultiplier*(double)maxValue;
            if (magnColor > maxValue)
                magnColor = maxValue;
            
            double phaseValue = phases.Get()[i];
            
            double phaseColor = phaseValue*mPhasesMultiplier*(double)maxValue;
            if (phaseColor > maxValue)
                phaseColor = maxValue;
            
            fprintf(file, "%d %d 0\n", (int)magnColor, (int)phaseColor);
        }
        
        fprintf(file, "\n");
    }
    fclose(file);
}

BLSpectrogram *
BLSpectrogram::ImageToSpectrogram(PPMFile::PPMImage *image, bool is16Bits,
                                  double magnsMultiplier, double phasesMultiplier)
{
    double ppmMultiplier = is16Bits ? 65535.0 : 255.0;
    
    BLSpectrogram *result = new BLSpectrogram(image->h);
    result->SetMultipliers(magnsMultiplier, phasesMultiplier);
    
    for (int i = 0; i < image->w; i++)
    {
        WDL_TypedBuf<double> magns;
        magns.Resize(image->h);
        
        WDL_TypedBuf<double> phases;
        phases.Resize(image->h);
        
        for (int j = 0; j < image->h; j++)
        {
            double magnColor;
            double phaseColor;
            
            if (!is16Bits)
            {
                PPMFile::PPMPixel pix = image->data[i + (image->h - j - 1)*image->w];
                magnColor = pix.red;
                phaseColor = pix.green;
            }
            else
            {
                PPMFile::PPMPixel16 pix = image->data16[i + (image->h - j - 1)*image->w];
                magnColor = pix.red;
                phaseColor = pix.green;
            }
            
            // Magn
            double magnValue = magnColor/(magnsMultiplier*ppmMultiplier);
            
            magnValue = magnValue*magnValue;
            magnValue = magnValue*magnValue;
            
            magns.Get()[j] = magnValue;
            
            // Phase
            double phaseValue = phaseColor/(phasesMultiplier*ppmMultiplier);
            
            phases.Get()[j] = phaseValue;
        }
        
        result->AddLine(magns, phases);
    }
    
    free(image);
    
    return result;
}

BLSpectrogram *
BLSpectrogram::ImagesToSpectrogram(PPMFile::PPMImage *magnsImage,
                                   PPMFile::PPMImage *phasesImage)
{
    double ppmMultiplier = 65535.0;
    
    BLSpectrogram *result = new BLSpectrogram(magnsImage->h);
    
    for (int i = 0; i < magnsImage->w; i++)
    {
        WDL_TypedBuf<double> magns;
        magns.Resize(magnsImage->h);
        
        WDL_TypedBuf<double> phases;
        phases.Resize(magnsImage->h);
        
        for (int j = 0; j < magnsImage->h; j++)
        {
            float magnValue;
            float phaseValue;
            
            // Magn
            PPMFile::PPMPixel16 magnPix =
                    magnsImage->data16[i + (magnsImage->h - j - 1)*magnsImage->w];
            ((short *)&magnValue)[0] = magnPix.red;
            ((short *)&magnValue)[1] = magnPix.green;
            
            // Phase
            PPMFile::PPMPixel16 phasePix =
                    phasesImage->data16[i + (phasesImage->h - j - 1)*phasesImage->w];
            ((short *)&phaseValue)[0] = phasePix.red;
            ((short *)&phaseValue)[1] = phasePix.green;
            
            magns.Get()[j] = magnValue;
            phases.Get()[j] = phaseValue;
        }
        
        result->AddLine(magns, phases);
    }
    
    free(magnsImage);
    free(phasesImage);
    
    return result;
}

double
BLSpectrogram::MapToPi(double val)
{
    /* Map delta phase into +/- Pi interval */
    val =  fmod(val, 2.0*M_PI);
    if (val <= -M_PI)
        val += 2.0*M_PI;
    if (val > M_PI)
        val -= 2.0*M_PI;
    
    return val;
}




