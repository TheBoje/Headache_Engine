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
}