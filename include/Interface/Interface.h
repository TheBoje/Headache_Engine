#ifndef IFT_3100_INTERFACE_H
#define IFT_3100_INTERFACE_H

#include "ofMain.h"
#include "ofxImGui.h"
#include "DrawingProperties.h"
#include "InterfaceUtils.h"
#include "Hierarchy.h"
#include "Renderer2D.h"
#include "Theme.h"
#include "ofImage.h"
#include "ImageUtils.h"
#include "Logger.h"
#include "InspectorInterface.h"

#include <string.h>

namespace ift3100 {
class Application;
class Interface {
public:
	Application& application; // Reference to main application for callbacks

	InspectorInterface inspector;

	ofxImGui::Gui _gui;
	Theme		  theme;
	ofVec4f		  mousePos;

	unsigned int** _rgb;

	float  primitiveStrokeWidth;
	ImVec4 primitiveStrokeColor; // Dont question it.
	bool   primitiveFill;
	ImVec4 primitiveFillColor; // Forget it.

	PrimitiveType drawMode;
	MouseAction	  mouseAction;

	ofTexture textureSource;
	GLuint	  textureSourceID;
	ofImage	  image;
	bool	  isHistComputed;
	char	  imageRenderName[64];

	bool axesCameraEnable;
	bool mainCameraOrtho;

	Interface(Application& _application);

	void setup();
	void loadImage(std::string path);
	void imageUI();
	void drawingUI();
	void drawInspector();
	void draw3dRendererUI();
	void draw();
	void button_pressed();
};
} // namespace ift3100

#endif //IFT_3100_INTERFACE_H