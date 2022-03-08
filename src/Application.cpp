#include "Application.h"
#include "ImageUtils.h"
#include "Hierarchy.h"

namespace ift3100 {
	// Create application, and give interface a reference of itself
	Application::Application() : interface(*this) {}

	// fonction d'initialisation de l'application
	void Application::setup() {
		ofSetWindowTitle("IFT-3100 Main");

		isMouseDown = false;
        interface.setup();
		renderer.setup();

		ofLog() << "<app::setup> done";
	}

	// fonction de mise à jour de la logique de l'application
	void Application::update() {
        renderer.update();

		if (isMouseDown && interface.mouseAction == DrawPrimitive) {
			drawPrimitivePreview();
		}

		if(!renderer.prims.selected_nodes.empty()) {
			for(Hierarchy<VectorPrimitive> * selected : renderer.prims.selected_nodes) {
				selected->map([=](std::shared_ptr<VectorPrimitive> p)
				{
					p->FILL = interface.primitiveFill;
					p->FILL_COLOR = interface.primitiveFillColor;
					p->STROKE_WIDTH = interface.primitiveStrokeWidth;
					p->STROKE_COLOR = interface.primitiveStrokeColor;
				});
			}
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

	void Application::keyReleased(int key) {
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

	void Application::keyPressed(int key) {
		if(key == OF_KEY_DEL) {

			// Delete each selected VectorPrimitive in hierarchy
			for(Hierarchy<VectorPrimitive> * selected : renderer.prims.selected_nodes) {
				if(renderer.prims.isRoot(*selected))
					renderer.prims.clear();
				else
					delete selected;
			}

			for (auto it = renderer.primitives.begin(); it != renderer.primitives.end(); it++)
			{
				// remove shared_ptr that are only in the primitives vector (meaning that there are not in the hierarchy)
				if (it->use_count() == 1)
				{
					renderer.primitives.erase(it--);
				}
			}
			renderer.prims.selected_nodes.clear();
		} 
	}

	void Application::mouseMoved(int x, int y) {
		interface.mousePos.x = x;
		interface.mousePos.y = y;
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
		isMouseDown = true;
	}

	void Application::mouseReleased(int x, int y, int button) {
		interface.mousePos.x = x;
		interface.mousePos.y = y;
		isMouseDown = false;
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
		ofLog() << "<app::mouseEntered> at (" << x << ", " << y << ")";
		interface.mousePos.x = x;
		interface.mousePos.y = y;
	}

	void Application::mouseExited(int x, int y) {
		ofLog() << "<app::mouseExited> at (" << x << ", " << y << ")";
		interface.mousePos.x = x;
		interface.mousePos.y = y;
	}

	void Application::dragEvent(ofDragInfo dragInfo) {
		ofLog() << "<app::ofDragInfo> " << dragInfo.files.at(0);
		interface.loadImage(dragInfo.files.at(0));
	}

    void Application::windowResized(int w, int h) {
        ofLog() << "<app::windowResized: (" << w << ", " << h << ")>";
    }

	void Application::drawPrimitivePreview() {
		// Don't draw anything if clicking on the UI - one of these flag will be triggered
		if (ImGui::IsAnyWindowFocused() || ImGui::IsAnyWindowHovered() || ImGui::IsAnyItemHovered()) return;
		// Call proper render method based on UI state / mouse action
		ofColor primitiveStrokeColorPreview = interface.primitiveStrokeColor;
		primitiveStrokeColorPreview.a = 80;
		ofColor primitiveFillColorPreview = interface.primitiveFillColor;
		primitiveFillColorPreview.a = 80;
		// Draw transparent preview primitive for 1 frame
		renderer.addPrimitive(interface.mousePos, interface.drawMode, 
							interface.primitiveStrokeWidth, primitiveStrokeColorPreview, 
							interface.primitiveFill, primitiveFillColorPreview, 1);
		// Draw bounding box of drawing
		renderer.addPrimitive(interface.mousePos, Rectangle, 1, ofColor(0, 80), false, ofColor::white, 1);
	}

	void Application::rendererUndo() {
		renderer.undoPrimitive();
	}

	void Application::rendererRedo() {
		renderer.redoPrimitive();
	}

	void Application::exportRender(std::string name) {
		name += ".png";
		ImageUtils::exportImage(name);
	}
}
