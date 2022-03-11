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
    viewports[i].x = (CameraManager::OFFSET / 2) - CameraManager::STROKE_WIDTH + ((width)/2) * (i % 2);
    viewports[i].y = (CameraManager::OFFSET / 2) - CameraManager::STROKE_WIDTH + ((height)/2) * (i / 2);
    viewports[i].width = (width / (i == 0 ? 1 : 2)) - CameraManager::OFFSET - CameraManager::STROKE_WIDTH;
    viewports[i].height = (height / (i == 0 ? 1 : 2)) - CameraManager::OFFSET - CameraManager::STROKE_WIDTH;

    _axes_cameras[0].rotateDeg(90, ofVec3f(0, 1, 0));
    _axes_cameras[1].rotateDeg(90, ofVec3f(1, 0, 0));
    _axes_cameras[2].rotateDeg(90, ofVec3f(0, 0, 1));
}

void CameraManager::update() {

}

void CameraManager::onWindowResize(int width, int height) {
    int i = 0;
    
    if(_axes_cameras_enabled) {
        for(; i < NB_AXES_CAM; i++) {  
            _axes_cameras[i].enableOrtho();        
            viewports[i].x = (CameraManager::OFFSET / 2) - CameraManager::STROKE_WIDTH + ((width)/2) * (i % 2);
            viewports[i].y = (CameraManager::OFFSET / 2) - CameraManager::STROKE_WIDTH + ((height)/2) * (i / 2);
            viewports[i].width = (width / 2) - CameraManager::OFFSET - CameraManager::STROKE_WIDTH;
            viewports[i].height = (height / 2) - CameraManager::OFFSET - CameraManager::STROKE_WIDTH;
        }
    }

    // i = 3 main camera viewport
    viewports[i].x = (CameraManager::OFFSET / 2) - CameraManager::STROKE_WIDTH + ((width)/2) * (i % 2);
    viewports[i].y = (CameraManager::OFFSET / 2) - CameraManager::STROKE_WIDTH + ((height)/2) * (i / 2);
    viewports[i].width = (width / (i == 0 ? 1 : 2)) - CameraManager::OFFSET - CameraManager::STROKE_WIDTH;
    viewports[i].height = (height / (i == 0 ? 1 : 2)) - CameraManager::OFFSET - CameraManager::STROKE_WIDTH;

}

void CameraManager::focus(const ofVec3f& position) {
    _main_camera.setPosition(position);

    for(int i = 0; i < NB_AXES_CAM; i++) 
        _axes_cameras[i].setPosition(position + OFFSET_DATA[i]);
}

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

    int i = 0;
    float width = ofGetWidth();
    float height = ofGetHeight();
    
    if(_axes_cameras_enabled) {
        for(; i < NB_AXES_CAM; i++) {     
            _axes_cameras[i].enableOrtho();     
            viewports[i].x = (CameraManager::OFFSET / 2) - CameraManager::STROKE_WIDTH + ((width)/2) * (i % 2);
            viewports[i].y = (CameraManager::OFFSET / 2) - CameraManager::STROKE_WIDTH + ((height)/2) * (i / 2);
            viewports[i].width = (width / 2) - CameraManager::OFFSET - CameraManager::STROKE_WIDTH;
            viewports[i].height = (height / 2) - CameraManager::OFFSET - CameraManager::STROKE_WIDTH;
        }
    }

    // i = 3 main camera viewport
    viewports[i].x = (CameraManager::OFFSET / 2) - CameraManager::STROKE_WIDTH + ((width)/2) * (i % 2);
    viewports[i].y = (CameraManager::OFFSET / 2) - CameraManager::STROKE_WIDTH + ((height)/2) * (i / 2);
    viewports[i].width = (width / (i == 0 ? 1 : 2)) - CameraManager::OFFSET - CameraManager::STROKE_WIDTH;
    viewports[i].height = (height / (i == 0 ? 1 : 2)) - CameraManager::OFFSET - CameraManager::STROKE_WIDTH;
    IFT_LOG << "enable axes cameras";
}

void CameraManager::disableAxesCameras() {
    _axes_cameras_enabled = false;

    int i = 3;
    float width = ofGetWidth();
    float height = ofGetHeight();

    // i = 3 main camera viewport
    viewports[i].x = (CameraManager::OFFSET / 2) - CameraManager::STROKE_WIDTH;
    viewports[i].y = (CameraManager::OFFSET / 2) - CameraManager::STROKE_WIDTH;
    viewports[i].width = width - CameraManager::OFFSET - CameraManager::STROKE_WIDTH;
    viewports[i].height = height - CameraManager::OFFSET - CameraManager::STROKE_WIDTH;
    IFT_LOG << "disable axes cameras";
}

