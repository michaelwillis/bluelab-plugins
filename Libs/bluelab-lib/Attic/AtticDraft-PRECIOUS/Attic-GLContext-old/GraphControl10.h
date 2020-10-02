//
//  Graph.h
//  Transient
//
//  Created by Apple m'a Tuer on 03/09/17.
//
//

#ifndef Transient_Graph7_h
#define Transient_Graph7_h

#include "../../WDL/IPlug/IControl.h"

#include <string>
#include <vector>
using namespace std;

#include <GraphCurve4.h>
#include <ParamSmoother.h>

#define PROFILE_GRAPH 0

#if PROFILE_GRAPH
#include <BlaTimer.h>
#include <Debug.h>
#endif

// MODIF FOR Ghost
//#ifdef WIN32
//#define SWAP_COLOR(__RGBA__)
//#endif

#ifdef WIN32
// Convert from RGBA to ABGR
#define SWAP_COLOR(__RGBA__) \
{ int tmp[4] = { __RGBA__[0], __RGBA__[1], __RGBA__[2], __RGBA__[3] }; \
__RGBA__[0] = tmp[2]; \
__RGBA__[1] = tmp[1]; \
__RGBA__[2] = tmp[0]; \
__RGBA__[3] = tmp[3]; }
#endif

#ifdef __APPLE__
// Convert from RGBA to ABGR
#define SWAP_COLOR(__RGBA__) \
{ int tmp[4] = { __RGBA__[0], __RGBA__[1], __RGBA__[2], __RGBA__[3] }; \
__RGBA__[0] = tmp[2]; \
__RGBA__[1] = tmp[1]; \
__RGBA__[2] = tmp[0]; \
__RGBA__[3] = tmp[3]; }
#endif

// Font size
#define FONT_SIZE 14.0

#define GRAPH_VALUE_UNDEFINED 1e16

#define GRAPHCONTROL_PIXEL_DENSITY 2.0

// Graph class (Use NanoVG internally).
// Bug correction: vertical axis and dB

// Direct OpenGL render, no more GLReadPixels (8 x faster !)
// Be sure to set OPENGL_YOULEAN_PATCH to 1, in Lice and IGraphicsMac.mm 

// Use with GraphCurve2
// ... then use with GraphCurve3

// From GraphControl5
// Precompute all possible before while adding the points
// (instead of computing at each draw)
// (useful for large number of points, e.g 500000 for Impulse with 15s)
//
// GraphControl7 : added Spectrograms
//
// GraphControl8 : for BLSpectrogram3
//
// GraphControl9 : for BLSpectrogram3
//
// GraphControl10 : for SpectrogramDisplay
//
struct NVGcontext;
struct NVGLUframebuffer;

class GLContext2;

class BLSpectrogram3;
class SpectrogramDisplay;

// Added this test to avoid redraw everything each time
// NOTE: added for StereoWidth
//
// With this flag, everything is redrawn only if the lements changes
// (not at each call to DrawGraph)
//
// => real optimization when nothing changes !
//
#define DIRTY_OPTIM 1

// Class to add special drawing, depending on the plugins
class GraphCustomDrawer
{
public:
    GraphCustomDrawer() {}
    
    virtual ~GraphCustomDrawer() {}
    
    // Implement one of the two methods, depending on when
    // you whant to draw
    
    // Draw before everything
    virtual void PreDraw(NVGcontext *vg, int width, int height) {}
    
    // Draw after everything
    virtual void PostDraw(NVGcontext *vg, int width, int height) {}
};

class GraphCustomControl
{
public:
    GraphCustomControl() {}
    
    virtual ~GraphCustomControl() {}
    
    virtual void OnMouseDown(int x, int y, IMouseMod* pMod) {}
    virtual void OnMouseUp(int x, int y, IMouseMod* pMod) {}
    virtual void OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod) {}
    virtual bool OnMouseDblClick(int x, int y, IMouseMod* pMod) { return false; }
    virtual void OnMouseWheel(int x, int y, IMouseMod* pMod, double d) {};
    virtual bool OnKeyDown(int x, int y, int key, IMouseMod* pMod) { return false; }
    
    virtual void OnMouseOver(int x, int y, IMouseMod* pMod) {}
    virtual void OnMouseOut() {}
};

// WARNING: Since GraphControl4 width must be a multiple of 4 !
class GraphControl10 : public IControl
{
public:
    GraphControl10(IPlugBase *pPlug, IRECT p, int paramIdx,
                 int numCurves, int numCurveValues, const char *fontPath);
    
    virtual ~GraphControl10();
    
    void GetSize(int *width, int *height);
    void Resize(int width, int height);
    
    void SetBounds(double x0, double y0, double x1, double y1);
    
    // Not tested
    void Resize(int numCurveValues);
    
    int GetNumCurveValues();
    
#if 0 // Commented for StereoWidthProcess::DebugDrawer
    bool IsDirty();
  
    void SetDirty(bool flag);
#endif
    
//#if DIRTY_OPTIM
    // Used to force redraw when something changed that is not pcurve point change
    void SetMyDirty(bool flag);
//#endif
    
    // Does nothing
    bool Draw(IGraphics* pGraphics) { return true; };
    
    const LICE_IBitmap *DrawGL();
    
	//void OnIdle();

    // Set a separator line at the bottom
    void SetSeparatorY0(double lineWidth, int color[4]);
    
    void AddHAxis(char *data[][2], int numData,
                  int axisColor[4], int axisLabelColor[4],
                  double offsetY = 0.0,
                  int axisOverlayColor[4] = NULL);
    
    void ReplaceHAxis(char *data[][2], int numData);
    
    void AddVAxis(char *data[][2], int numData,
                  int axisColor[4], int axisLabelColor[4],
                  double offset = 0.0, double offsetX = 0.0, // Tmp offset hack
                  int axisOverlayColor[4] = NULL);
    
    void AddVAxis(char *data[][2], int numData,
                  int axisColor[4], int axisLabelColor[4],
                  bool dbFlag, double minY, double maxY,
                  double offset = 0.0, // Tmp offset hack
                  int axisOverlayColor[4] = NULL);
    
    void SetXScale(bool dBFlag, double minX = 0.0, double maxX = 1.0);
    
    void SetAutoAdjust(bool flag, double smoothCoeff);
    
    void SetYScaleFactor(double factor);
    
    void SetClearColor(int r, int g, int b, int a);
    
    // Curves
    void ResetCurve(int curveNum, double resetVal);
    
    void SetCurveDescription(int curveNum, const char *description, int descrColor[4]);
    
    // Bounds
    void SetCurveLimitToBounds(int curveNum, bool flag);
    
    // Curves style
    void SetCurveYScale(int curveNum, bool flag,
                        double minY = -120.0, double maxY = 0.0);
    
    void SetCurveColor(int curveNum, int r, int g, int b);
    
    void SetCurveAlpha(int curveNum, double alpha);
    
    void SetCurveLineWidth(int curveNum, double lineWidth);
    
    void SetCurveSmooth(int curveNum, bool flag);
    
    void SetCurveFill(int curveNum, bool flag);
    
    // Down
    // Fill under the curve
    void SetCurveFillAlpha(int curveNum, double alpha);
    
    // Up
    // Fill over the curve
    void SetCurveFillAlphaUp(int curveNum, double alpha);
    
    // Points
    void SetCurvePointSize(int curveNum, double pointSize);
    
    void SetCurveXScale(int curveNum, bool dBFlag,
                        double minX = 0.0, double maxX = 1.0);

    void SetCurvePointStyle(int curveNum, bool flag, bool pointsAsLines);
    
    void SetCurveValuesPoint(int curveNum,
                             const WDL_TypedBuf<double> &xValues,
                             const WDL_TypedBuf<double> &yValues);
    
    void SetCurveColorWeight(int curveNum, const WDL_TypedBuf<double> &colorWeights);

    
    void SetCurveValuesPointWeight(int curveNum,
                                   const WDL_TypedBuf<double> &xValues,
                                   const WDL_TypedBuf<double> &yValues,
                                   const WDL_TypedBuf<double> &weights);

    
    // Curves data
    void CurveFillAllValues(int curveNum, double val);
    
    void SetCurveValues(int curveNum, const WDL_TypedBuf<double> *values);
    
    void SetCurveValues2(int curveNum, const WDL_TypedBuf<double> *values);
    
    void SetCurveValues3(int curveNum, const WDL_TypedBuf<double> *values);
    
    // Use simple decimation
    void SetCurveValuesDecimateSimple(int curveNum, const WDL_TypedBuf<double> *values);
    
    // Optimized version
    // Remove points if there is more points density than graph pixel density
    void SetCurveValuesDecimate(int curveNum, const WDL_TypedBuf<double> *values,
                                bool isWaveSignal = false);
    
    // Fixed for positive and negative maxima for sample/wave type values
    void SetCurveValuesDecimate2(int curveNum, const WDL_TypedBuf<double> *values,
                                 bool isWaveSignal = false);
    
    // Fixed for some flat sections at 0
    void SetCurveValuesDecimate3(int curveNum, const WDL_TypedBuf<double> *values,
                                 bool isWaveSignal = false);
    
    // T is x normalized
    void SetCurveValue(int curveNum, double t, double val);
    
    void SetCurveSingleValueH(int curveNum, double val);
    
    void SetCurveSingleValueV(int curveNum, double val);
    
    void PushCurveValue(int curveNum, double val);
    
    void SetCurveSingleValueH(int curveNum, bool flag);
    
    void SetCurveSingleValueV(int curveNum, bool flag);


#if 0
    void SetCurveValuePoint(int curveNum, double x, double y);
#endif
    
    static void DrawText(NVGcontext *vg, double x, double y, double fontSize, const char *text,
                         int color[4], int halign, int valign);
    
    void SetSpectrogram(BLSpectrogram3 *spectro,
                        double left, double top, double right, double bottom);
    
    SpectrogramDisplay *GetSpectrogramDisplay();
    
    void UpdateSpectrogram(bool updateData, bool updateFullData);

    
    // Custom drawers
    void AddCustomDrawer(GraphCustomDrawer *customDrawer);
    
    void CustomDrawersPreDraw();
    
    void CustomDrawersPostDraw();
    
    void DrawSeparatorY0();
    
    // Custom control
    void AddCustomControl(GraphCustomControl *customControl);
    
    void OnMouseDown(int x, int y, IMouseMod* pMod);
    void OnMouseUp(int x, int y, IMouseMod* pMod);
    void OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod);
    bool OnMouseDblClick(int x, int y, IMouseMod* pMod);
    void OnMouseWheel(int x, int y, IMouseMod* pMod, double d);
    bool OnKeyDown(int x, int y, int key, IMouseMod* pMod);
    
    void OnMouseOver(int x, int y, IMouseMod* pMod);
    void OnMouseOut();
    
    // Must be called at the beginning of the plugin
    void InitGL();
    
    // Must be called before destroying the plugin
    void ExitGL();
    
    void DBG_PrintCoords(int x, int y);
    
    void SetBackgroundImage(LICE_IBitmap *bmp);
    
protected:
    // Axis
    typedef struct
    {
        double mT;
        string mText;
    } GraphAxisData;
    
    typedef struct
    {
        vector<GraphAxisData> mValues;
        
        int mColor[4];
        int mLabelColor[4];
        
        // Hack
        double mOffset;
        
        // To be able to display the axis on the right
        double mOffsetX;
        
        double mOffsetY;
        
        bool mOverlay;
        int mOverlayColor[4];
    } GraphAxis;
    
    void DisplayCurveDescriptions();
    
    void AddAxis(GraphAxis *axis, char *data[][2], int numData,
                 int axisColor[4], int axisLabelColor[4],
                 double mindB, double maxdB, int axisOverlayColor[4] = NULL);
    
    void DrawAxis(GraphAxis *axis, bool horizontal, bool lineLabelFlag);
    
    // If flag is true, we will draw only the lines
    // If flag is false, we will draw only the labels
    // Usefull because the curves must be drawn over the lines but under the labels
    void DrawAxis(bool lineLabelFlag);
    
    void DrawCurves();
    
    void DrawLineCurve(GraphCurve4 *curve);
    
    // Fill and stroke at the same time
    void DrawFillCurve(GraphCurve4 *curve);
    
    // Single value Horizontal
    void DrawLineCurveSVH(GraphCurve4 *curve);
    
    void DrawFillCurveSVH(GraphCurve4 *curve);
    
    // Single value Vertical
    void DrawLineCurveSVV(GraphCurve4 *curve);
    
    void DrawFillCurveSVV(GraphCurve4 *curve);
    
    // Draw points
    void DrawPointCurve(GraphCurve4 *curve);
    
    // TEMPORARY
    // Draw lines, from a "middle" to each point
    void DrawPointCurveLines(GraphCurve4 *curve);
    
    void AutoAdjust();
    
    static double MillisToPoints(long long int elapsed, int sampleRate, int numSamplesPoint);
    
    // Text
    void InitFont(const char *fontPath);
    
    void DrawText(double x, double y, double fontSize, const char *text,
                  int color[4], int halign, int valign);
    
    // OpenGL
    
    // Separated function with all the OpenGL calls
    // May be useful if we want to render with a specific thread
    void DrawGraph();
    
    double ConvertX(GraphCurve4 *curve, double val, double width);
    
    double ConvertY(GraphCurve4 *curve, double val, double height);
    
    void SetCurveDrawStyle(GraphCurve4 *curve);
    
    void SetCurveDrawStyleWeight(GraphCurve4 *curve, double weight);
    
    // Bounds
    double ConvertToBoundsX(double t);
    double ConvertToBoundsY(double t);
    
    void UpdateBackgroundImage();

    void DrawBackgroundImage();
    
    void InitNanoVg();
    void ExitNanoVg();
    
    double mBounds[4];
    
    int mNumCurveValues;
    int mNumCurves;
    
    vector<GraphCurve4 *> mCurves;
    
    // Auto adjust
    bool mAutoAdjustFlag;
    ParamSmoother mAutoAdjustParamSmoother;
    double mAutoAdjustFactor;
    
    double mYScaleFactor;
    
    CurveColor mClearColor;
    
    // NanoVG
    NVGcontext *mVg;
    
#if 0 // Commented for StereoWidthProcess::DebugDrawer
    // Was strange, overloaded the member variable of IControl
    bool mDirty;
#endif

#if DIRTY_OPTIM
    bool mMyDirty;
#endif
    
    bool mSeparatorY0;
    double mSepY0LineWidth;
    int mSepY0Color[4];
    
    // X dB scale
    bool mXdBScale;
    double mMinX;
    double mMaxX;

    GraphAxis *mHAxis;
    GraphAxis *mVAxis;
    
    WDL_String mFontPath;
    
    SpectrogramDisplay *mSpectrogramDisplay;
    
    // Custom drawers
    vector<GraphCustomDrawer *> mCustomDrawers;
    
    // Custom control
    vector<GraphCustomControl *> mCustomControls;
    
    // Background image
    LICE_IBitmap *mBgImage;
    int mNvgBackgroundImage;
    
private:
    bool mGLInitialized;
    
	bool mFontInitialized;
    
    LICE_IBitmap *mLiceFb;
    
private:
    bool CreateCGLContext();
    void DestroyCGLContext();
    bool BindCGLContext();
    void UnBindCGLContext();

    
    void *mGLContext;
    
#if PROFILE_GRAPH
    BlaTimer mDebugTimer;
    int mDebugCount;
#endif
};

#endif
