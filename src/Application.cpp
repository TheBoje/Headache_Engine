#include "Application.h"
#include "ImageUtils.h"
#include "Hierarchy.h"

namespace ift3100 {
	// fonction d'initialisation de l'application
	void Application::setup()
	{
		ofSetWindowTitle("IFT-3100 Main");

        interface.setup();
		renderer.setup();

		ofLog() << "<app::setup> done";
	}

	// fonction de mise à jour de la logique de l'application
	void Application::update() {
        renderer.update();
		
		// UI - call proper function after changing state
		if (interface.primitiveUndo) {
			interface.primitiveUndo = false;
			renderer.undoPrimitive();
		} else if (interface.primitiveRedo) {
			interface.primitiveRedo = false;
			renderer.redoPrimitive();
		}
	}

	// fonction de mise à jour du rendu de la fenêtre d'affichage de l'application
	void Application::draw() {
        renderer.draw();
		interface.draw();
	}

	// fonction appelée juste avant de quitter l'application
	void Application::exit() {
        ofLog() << "<app::exit>";
	}

	void Application::addImage(ofImage * image) {
		ofLog() << "<app:addImage> x:" << image->getHeight() << " y:" << image->getWidth();
		renderer._image = image;
	}

	void Application::keyReleased(int key)
	{
		ofLog() << "<app::keyReleased: " << key << ">";

		if (key == ' ') {
			ift3100::ImageUtils::exportImage("render.png");
		}
        // if (key == 117) // touche u
        // {
        //     interface.checkbox = !interface.checkbox;
        //     ofLog() << "<toggle ui: " << interface.checkbox << ">";
        // }
	}

	void Application::mouseMoved(int x, int y) {
		interface.mousePos.x = x;
		interface.mousePos.y = y;
        renderer.curseur.souris_courant_x = x;
        renderer.curseur.souris_courant_y = y;

	}

	void Application::mouseDragged(int x, int y, int button) {
		interface.mousePos.x = x;
		interface.mousePos.y = y;
	}

	void Application::mousePressed(int x, int y, int button) {
		interface.mousePos.x = x;
		interface.mousePos.y = y;
		interface.mousePos.z = x;
		interface.mousePos.w = y;
	}

	void Application::mouseReleased(int x, int y, int button) {
		interface.mousePos.x = x;
		interface.mousePos.y = y;
		// Don't draw anything if clicking on the UI - one of these flag will be triggered
		if (ImGui::IsAnyWindowFocused() || ImGui::IsAnyWindowHovered() || ImGui::IsAnyItemHovered()) return;
		// Call proper render method based on UI state / mouse action
		switch (interface.mouseAction) {
			case DrawPrimitive:
				renderer.addPrimitive(interface.mousePos, interface.drawMode, 
									interface.primitiveStrokeWidth, interface.primitiveStrokeColor, 
									interface.primitiveFill, interface.primitiveFillColor);
				break;
			case None:
				break;
			default:
				ofLog() << "<app::mouseReleased> Mouse Action:" << interface.mouseAction << " unknown";
				break;
		};
	}
	

	void Application::mouseEntered(int x, int y) {
		interface.mousePos.x = x;
		interface.mousePos.y = y;
	}

	void Application::mouseExited(int x, int y) {
		interface.mousePos.x = x;
		interface.mousePos.y = y;
	}

	void Application::dragEvent(ofDragInfo dragInfo) {
		ofLog() << "<app::ofDragInfo> " << dragInfo.files.at(0);

		if (renderer._image == nullptr) {
			renderer._image = new ofImage();
		}

		renderer._image->load(dragInfo.files.at(0));

		if (renderer._image->getWidth() > 0 && renderer._image->getHeight() > 0)
			ofSetWindowShape(renderer._image->getWidth(), renderer._image->getHeight());
	}

    void Application::windowResized(int w, int h)
    {
        ofLog() << "<app::windowResized: (" << w << ", " << h << ")>";
    }
}
