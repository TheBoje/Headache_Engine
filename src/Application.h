#ifndef APPLICATION_H
#define APPLICATION_H

#include "ofMain.h"
#include "Interface.h"
#include "Cursor.h"
#include "Renderer.h"
#include "Cursor.h"
#include "ImageUtils.h"
#include "Hierarchy.h"
#include "Logger.h"

namespace ift3100 {
	class Application : public ofBaseApp {
	public:
	Renderer renderer;
    Interface interface;
	Cursor cursor;

	bool isMouseDown;

	Application();

	void setup();
	void update();
	void keyReleased(int key);
	void keyPressed(int key);
	void dragEvent(ofDragInfo dragInfo);
	void draw();
	void exit();

	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
    void windowResized(int w, int h);

	void drawPrimitivePreview();
	void rendererUndo();
	void rendererRedo();
	void exportRender(std::string filename);
	};
}

#endif // !APPLICATION_H