#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <string>
#include "ofMain.h"
#include "ofxCv.h"
#include "Logger.h"

namespace ift3100 {
    class ImageUtils {
    public:
        static ofImage * importImage(const std::string& path);
        static void exportImage(const std::string& name);
        static unsigned int ** computeHistRGB(const ofImage& image);
        static cv::Mat convert(const ofImage& image, int code);
    };
}

#endif // !IMAGEUTILS_H