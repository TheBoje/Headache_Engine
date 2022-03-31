#include "Interface.h"
#include "Application.h"

#include <string>

namespace ift3100 {
Interface* Interface::_interface = nullptr;

Interface::Interface() { }

Interface::~Interface() { }

Interface* Interface::Get() {
	if (_interface == nullptr) {
		_interface = new Interface();
	}
	return _interface;
}

void Interface::setup() {
	_gui.setup(new Theme(), true, ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable, true);

	animPaused = false;
	inspector.setup();

	primitiveStrokeWidth   = DEFAULT_STROKE_WIDTH;
	primitiveStrokeColor   = ofColor::white;
	primitiveStrokeColor.w = 1;
	primitiveFillColor	   = ofColor::gray;
	primitiveFillColor.w   = 1;
	primitiveFill		   = true;
	mouseAction			   = None;
	drawMode			   = Line;
	axesCameraEnable	   = false;
	mainCameraOrtho		   = false;

	isHistComputed = false;

	IFT_LOG << "done";
}

float getter(void* data, int index) {
	return (float)((int*)data)[index];
}

void Interface::loadImage(std::string path) {
	image.load(path);
	textureSourceID = _gui.loadTexture(textureSource, path);
}

void Interface::imageUI() {
	if (ImGui::Button("compute histogram") && image.isAllocated()) {
		_rgb		   = ImageUtils::computeHistRGB(image);
		isHistComputed = true;
	}

	if (isHistComputed) {
		unsigned int maxr = 0;
		unsigned int maxg = 0;
		unsigned int maxb = 0;

		for (int i = 0; i < 256; i++) {
			maxr = std::max(_rgb[0][i], maxr);
			maxg = std::max(_rgb[1][i], maxg);
			maxb = std::max(_rgb[2][i], maxb);
		}

		ImGui::PlotHistogram("R", &getter, _rgb[0], 256, 0, NULL, 0.0f, (float)maxr, ImVec2(0, 80));
		ImGui::NewLine();
		ImGui::PlotHistogram("G", &getter, _rgb[1], 256, 0, NULL, 0.0f, (float)maxg, ImVec2(0, 80));
		ImGui::NewLine();
		ImGui::PlotHistogram("B", &getter, _rgb[2], 256, 0, NULL, 0.0f, (float)maxb, ImVec2(0, 80));
		ImGui::NewLine();
	}

	ImGui::InputText("Output file name", imageRenderName, IM_ARRAYSIZE(imageRenderName));
	if (ImGui::Button("Export")) {
		ImageUtils::exportImage(imageRenderName);
	}

	ImGui::InputText("Import 3D file", import3DObj, IM_ARRAYSIZE(import3DObj));
	if (ImGui::Button("Import")) {
		Renderer3D::Get()->importFromPath(import3DObj);
	}

	if (image.isAllocated())
		ImGui::Image((ImTextureID)(uintptr_t)textureSourceID, ImVec2(textureSource.getWidth() / 4, textureSource.getHeight() / 4));
}

void Interface::drawingUI() {
	const char* items[] = {"Point", "Line", "Rectangle", "Ellipse", "Triangle", "Cross", "Star"};

	int drawModeCurrentIndex = drawMode;
	if (ImGui::BeginCombo("Primitive type", items[drawModeCurrentIndex])) {
		for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
			bool is_selected = (drawModeCurrentIndex == n);
			if (ImGui::Selectable(items[n], is_selected))
				drawModeCurrentIndex = n;
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	drawMode = InterfaceUtils::intToPrimitiveType(drawModeCurrentIndex);

	ImGui::SliderFloat("Stroke Width", &primitiveStrokeWidth, 0.0f, 20.0f);
	ImGui::ColorEdit4("Stroke Color", (float*)&primitiveStrokeColor);
	ImGui::ColorEdit4("Fill Color", (float*)&primitiveFillColor);
	ImGui::Checkbox("Enable fill", &primitiveFill);

	bool isDrawPrimitive = mouseAction == DrawPrimitive;
	if (ImGui::Checkbox("Draw", &isDrawPrimitive)) {
		mouseAction = DrawPrimitive;
	}

	if (ImGui::Button("Undo")) {
		Renderer2D::Get()->undoPrimitive();
	}
	ImGui::SameLine();
	if (ImGui::Button("Redo")) {
		Renderer2D::Get()->redoPrimitive();
	}
}

void Interface::draw3dRendererUI() {
	if (ImGui::BeginMenu("Objects")) {
		if (ImGui::MenuItem("Cube", NULL, false, true)) {
			Renderer3D::Get()->hierarchy.addChild(std::make_shared<Object3D>("Cube", ofBoxPrimitive()));
		}
		if (ImGui::MenuItem("Cone", NULL, false, true)) {
			Renderer3D::Get()->hierarchy.addChild(std::make_shared<Object3D>("Cone", ofConePrimitive()));
		}
		if (ImGui::MenuItem("Plane", NULL, false, true)) {
			Renderer3D::Get()->hierarchy.addChild(std::make_shared<Object3D>("Plane", ofPlanePrimitive()));
		}
		if (ImGui::MenuItem("Sphere", NULL, false, true)) {
			Renderer3D::Get()->hierarchy.addChild(std::make_shared<Object3D>("Sphere", ofSpherePrimitive()));
		}
		if (ImGui::MenuItem("IcoSphere", NULL, false, true)) {
			Renderer3D::Get()->hierarchy.addChild(std::make_shared<Object3D>("IcoSphere", ofIcoSpherePrimitive()));
		}
		if (ImGui::MenuItem("Cylinder", NULL, false, true)) {
			Renderer3D::Get()->hierarchy.addChild(std::make_shared<Object3D>("Cylinder", ofCylinderPrimitive()));
		}
		if(ImGui::MenuItem("Camera", NULL, false, true)) {
			Renderer3D::Get()->hierarchy.addChild(std::make_shared<Object3D>("Camera", ofCamera()));
		}

		ImGui::EndMenu();
	}
}

void Interface::drawAnimator() {
	if (animPaused) {
		if (ImGui::Button("Resume")) {
			Renderer3D::Get()->animator.resume();
			animPaused = false;
		}
	} else {
		if (ImGui::Button("Pause")) {
			Renderer3D::Get()->animator.pause();
			animPaused = true;
		}
	}

	if (ImGui::Button("Reset")) {
		Renderer3D::Get()->animator.reset();
		animPaused = true;
	}
}

void Interface::draw() {
	_gui.begin();
	bool* mainmenu;
	ImGui::Begin("IFT-3100 - Main menu", mainmenu, ImGuiWindowFlags_MenuBar);
	{
		if (ImGui::CollapsingHeader("Debug", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("Mouse action: %s", InterfaceUtils::MouseActionToChar(mouseAction));
		}

		if (ImGui::CollapsingHeader("Image")) {
			imageUI();
		}

		if (ImGui::CollapsingHeader("Tree")) {
			Renderer2D::Get()->hierarchyPrimitives.drawUI();
		}

		if (ImGui::CollapsingHeader("3d tree")) {
			Renderer3D::Get()->hierarchy.drawUI();
		}

		if (ImGui::CollapsingHeader("Drawing")) {
			drawingUI();
		}

		if (ImGui::CollapsingHeader("Animator")) {
			drawAnimator();
		}

		if (ImGui::BeginMenuBar()) {
			draw3dRendererUI();
			ImGui::EndMenuBar();
		}

		if (ImGui::CollapsingHeader("Cameras")) {
			if (ImGui::Checkbox("Activate axes cameras", &axesCameraEnable)) {
				Renderer3D::Get()->toggleAxesCameras(axesCameraEnable);
			}

			if (ImGui::Checkbox("Main camera ortho", &mainCameraOrtho)) {
				Renderer3D::Get()->setMainCameraOrtho(mainCameraOrtho);
			}
		}
	}

	if (!Renderer2D::Get()->hierarchyPrimitives.selected_nodes.empty()) {
		ImGui::Begin("IFT-3100 - Inspector");
		{ inspector.drawInspectorVectorPrimitive(&Renderer2D::Get()->hierarchyPrimitives.selected_nodes); }
	}

	if (!Renderer3D::Get()->hierarchy.selected_nodes.empty()) {
		ImGui::Begin("IFT-3100 - Inspector 3D");
		{ inspector.drawInspector3d(&Renderer3D::Get()->hierarchy.selected_nodes); }
	}


	if(Renderer3D::Get()->isCameraSelected) {
		ImGui::Begin("Camera preview");
		{ ofxImGui::AddImage(Renderer3D::Get()->selectedCameraFBO, ofVec2f(200, 200)); }
	}

	_gui.end();
}
} // namespace ift3100