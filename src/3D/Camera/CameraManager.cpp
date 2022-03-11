#include "CameraManager.h"
#include "ofMain.h"
#include "Logger.h"

using namespace ift3100;

static const ofVec3f OFFSET_DATA[] = {
            ofVec3f(CameraManager::OFFSET_FOCUS, 0, 0),
            ofVec3f(0, CameraManager::OFFSET_FOCUS, 0),
            ofVec3f(0, 0, CameraManager::OFFSET_FOCUS),
        };

CameraManager::CameraManager() : _axes_cameras_enabled(false) 
{}

/**
 * @brief Compute the height and width of viewports
 * 
 */
void CameraManager::computeViewports() {
    float width = ofGetWidth();
    float height = ofGetHeight();
    
    int i = 0;
    if(_axes_cameras_enabled) {
        for(; i < NB_AXES_CAM; i++) {
            _axes_cameras[i].enableOrtho();
            
            viewports[i].x = (CameraManager::OFFSET / 2) + ((width)/2) * (i % 2);
            viewports[i].y = (CameraManager::OFFSET / 2) + ((height)/2) * (i / 2);
            viewports[i].width = (width / 2) - CameraManager::OFFSET - CameraManager::STROKE_WIDTH;
            viewports[i].height = (height / 2) - CameraManager::OFFSET - CameraManager::STROKE_WIDTH;
        }
    }

    // i = 3 main camera viewport
    viewports[3].x = (CameraManager::OFFSET / 2) - CameraManager::STROKE_WIDTH + ((width)/2) * (i % 2);
    viewports[3].y = (CameraManager::OFFSET / 2) - CameraManager::STROKE_WIDTH + ((height)/2) * (i / 2);
    viewports[3].width = (width / (i == 0 ? 1 : 2)) - CameraManager::OFFSET - CameraManager::STROKE_WIDTH;
    viewports[3 ].height = (height / (i == 0 ? 1 : 2)) - CameraManager::OFFSET - CameraManager::STROKE_WIDTH;
}

void CameraManager::setup() {

    computeViewports();
    focus(ofVec3f(0, 0, 0));
}

void CameraManager::update() {

}

void CameraManager::onWindowResize() {
    computeViewports();
}

/**
 * @brief Focus the camera.s on the position.
 * 
 * @param position 
 */
void CameraManager::focus(const ofVec3f& position) {
    _main_camera.setPosition(position + OFFSET_DATA[0]);
    _main_camera.lookAt(position);

    for(int i = 0; i < NB_AXES_CAM; i++) 
        _axes_cameras[i].setPosition(position + OFFSET_DATA[i]);
    
     for(int i = 0; i < NB_AXES_CAM; i++) {
         _axes_cameras[i].lookAt(ofVec3f(0, 0, 0));
     }
}

/**
 * @brief begin camera state.
 * 0 to 2 -> axes x,y,z cameras
 * 3 -> main camera
 * 
 * @param index 
 */
void CameraManager::beginCamera(std::size_t index) {
    if(index < 0 || index > NB_AXES_CAM + 1) return;

    if(_axes_cameras_enabled && index < 3) {
        ofDrawRectangle(viewports[index]);
        _axes_cameras[index].begin(viewports[index]);
    }
    
    if(index == 3) {
        ofDrawRectangle(viewports[3]);
        _main_camera.begin(viewports[index]);
    }
}

void CameraManager::endCamera(std::size_t index) {
    if(index < 0 || index > NB_AXES_CAM + 1) return;

    if(_axes_cameras_enabled && index < 3) 
        _axes_cameras[index].end();
    else 
        _main_camera.end();
}

void CameraManager::enableAxesCameras() {
    _axes_cameras_enabled = true;
    
    computeViewports();
    
    IFT_LOG << "enable axes cameras";
}

void CameraManager::disableAxesCameras() {
    _axes_cameras_enabled = false;

    computeViewports();
    
    IFT_LOG << "disable axes cameras";
}

