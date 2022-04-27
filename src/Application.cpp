#include "Application.h"

#include "Interface.h"
#include "Renderer2D.h"
#include "Renderer3D.h"

namespace ift3100 {
Application* Application::application = nullptr;

Application::Application() { }
Application::~Application() { }
Application* Application::Get() {
	if (application == nullptr) {
		application = new Application();
	}
	return application;
}

// fonction d'initialisation de l'application
void Application::setup() {
	ofSetWindowTitle("IFT-3100 - Main");
	// ofAppGLFWWindow* win = dynamic_cast<ofAppGLFWWindow*>(ofGetWindowPtr());
	// win->setWindowIcon("../../&.ico");

	ofDisableArbTex();
	ofSetVerticalSync(true);
	ofSetCircleResolution(32);
	isMouseDown = false;

	interface  = Interface::Get();
	renderer2D = Renderer2D::Get();
	renderer3D = Renderer3D::Get();

	interface->setup();
	renderer2D->setup();
	renderer3D->setup();

	cursor.setup();

	IFT_LOG << "done";
}

// fonction de mise à jour de la logique de l'application
void Application::update() {
	// Draw 2D primitive preview if UI not used && drawing mode on
	if (isMouseDown && interface->mouseAction == DrawPrimitive &&
		(!ImGui::IsWindowFocused() || !ImGui::IsWindowHovered() || !ImGui::IsAnyItemHovered())) {
		// NOTE(Refactor): Maybe this bit belongs in `Interface`?
		Renderer2D::Get()->addPreviewPrimitive(mousePos,
			interface->drawMode,
			interface->primitiveStrokeWidth,
			interface->primitiveStrokeColor,
			interface->primitiveFill,
			interface->primitiveFillColor);
	}

	// Low framerate warning
	if (ofGetFrameRate() < 5 && ofGetFrameNum() > 5) {
		IFT_LOG_WARNING << std::setprecision(2) << "frame:" << ofGetFrameNum() << " fps: " << ofGetFrameRate();
	}

	renderer2D->update();
	renderer3D->update();
}

// fonction de mise à jour du rendu de la fenêtre d'affichage de l'application
void Application::draw() {
	renderer3D->draw();
	renderer2D->draw();
	interface->draw();
	cursor.draw(mousePos.x, mousePos.y, interface->mouseAction, interface->drawMode);
}

// fonction appelée juste avant de quitter l'application
void Application::exit() {
	IFT_LOG << "done";
}

void Application::keyReleased(int key) {
	if (key == ' ') {
		ift3100::ImageUtils::exportImage("render.png");
	}
}

void Application::keyPressed(int key) {
	if (key == OF_KEY_DEL) {
		renderer2D->deleteSelected();
		renderer3D->deleteSelected();
	}
}

void Application::mouseMoved(int x, int y) {
	mousePos.x = x;
	mousePos.y = y;
}

void Application::mouseDragged(int x, int y, int button) {
	mousePos.x = x;
	mousePos.y = y;
}

void Application::mousePressed(int x, int y, int button) {
	mousePos.x	= x;
	mousePos.y	= y;
	mousePos.z	= x;
	mousePos.w	= y;
	isMouseDown = true;
	if (ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered() || interface->mouseAction == DrawPrimitive) {
		renderer3D->setMouseInput(false); // Disable camera mouse input
	}
}

void Application::mouseReleased(int x, int y, int button) {
	mousePos.x	= x;
	mousePos.y	= y;
	isMouseDown = false;
	renderer3D->setMouseInput(true); // Enable mouse input

	// Don't draw anything if clicking on the UI - one of these flag will be triggered
	if (ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered())
		return;
	// Call proper render method based on UI state / mouse action
	switch (interface->mouseAction) {
		case DrawPrimitive:
			renderer2D->addPrimitive(mousePos,
				interface->drawMode,
				interface->primitiveStrokeWidth,
				interface->primitiveStrokeColor,
				interface->primitiveFill,
				interface->primitiveFillColor);
			break;
		case None: break;
		default: IFT_LOG_WARNING << "Mouse Action:" << interface->mouseAction << " unknown"; break;
	};
}

void Application::mouseEntered(int x, int y) {
	mousePos.x = x;
	mousePos.y = y;
}

void Application::mouseExited(int x, int y) {
	mousePos.x = x;
	mousePos.y = y;
}

void Application::dragEvent(ofDragInfo dragInfo) {
	IFT_LOG << dragInfo.files.at(0);

	auto selected_node = Renderer3D::Get()->hierarchy.selected_nodes;

	for (auto node : selected_node) {
		if (node->getRef()->getType() == ObjectType::Model3D) {
			node->getRef()->getModel()->loadTexture(dragInfo.files.at(0));
			return;
		} else if (node->getRef()->getType() == ObjectType::ParametricSurface) {
			node->getRef()->getSurface()->loadTexture(dragInfo.files.at(0));
			return;
		}
	}

	interface->loadImage(dragInfo.files.at(0));
}

void Application::windowResized(int w, int h) {
	renderer3D->cameraManager.windowResize(); // Update camera viewport
	IFT_LOG << "(" << w << ", " << h << ")";
}
} // namespace ift3100
