#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "ofEasyCam.h"
#include "Constant.h"

#include <functional>

namespace ift3100
{   
    class Application;
    /**
     * @brief Manage build cameras
     * 1 main (perspective by default)
     * x camera
     * y camera
     * z camera
     * all 3 ortho by default
     */
    class CameraManager {
        ofEasyCam   _main_camera;
        ofEasyCam   _axes_cameras[NB_AXES_CAM];

        bool        _axes_cameras_enabled;


    public:
        static constexpr float  OFFSET = 10.0f;
        static constexpr float  STROKE_WIDTH = 1.0f;
        static constexpr float  OFFSET_FOCUS = 30.0f;
        
        ofRectangle viewports[NB_AXES_CAM + 1];

        CameraManager();

        void setup();
        void update();
        void beginCamera(std::size_t index);
        void endCamera(std::size_t index);
        void onWindowResize(int width, int height);
        void focus(const ofVec3f& position);


        void enableAxesCameras();
        void disableAxesCameras();

    };
} // namespace ift3100


#endif