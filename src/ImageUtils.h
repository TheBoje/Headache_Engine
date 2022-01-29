#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include "ofMain.h"
#include <string>

namespace ift3100 {

    class ImageUtils {
    public:

        static ofImage * importImage(const std::string& path);
        static void exportImage(const std::string& name);
        static unsigned int * computeHist(const ofImage& image);
        static unsigned int ** computeHistRGB(const ofImage& image);
    };
}

#endif // !IMAGEUTILS_H