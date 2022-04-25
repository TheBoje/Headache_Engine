#include "Application.h"
#include "Interface.h"
#include "Renderer2D.h"
#include "Renderer3D.h"
#include "ofMain.h"

int main(int argc, char** argv) {
#ifdef SILENT
	ofSetLogLevel(OF_LOG_SILENT);
#else
	ofSetLogLevel(OF_LOG_NOTICE);
#endif

	ofGLFWWindowSettings windowSettings;
	windowSettings.setSize(1024, 1024);
	windowSettings.setGLVersion(3, 3);
	ofCreateWindow(windowSettings);
	ofRunApp(ift3100::Application::Get());

	return EXIT_SUCCESS;
}
