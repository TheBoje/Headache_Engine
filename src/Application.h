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

    void windowResized(int w, int h);

	void addImage(ofImage * image);
	};
}

#endif // !APPLICATION_H