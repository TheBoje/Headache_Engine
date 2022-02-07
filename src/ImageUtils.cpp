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
     * @return a 3 lengthen array of 256 lengthen unsigned int vectors for rbg colors
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

    /**
     * Convert an image using openCV library.
     * 
     * @param image 
     * @param code convertion code, see https://docs.opencv.org/3.1.0/de/d25/imgproc_color_conversions.html
     * @return image converted following `code` type.
     */
    ofImage ImageUtils::convert(const ofImage& image, int code) {
        cv::Mat tmp = ofxCv::toCv(image); // Convert from ofImage to cv::Mat (openCV image object)
        cv::Mat colored;
        cv::cvtColor(tmp, colored, code); // Convert color 
        ofImage res;
        ofxCv::toOf(colored, res);        // Convert from cv::Mat to ofImage 
        return res;
    }
}