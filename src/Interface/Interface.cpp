#include "Interface.h"
#include "Application.h"
#include "ParamCurve.h"

#include "Raytracing.h"

#include <string>

namespace ift3100 {
Interface* Interface::_interface = nullptr;

Interface::Interface() { }

Interface::~Interface() {
	delete theme;
	delete mainMenu;
}

Interface* Interface::Get() {
	if (_interface == nullptr) {
		_interface = new Interface();
	}
	return _interface;
}

void Interface::setup() {
	theme				   = new Theme();
	mainMenu			   = new bool;
	*mainMenu			   = true;
	ImGuiConfigFlags flags = ImGuiConfigFlags_DockingEnable;
	// flags |= ImGuiConfigFlag	s_ViewportsEnable;
	_gui.setup(theme, true, flags, true);

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

	if (ImGui::Button("Render")) {
		ofImage				  image;
		std::vector<Model*>	  models;
		std::vector<ofLight*> lights;
		Renderer3D::Get()->hierarchy.mapChildren([&](std::shared_ptr<Object3D> object) {
			if (object->getType() == ObjectType::Model3D)
				models.emplace_back(object->getModel());

			if (object->getType() == ObjectType::Light)
				lights.emplace_back((ofLight*)object->getNode());
		});
		Raytracing raytracing(Renderer3D::Get()->selectedCamera, lights, models);
		raytracing.render();
		raytracing.saveImage();
	}
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

/**
 * @brief Menu for every instanciation of 3D models and objects
 */
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
			Renderer3D::Get()->hierarchy.addChild(std::make_shared<Object3D>("Sphere", ofSpherePrimitive(10, 64)));
		}
		if (ImGui::MenuItem("IcoSphere", NULL, false, true)) {
			Renderer3D::Get()->hierarchy.addChild(std::make_shared<Object3D>("IcoSphere", ofIcoSpherePrimitive()));
		}
		if (ImGui::MenuItem("Cylinder", NULL, false, true)) {
			Renderer3D::Get()->hierarchy.addChild(std::make_shared<Object3D>("Cylinder", ofCylinderPrimitive()));
		}
		if (ImGui::MenuItem("Curve Bezier", NULL, false, true)) {
			ParamCurve pc(ParamCurveType::Bezier, 50);
			pc.setup({-100, 0, 0}, {-50, 100, 0}, {50, -100, 0}, {100, 0, 0});
			Renderer3D::Get()->hierarchy.addChild(std::make_shared<Object3D>("bezier curve", pc));
		}
		if (ImGui::MenuItem("Curve Hermite", NULL, false, true)) {
			ParamCurve pc(ParamCurveType::Hermite, 50);
			pc.setup({-100, 0, 0}, {-50, 100, 0}, {50, -100, 0}, {100, 0, 0});
			Renderer3D::Get()->hierarchy.addChild(std::make_shared<Object3D>("hermite curve", pc));
		}
		if (ImGui::MenuItem("Surface Coons (Bezier)", NULL, false, true)) {
			// TODO(Louis): Change control points translation from UI!
			ParamSurface ps(ParamSurfaceType::Coons, 20, 20);
			ps.setup({{-15, 0, -15},
				{-5, -10, -15},
				{5, 50, -15},
				{15, 0, -15},
				{15, -10, -5},
				{15, -40, 5},
				{15, 0, 15},
				{5, 20, 15},
				{-5, 0, 15},
				{-15, 10, 15},
				{-15, -30, 15},
				{-15, 0, 5}});
			Renderer3D::Get()->hierarchy.addChild(std::make_shared<Object3D>("surface coons", ps));
		}
		ImGui::Separator();
		if (ImGui::MenuItem("Camera", NULL, false, true)) {
			Renderer3D::Get()->hierarchy.addChild(std::make_shared<Object3D>("Camera", ofCamera()));
		}
		if (ImGui::MenuItem("Light", NULL, false, true)) {
			if (Renderer3D::Get()->lights.size() == MAX_LIGHTS) {
				IFT_LOG_WARNING << "Max number of light is reached. Max lights = " << MAX_LIGHTS;
			} else {
				std::shared_ptr<Object3D> light = std::make_shared<Object3D>("Light", ofLight());
				Renderer3D::Get()->lights.emplace_back(light);
				Renderer3D::Get()->hierarchy.addChild(light);
			}
		}

		ImGui::EndMenu();
	}
}

/**
 * @brief Menu for every options modifying the interface
 * or the engine functionnality
 */
void Interface::drawOptionsMenu() {
	if (ImGui::BeginMenu("Options")) {
		ImGui::Checkbox("Enable exploding on selected meshes", &Renderer3D::Get()->isExploding);
		ImGui::Separator();

		const char* items[] = {"Default", "Phong illumination", "Lambert illumination", "Gouraud illumination", "BlinnPhong illumination"};

		ImGui::Text("illumination:");
		if (ImGui::BeginListBox("##listboxIllum")) {
			for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
				const bool is_selected = (Renderer3D::Get()->illumination == n);
				if (ImGui::Selectable(items[n], is_selected)) {
					Renderer3D::Get()->illumination = (IlluminationStyle)n;
					IFT_LOG << "switched to " << items[n];
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}
		ImGui::EndMenu();
	}
}

void Interface::drawAnimator() { }

void Interface::drawMaterialViewer() {
	Model* mod = nullptr;
	for (auto selected : Renderer3D::Get()->hierarchy.selected_nodes) {
		if (selected->getRef()->getType() == ObjectType::Model3D) {
			mod = new Model(*selected->getRef()->getModel());
			break;
		}
	}

	// If there is no model selected in the Renderer3D, we escape the function
	if (mod == nullptr)
		return;

	MaterialViewer::Get()->setTarget(*mod);
	MaterialViewer::Get()->draw();

	ImGui::Begin("Material preview");

	const char* items[] = {"Default", "Phong illumination", "Lambert illumination", "Gouraud illumination", "BlinnPhong illumination"};

	ImGui::Text("illumination:");
	if (ImGui::BeginListBox("##listbox")) {
		for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
			const bool is_selected = (MaterialViewer::Get()->illuminationStyle == n);
			if (ImGui::Selectable(items[n], is_selected)) {
				MaterialViewer::Get()->illuminationStyle = (IlluminationStyle)n;
				IFT_LOG << "switched to " << items[n];
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	const char* itemsPrimitive[] = {"Sphere", "Cube", "IcoSphere", "Cone"};

	ImGui::Text("Primitive:");
	if (ImGui::BeginListBox("##listbox2")) {
		for (int n = 0; n < IM_ARRAYSIZE(itemsPrimitive); n++) {
			const bool is_selected = (MaterialViewer::Get()->getType() == n);
			if (ImGui::Selectable(itemsPrimitive[n], is_selected)) {
				MaterialViewer::Get()->setPrimitiveType((PreviewPrimitiveType)n);
				IFT_LOG << "switched to " << itemsPrimitive[n] << " display type";
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	ImVec4 lightColor = MaterialViewer::Get()->getLightColor();
	if (ImGui::ColorEdit4("Light color", (float*)&lightColor)) {
		MaterialViewer::Get()->setLightColor(lightColor);
	}

	ImGui::Begin("Preview");
	{
		ofxImGui::AddImage(MaterialViewer::Get()->getFbo(),
			ofVec2f(ImGui::GetWindowHeight() * (MaterialViewer::Get()->getFbo().getWidth() / MaterialViewer::Get()->getFbo().getHeight()),
				ImGui::GetWindowHeight()));
	}
	ImGui::End();
}

void Interface::draw() {
	_gui.begin();
	ImGui::Begin("Main menu", mainMenu, ImGuiWindowFlags_MenuBar);

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

		if (ImGui::BeginMenuBar()) {
			draw3dRendererUI();
			drawOptionsMenu();
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
		ImGui::Begin("Inspector 2D");
		{ inspector.drawInspectorVectorPrimitive(&Renderer2D::Get()->hierarchyPrimitives.selected_nodes); }
	}

	if (!Renderer3D::Get()->hierarchy.selected_nodes.empty()) {
		ImGui::Begin("Inspector 3D");
		{ inspector.drawInspector3d(&Renderer3D::Get()->hierarchy.selected_nodes); }
	}

	drawMaterialViewer();

	ImGui::Begin("Animator");
	{
		Renderer3D::Get()->animatorManager.drawUI();
		ImGui::Separator();

		if (!Renderer3D::Get()->hierarchy.selected_nodes.empty()) {
			if (ImGui::Button("Add animator")) {
				Renderer3D::Get()->animatorManager.addAnimator(Renderer3D::Get()->hierarchy.selected_nodes.at(0)->getRef());
			}
		}
	}

	auto fbo = Renderer3D::Get()->selectedCameraFBO;
	if (fbo.isAllocated()) {
		ImGui::Begin("Camera preview");
		{ ofxImGui::AddImage(fbo, ofVec2f(ImGui::GetWindowHeight() * (fbo.getWidth() / fbo.getHeight()), ImGui::GetWindowHeight())); }
	}

	_gui.end();
}
} // namespace ift3100