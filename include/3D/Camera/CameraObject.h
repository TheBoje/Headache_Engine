#ifndef CAMERA_OBJECT__H
#define CAMERA_OBJECT_H

#include <string>

#include "ofEasyCam.h"
#include "Object3D.h"

namespace ift3100 {
    /**
     * @brief Camera used for rendering that can be put in 
     * the hierarchy of 3Dmodels as 3d ojects
     * 
     */
    class CameraObject : public ofEasyCam, public Object3D {
        
    public:
        CameraObject(std::string name) : Object3D(name) {}
    };
}

#endif 