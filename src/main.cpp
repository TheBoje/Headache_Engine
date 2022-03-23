#include "Application.h"
#include "Interface.h"
#include "Renderer2D.h"
#include "Renderer3D.h"
#include "ofMain.h"

int main(int argc, char** argv) {
	ofSetLogLevel(OF_LOG_VERBOSE);

	ofGLFWWindowSettings windowSettings;
	windowSettings.setSize(1024, 1024);
	// windowSettings.setGLVersion(3, 2);
	ofCreateWindow(windowSettings);
	ofRunApp(ift3100::Application::Get());

	return EXIT_SUCCESS;
}
