#include "Application.h"
namespace ift3100 {
	// Create application, and give interface and renderers a reference of itself
	Application::Application() : interface(*this), renderer2D(*this), renderer3D(*this) {}
	Application::~Application() {}

	// fonction d'initialisation de l'application
	void Application::setup() {
		ofSetWindowTitle("IFT-3100 Main");

		isMouseDown = false;
        interface.setup();
		renderer2D.setup();
		renderer3D.setup();
        cursor.setup();
		IFT_LOG << "done";
	}

	// fonction de mise à jour de la logique de l'application
	void Application::update() {
		if (isMouseDown && interface.mouseAction == DrawPrimitive) {
			drawPrimitivePreview();
		}

		// Low framerate warning
		if (ofGetFrameRate() < 5 && ofGetFrameNum() > 5) {
			IFT_LOG_WARNING << std::setprecision(2) << "frame:" << ofGetFrameNum() << " fps: " << ofGetFrameRate();
		}

        renderer2D.update();
		renderer3D.update();
	}

	// fonction de mise à jour du rendu de la fenêtre d'affichage de l'application
	void Application::draw() {
        renderer2D.draw();
		renderer3D.draw();
		interface.draw();
		cursor.draw(interface.mousePos.x, interface.mousePos.y, interface.mouseAction, interface.drawMode);
	}

	// fonction appelée juste avant de quitter l'application
	void Application::exit() {
        IFT_LOG;
	}

	void Application::keyReleased(int key) {
		if (key == ' ') {
			ift3100::ImageUtils::exportImage("render.png");
		}
	}

	void Application::keyPressed(int key) {
		if(key == OF_KEY_DEL) {
			// Delete each selected VectorPrimitive in hierarchy
			for(Hierarchy<VectorPrimitive> * selected : renderer2D.hierarchyPrimitives.selected_nodes) {
				if(renderer2D.hierarchyPrimitives.isRoot(*selected))
					renderer2D.hierarchyPrimitives.clear();
				else
					delete selected;
			}

			for (auto it = renderer2D.primitives.begin(); it != renderer2D.primitives.end(); it++) {
				// remove shared_ptr that are only in the primitives vector (meaning that there are not in the hierarchy)
				if (it->use_count() == 1) {
					renderer2D.primitives.erase(it--);
				}
			}
			renderer2D.hierarchyPrimitives.selected_nodes.clear();
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
				renderer2D.addPrimitive(interface.mousePos, interface.drawMode,
									interface.primitiveStrokeWidth, interface.primitiveStrokeColor,
									interface.primitiveFill, interface.primitiveFillColor);
				break;
			case None:
				break;
			default:
				IFT_LOG << "Mouse Action:" << interface.mouseAction << " unknown";
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
		IFT_LOG << dragInfo.files.at(0);
		interface.loadImage(dragInfo.files.at(0));
	}

	void Application::windowResized(int w, int h) {
		IFT_LOG << "(" << w << ", " << h << ")";
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
		renderer2D.addPrimitive(interface.mousePos, interface.drawMode,
							interface.primitiveStrokeWidth, primitiveStrokeColorPreview,
							interface.primitiveFill, primitiveFillColorPreview, 1);
		// Draw bounding box of drawing
		renderer2D.addPrimitive(interface.mousePos, Rectangle, 1, ofColor(0, 80), false, ofColor::white, 1);
	}

	void Application::rendererUndo() {
		renderer2D.undoPrimitive();
	}

	void Application::rendererRedo() {
		renderer2D.redoPrimitive();
	}

	void Application::exportRender(std::string name) {
		name += ".png";
		ImageUtils::exportImage(name);
	}
}
