#include "Renderer3D.h"
#include "Application.h"

using namespace ift3100;

Renderer3D::Renderer3D(Application& _application) : application(_application)
{}

void Renderer3D::setup() {
    ofEnableSmoothing();
    ofEnableDepthTest();
    cameraManager.setup();
    
    // TODO: Temporaire
    hierarchy.setRoot(std::make_shared<Object3D>("root"));
    ofBoxPrimitive box;
    box.setPosition(0, 15, 0);
    hierarchy.addChild(std::make_shared<Object3D>("box", box));
    light.setAmbientColor(ofColor(0, 60, 130));
    light.setPosition(ofVec3f(150, 150, 150));
    // ----

    IFT_LOG << "done";
}

void Renderer3D::update() {
    cameraManager.update();
}

void Renderer3D::draw() {
    ofNoFill();
    ofSetColor(255);

    // Draw axis camera if enabled
    if (cameraManager.axesCamerasEnabled()) {
        for (int i = 0; i < NB_AXES_CAM; i++) {
            cameraManager.beginCamera(i);
            hierarchy.mapChildren([](std::shared_ptr<Object3D> o){
                ObjectType t = o->getType();
                switch (t)
                {
                case ObjectType::Primitive :
                    o->getNode().draw();
                    break;

                default:
                    break;
                }   
            });
            cameraManager.endCamera(i);
        }
    }
    // Draw main camera
    cameraManager.beginCamera(3);
    hierarchy.mapChildren([](std::shared_ptr<Object3D> o){
        ObjectType t = o->getType();
        switch (t)
        {
        case ObjectType::Primitive :
            o->getNode().draw();
            break;

        default:
            break;
        }   
    });
    cameraManager.endCamera(3);
}

void Renderer3D::setMouseInput(bool enable) {
    cameraManager.setMouseInput(enable);
}

void Renderer3D::toggleAxesCameras(bool enabled) {
    cameraManager.toggleAxesCameras(enabled);
}

void Renderer3D::setMainCameraOrtho(bool ortho) {
    cameraManager.setMainCameraOrtho(ortho);
}

