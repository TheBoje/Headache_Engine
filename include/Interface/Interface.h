#ifndef IFT_3100_INTERFACE_H
#define IFT_3100_INTERFACE_H

#include "Hierarchy.h"
#include "ImageUtils.h"
#include "InspectorInterface.h"
#include "Renderer2D.h"
#include "Renderer3D.h"
#include "Theme.h"
#include "ofImage.h"
#include "ofMain.h"
#include "ofxImGui.h"

#include <string.h>

namespace ift3100 {
class Application;
class Interface {
private:
	static Interface* _interface;

protected:
	Interface();

public:
	Interface(const Interface&) = delete;
	~Interface();

	static Interface* Get();

	Interface& operator=(const Interface&) = delete;

public:
	InspectorInterface inspector;

	ofxImGui::Gui _gui;
	Theme* theme;
	bool* mainMenu;

	unsigned int** _rgb;

	float primitiveStrokeWidth;
	ImVec4 primitiveStrokeColor; // Dont question it.
	bool primitiveFill;
	ImVec4 primitiveFillColor; // Forget it.

	PrimitiveType drawMode;
	MouseAction mouseAction;

	ofTexture textureSource;
	GLuint textureSourceID;
	ofTexture cameraTexture;
	GLuint cameraTextureID;

	ofImage image;
	bool isHistComputed;
	char imageRenderName[64] = "";
	char import3DObj[64] = "";

	bool axesCameraEnable;
	bool mainCameraOrtho;

	void setup();
	void loadImage(std::string path);
	void imageUI();
	void drawingUI();
	void drawInspector();
	void draw3dRendererUI();
	void drawOptionsMenu();
	void drawAnimator();

	void draw();
	void button_pressed();
};
} // namespace ift3100

#endif //IFT_3100_INTERFACE_H