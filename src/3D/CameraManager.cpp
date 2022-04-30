#include "CameraManager.h"
#include "Asserts.h"
#include "Logger.h"
#include "ofMain.h"

namespace ift3100 {
static const ofVec3f OFFSET_DATA[] = {
	ofVec3f(CameraManager::OFFSET_FOCUS, 0, 0),
	ofVec3f(0, CameraManager::OFFSET_FOCUS, 0),
	ofVec3f(0, 0, CameraManager::OFFSET_FOCUS),
};

CameraManager::CameraManager()
	: _axes_cameras_enabled(false) { }

/**
     * @brief Compute the height and width of viewports
     *
     */
void CameraManager::computeViewports() {
	float width = ofGetWidth();
	float height = ofGetHeight();

	int i = 0;
	if (_axes_cameras_enabled) {
		for (; i < NB_AXES_CAM; i++) {
			_axes_cameras[i].enableOrtho();

			viewports[i].x = (CameraManager::OFFSET / 2) + ((width) / 2) * (i % 2);
			viewports[i].y = (CameraManager::OFFSET / 2) + ((height) / 2) * (i / 2);
			viewports[i].width = (width / 2) - CameraManager::OFFSET - CameraManager::STROKE_WIDTH;
			viewports[i].height = (height / 2) - CameraManager::OFFSET - CameraManager::STROKE_WIDTH;
		}
	}

	// i = 3 main camera viewport
	viewports[3].x = (CameraManager::OFFSET / 2) - CameraManager::STROKE_WIDTH + ((width) / 2) * (i % 2);
	viewports[3].y = (CameraManager::OFFSET / 2) - CameraManager::STROKE_WIDTH + ((height) / 2) * (i / 2);
	viewports[3].width = (width / (i == 0 ? 1 : 2)) - CameraManager::OFFSET - CameraManager::STROKE_WIDTH;
	viewports[3].height = (height / (i == 0 ? 1 : 2)) - CameraManager::OFFSET - CameraManager::STROKE_WIDTH;
}

void CameraManager::setup() {
	computeViewports();
	focus(ofVec3f(0, 0, 0));
}

void CameraManager::update() { }

void CameraManager::windowResize() {
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

	for (int i = 0; i < NB_AXES_CAM; i++)
		_axes_cameras[i].setPosition(position + OFFSET_DATA[i]);

	for (int i = 0; i < NB_AXES_CAM; i++) {
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
	if (index < 0 || index > NB_AXES_CAM + 1)
		return;

	if (_axes_cameras_enabled && index < 3) {
		ofNoFill();
		ofDrawRectangle(viewports[index]); // Axis camera outlines
		_axes_cameras[index].begin(viewports[index]);
	}

	if (index == 3) {
		ofNoFill();
		ofDrawRectangle(viewports[3]); // Main camera outlines
		_main_camera.begin(viewports[index]);
	}
}

void CameraManager::endCamera(std::size_t index) {
	if (index < 0 || index > NB_AXES_CAM + 1)
		return;

	if (_axes_cameras_enabled && index < 3)
		_axes_cameras[index].end();
	else
		_main_camera.end();
}

void CameraManager::setMouseInput(bool enable) {
	if (enable) {
		_main_camera.enableMouseInput();
		for (int i = 0; i < NB_AXES_CAM; i++) {
			_axes_cameras[i].enableMouseInput();
		}
	} else {
		_main_camera.disableMouseInput();
		for (int i = 0; i < NB_AXES_CAM; i++) {
			_axes_cameras[i].disableMouseInput();
		}
	}
}

void CameraManager::setMainCameraOrtho(bool ortho) {
	if (ortho) {
		_main_camera.enableOrtho();
	} else {
		_main_camera.disableOrtho();
	}
}

void CameraManager::toggleAxesCameras(bool enabled) {
	_axes_cameras_enabled = enabled;
	computeViewports();
	IFT_LOG << "axes cameras is now " << (enabled ? "enabled" : "disabled");
}

bool const CameraManager::axesCamerasEnabled() const {
	return _axes_cameras_enabled;
}

const ofCamera& CameraManager::get(int i) const {
	IFT_ASSERT(i >= 0 && i <= 3, "Could not get camera [" << i << "], out of range [0, 3]");
	if (i == 3) {
		return _main_camera;
	} else if (i < NB_AXES_CAM && i >= 0) {
		return _axes_cameras[i];
	} else {
		exit(EXIT_FAILURE);
	}
}
} // namespace ift3100