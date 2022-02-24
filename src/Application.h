#ifndef APPLICATION_H
#define APPLICATION_H

#include "ofMain.h"
#include "Renderer.h"
#include "Interface.h"
#include "Curseur.h"

namespace ift3100 {
	class Application : public ofBaseApp {
	public:

	Renderer renderer;
    Interface interface;

	void setup();
	void update();
	void keyReleased(int key);
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
	};
}

#endif // !APPLICATION_H