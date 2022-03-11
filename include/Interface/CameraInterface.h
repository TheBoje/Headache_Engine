#ifndef CAMERA_INTERFACE_H
#define CAMERA_INTERFACE_H

#include "CameraManager.h"
#include "ofxImGui.h"

namespace ift3100
{
    class CameraInterface {
    public:
        bool axes_cam_enable;
        CameraManager cm;

        void setup();
        void drawUI();
    };
} // namespace ift3100


#endif