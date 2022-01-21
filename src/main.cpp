#include <iostream>
#include "ofMain.h"
#include "application.h"

int main(int argc, char**argv) {
	ofGLFWWindowSettings windowSettings;

	ofSetupOpenGL(512, 512, OF_WINDOW);

  	// instancier et lancer l'application (openFrameworks 0.11.0)
  	ofRunApp(new Application());

	return EXIT_SUCCESS;
}
