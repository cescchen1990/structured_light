#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <deque>

#ifndef THREESTEPPHASESHIFT_H
#define THREESTEPPHASESHIFT_H

using namespace std;

struct UnwrapPath {
    int x;
    int y;
    float phi; // last phase 
    //int r;   // phase distance

    UnwrapPath(int x, int y, float phi):
        x(x),y(y),phi(phi)
    {}

    //bool operator<(const UnwrapPath & p) {return r<p.r;}

};

class ThreeStepPhaseShift {

public:

    ThreeStepPhaseShift(
          IplImage *imgPhase1
        , IplImage *imgPhase2
        , IplImage *imgPhase3
        );

    ~ThreeStepPhaseShift();
 
    void phaseWrap();
    void phaseUnwrap();
    
    void  setZscale(float _zscale) { zscale = _zscale; }
    void  setZskew(float _zskew) { zskew = _zskew; }
    void  setNoiseThreshold(float _threshold) { noiseThreshold = _threshold;}
    float getZscale() { return zscale; }
    float getZskew() { return zskew; }
    float getNoiseThreshold() { return noiseThreshold; }
    float* getDepth() { return depth; }

    IplImage *getWrappedPhase()  { return imgWrappedPhase; };
    IplImage *getUnwrappedPhase()  { return imgUnwrappedPhase; };
    IplImage *getColorImage()  { return imgColor; };

    IplImage* imgPhase1Gray;

protected:

    // unwrap at x,y
    void phaseUnwrap(int x, int y, float phi );
    void makeDepth ();

    // inline helper functions
    float max_phase(float v1, float v2, float v3) {
        float max;
        max = v1>v2 ? v1 : v2;
        max = max>v3 ? max : v3;
        return max;
    }

    float min_phase(float v1, float v2, float v3) {
        float max = v1<v2 ? v1 : v2;
        max = max<v2 ? max : v3;
        return max;
    }

    /* use mean as luminance of an rgb triple */
    float luminance(uchar *color) {return (color[0]+color[1]+color[2])/(3.f*255);}

    void copy_channels(uchar *dest, uchar *src) {
        for(int i=0;i<3;i++)
            *(dest+i) = *(src+i);
    } 

private:

    IplImage* imgPhase1;
    IplImage* imgPhase2;
    IplImage* imgPhase3;
    IplImage* imgColor;  // reconstructed color image 
    IplImage* imgWrappedPhase;
    IplImage* imgUnwrappedPhase;

    // some helper matrices to track phase quality and
    // processing state (each from the same dimension as the input image)
    bool  *mask;
    bool  *process;
    float *distance;
    float *depth;
    float noiseThreshold;
    float zscale;
    float zskew;

    int width;
    int height;
    int step;   // for single channel images
    int step3; // for 3 channel images

    deque<UnwrapPath> procQueue;
};

#endif
