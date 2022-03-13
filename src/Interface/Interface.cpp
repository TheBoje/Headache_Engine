#include <string>

#include "Application.h"
#include "Interface.h"

namespace ift3100 {
Interface::Interface(Application& _application)
	: application(_application) { }

void Interface::setup() {
	_gui.setup();
	_gui.setTheme(new Theme());
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

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

float getter(void* data, int index) { return (float)((int*)data)[index]; }

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
		ImGui::PlotHistogram("R", &getter, _rgb[0], 256, 0, NULL, 0.0f, 70000.0f, ImVec2(0, 80));
		ImGui::NewLine();
		ImGui::PlotHistogram("G", &getter, _rgb[1], 256, 0, NULL, 0.0f, 70000.0f, ImVec2(0, 80));
		ImGui::NewLine();
		ImGui::PlotHistogram("B", &getter, _rgb[2], 256, 0, NULL, 0.0f, 70000.0f, ImVec2(0, 80));
		ImGui::NewLine();
	}

	ImGui::InputText("Output file name", imageRenderName, IM_ARRAYSIZE(imageRenderName));
	if (ImGui::Button("Export")) {
		application.exportRender(std::string(imageRenderName));
	}

	if (image.isAllocated())
		ImGui::Image((ImTextureID)(uintptr_t)textureSourceID, ImVec2(textureSource.getWidth() / 4, textureSource.getHeight() / 4));
}

void Interface::drawingUI() {
	const char* items[]				 = {"Point", "Line", "Rectangle", "Ellipse", "Triangle", "Cross", "Star"};
	int			drawModeCurrentIndex = drawMode;
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
		if (isDrawPrimitive)
			mouseAction = DrawPrimitive;
		else
			mouseAction = None;
	}

	if (ImGui::Button("Undo")) {
		application.rendererUndo();
	}
	ImGui::SameLine();
	if (ImGui::Button("Redo")) {
		application.rendererRedo();
	}
}

void Interface::draw3dRendererUI() {
	if (ImGui::BeginMenu("Objects")) {
		if (ImGui::MenuItem("Cube", NULL, false, true)) {
			application.renderer3D.hierarchy.addChild(std::make_shared<Object3D>("Cube", ofBoxPrimitive()));
		}
		if (ImGui::MenuItem("Cone", NULL, false, true)) {
			application.renderer3D.hierarchy.addChild(std::make_shared<Object3D>("Cone", ofConePrimitive()));
		}
		if (ImGui::MenuItem("Plane", NULL, false, true)) {
			application.renderer3D.hierarchy.addChild(std::make_shared<Object3D>("Plane", ofPlanePrimitive()));
		}
		if (ImGui::MenuItem("Sphere", NULL, false, true)) {
			application.renderer3D.hierarchy.addChild(std::make_shared<Object3D>("Sphere", ofSpherePrimitive()));
		}
		if (ImGui::MenuItem("IcoSphere", NULL, false, true)) {
			application.renderer3D.hierarchy.addChild(std::make_shared<Object3D>("IcoSphere", ofIcoSpherePrimitive()));
		}
		if (ImGui::MenuItem("Cylinder", NULL, false, true)) {
			application.renderer3D.hierarchy.addChild(std::make_shared<Object3D>("Cylinder", ofCylinderPrimitive()));
		}
		ImGui::EndMenu();
	}
}

void Interface::drawAnimator() {
	if (animPaused) {
		if (ImGui::Button("Resume")) {
			application.renderer3D.anim.resume();
			animPaused = false;
		}
	} else {
		if (ImGui::Button("Pause")) {
			application.renderer3D.anim.pause();
			animPaused = true;
		}
	}

	if (ImGui::Button("Reset")) {
		application.renderer3D.anim.reset();
		animPaused = true;
	}
}

void Interface::draw() {
	_gui.begin();
	bool* mainmenu;
	ImGui::Begin("Main menu", mainmenu, ImGuiWindowFlags_MenuBar);
	{
		if (ImGui::CollapsingHeader("Debug", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("Mouse action: %s", InterfaceUtils::MouseActionToChar(mouseAction));
		}

		if (ImGui::CollapsingHeader("Image")) {
			imageUI();
		}

		if (ImGui::CollapsingHeader("Tree")) {
			application.renderer2D.hierarchyPrimitives.drawUI();
		}

		if (ImGui::CollapsingHeader("3d tree")) {
			application.renderer3D.hierarchy.drawUI();
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
				application.renderer3D.toggleAxesCameras(axesCameraEnable);
			}

			if (ImGui::Checkbox("Main camera ortho", &mainCameraOrtho)) {
				application.renderer3D.setMainCameraOrtho(mainCameraOrtho);
			}
		}
	}

	if (!application.renderer2D.hierarchyPrimitives.selected_nodes.empty()) {
		ImGui::Begin("Inspector");
		{ inspector.drawInspectorVectorPrimitive(&application.renderer2D.hierarchyPrimitives.selected_nodes); }
	}

	if (!application.renderer3D.hierarchy.selected_nodes.empty()) {
		ImGui::Begin("Inspector 3D");
		{ inspector.drawInspector3d(&application.renderer3D.hierarchy.selected_nodes); }
	}

	_gui.end();
}
} // namespace ift3100