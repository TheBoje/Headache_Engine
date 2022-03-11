#include "CameraInterface.h"

using namespace ift3100;

void CameraInterface::setup() {
    _axes_cam_enable = false;
    cm.disableAxesCameras();
}

void CameraInterface::drawUI() {
    if(ImGui::Checkbox("Activate axes cameras", &_axes_cam_enable)) {
        if(_axes_cam_enable)
            cm.enableAxesCameras();
        else
            cm.disableAxesCameras();
    }
}