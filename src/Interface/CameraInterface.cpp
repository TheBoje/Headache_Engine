#include "CameraInterface.h"

using namespace ift3100;

void CameraInterface::setup() {
    axes_cam_enable = false;
    cm.setup();
    cm.disableAxesCameras();
}

void CameraInterface::drawUI() {
    if(ImGui::Checkbox("Activate axes cameras", &axes_cam_enable)) {
        if(axes_cam_enable)
            cm.enableAxesCameras();
        else
            cm.disableAxesCameras();
    }
}