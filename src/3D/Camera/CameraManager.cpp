#include "CameraManager.h"
#include "ofMain.h"
#include "Logger.h"

using namespace ift3100;

static const ofVec3f OFFSET_DATA[] = {
            ofVec3f(CameraManager::OFFSET_FOCUS, 0, 0),
            ofVec3f(0, CameraManager::OFFSET_FOCUS, 0),
            ofVec3f(0, 0, CameraManager::OFFSET_FOCUS),
        };

CameraManager::CameraManager() : _axes_cameras_enabled(true) 
{}

void CameraManager::setup() {

    float width = ofGetWidth();
    float height = ofGetHeight();
    int i;
    for(i = 0; i < CameraManager::NB_AXES_CAM; i++) {
        _axes_cameras[i].enableOrtho();
        
        viewports[i].x = ((CameraManager::OFFSET + width)/2) * (i % 2);
        viewports[i].y = ((CameraManager::OFFSET + height)/2) * (i / 2);
        viewports[i].width = (width / 2) - CameraManager::OFFSET;
        viewports[i].height = (height / 2) - CameraManager::OFFSET;
        IFT_LOG << "width : " << width << " height : " << height << " i : " << i << " viewport : " << viewports[i];
    }
    
    viewports[i].x = ((CameraManager::OFFSET + width)/2)* (i % 2);
    viewports[i].y = ((CameraManager::OFFSET + height)/2) * (i / 2);
    viewports[i].width = (width / 2) - CameraManager::OFFSET;
    viewports[i].height = (height / 2) - CameraManager::OFFSET;
    IFT_LOG << "width : " << width << " height : " << height << " i : " << i << " viewport : " << viewports[i];

    _axes_cameras[0].rotateDeg(90, ofVec3f(0, 1, 0));
    _axes_cameras[1].rotateDeg(90, ofVec3f(1, 0, 0));
    _axes_cameras[2].rotateDeg(90, ofVec3f(0, 0, 1));
}

void CameraManager::update() {

}

void CameraManager::onWindowResize(int width, int height) {
    int i;
    for(i = 0; i < CameraManager::NB_AXES_CAM; i++) {
        _axes_cameras[i].enableOrtho();
        
        viewports[i].x = ((CameraManager::OFFSET + width)/2) * (i % 2);
        viewports[i].y = ((CameraManager::OFFSET + height)/2) * (i / 2);
        viewports[i].width = (width / 2) - CameraManager::OFFSET;
        viewports[i].height = (height / 2) - CameraManager::OFFSET;
    }
    
    viewports[i].x = ((CameraManager::OFFSET + width)/2)* (i % 2);
    viewports[i].y = ((CameraManager::OFFSET + height)/2) * (i / 2);
    viewports[i].width = (width / 2) - CameraManager::OFFSET;
    viewports[i].height = (height / 2) - CameraManager::OFFSET;
}

void CameraManager::focus(const ofVec3f& position) {
    _main_camera.setPosition(position);

    for(int i = 0; i < CameraManager::NB_AXES_CAM; i++) 
        _axes_cameras[i].setPosition(position + OFFSET_DATA[i]);
}

void CameraManager::beginMainCamera() {
    _main_camera.begin(viewports[CameraManager::NB_AXES_CAM]);
}

void CameraManager::endMainCamera() {
    _main_camera.end();
}

void CameraManager::beginAxeXCamera() {
    if(!_axes_cameras_enabled) return;

    _axes_cameras[0].begin(viewports[0]);
}

void CameraManager::endAxeXCamera() {
    _axes_cameras[0].end();
}

void CameraManager::beginAxeYCamera() {
    if(!_axes_cameras_enabled) return;

    _axes_cameras[1].begin(viewports[1]);
}

void CameraManager::endAxeYCamera() {
    _axes_cameras[0].end();
}
void CameraManager::beginAxeZCamera() {
    if(!_axes_cameras_enabled) return;

    _axes_cameras[2].begin(viewports[2]);
}

void CameraManager::endAxeZCamera() {
    _axes_cameras[2].end();
}

void CameraManager::enableAxesCameras() {
    _axes_cameras_enabled = true;
}

void CameraManager::disableAxesCameras() {
    _axes_cameras_enabled = false;
}

