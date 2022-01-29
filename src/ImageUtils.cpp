#include "ImageUtils.h"

namespace ift3100 {
    ofImage * ImageUtils::importImage(const std::string& path) {
        ofLog() << "<utils:import> from " << path;
        return new ofImage(path);
    }

    void ImageUtils::exportImage(const std::string& name) {
        ofLog() << "<utils:export> " << name
                << " (" << ofGetWindowWidth() 
                << ", " << ofGetWindowHeight()  << ")";
        ofImage res;
        res.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
        res.save(name);
    }

    /**
     * Compute the histogram of the three colors of the image.
     * Allocate a 2 dimensional array and give the use the property, it's up to you
     * to delete it.
     *
     * @param image
     * @param space
     * @return a 3 lengthed array of 256 lengthed unsigned int vectors for rbg colors
     */
    unsigned int ** ImageUtils::computeHistRGB(const ofImage &image) {

        int channels = 3;
        unsigned int ** hist = new unsigned int * [channels];

        for(int i = 0; i < channels; i++)
            hist[i] = new unsigned int[256]();

        for(int i = 0; i < (int)image.getWidth(); i++) {
            for(int j = 0; j < (int)image.getHeight(); j++) {
                ofColor color = image.getColor(i, j);
                hist[0][color.r]++;
                hist[1][color.g]++;
                hist[2][color.b]++;
            }
        }

        return hist;
    }
}