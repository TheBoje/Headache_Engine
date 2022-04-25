#include "InspectorInterface.h"

#include <cstring>

namespace ift3100 {

static void HelpMarker(const char* desc) {
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered()) {
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void InspectorInterface::setup() {
	primitivePosition = ImVec2(0, 0);
}

/**
     * @brief Draw the inspector for the vector primitive including
     * - position x y
     * - name
     *
     * When multiple nodes are selected, compute the transformation from
     * the mean of the selected nodes position and apply it to every nodes.
     *
     * @param vvp selected nodes from interface for vector primitive hierarchy container
     */
void InspectorInterface::drawInspectorVectorPrimitive(std::vector<Hierarchy<VectorPrimitive>*>* vectorPrimitives) {
	std::size_t vectorPrimitivesSize = vectorPrimitives->size();
	ImVec2		sum(0, 0);

	// get the mean of all selected primitive position
	for (auto node : *vectorPrimitives) {
		sum = sum + (node->getRef()->POSITION_1 + node->getRef()->POSITION_2) / 2;
	}
	primitivePosition.x = sum.x / vectorPrimitivesSize;
	primitivePosition.y = sum.y / vectorPrimitivesSize;

	/* NAME primitive input */
	/* if several nodes are selected, apply the same name for all */
	char name[64] = "";
	std::strcpy(name, vectorPrimitives->at(0)->getRef()->NAME.c_str());
	if (ImGui::InputText("Name", name, 64, InspectorInterface::INPUT_FLAGS)) {
		for (auto node : *vectorPrimitives) {
			node->getRef()->NAME = name;
		}
	}

	/* X position input decimal */
	char bufx[64] = "";
	std::strcpy(bufx, std::to_string(primitivePosition.x).c_str());
	if (ImGui::InputText("x", bufx, 64, InspectorInterface::INPUT_DECIMAL_FLAGS)) {
		IFT_LOG << "Change x value to " << bufx;
		float disp = atof(bufx) - primitivePosition.x;

		for (auto node : *vectorPrimitives) {
			node->map([=](std::shared_ptr<VectorPrimitive> vectorPrimitive) {
				vectorPrimitive->POSITION_1.x += disp;
				vectorPrimitive->POSITION_2.x += disp;
			});
		}
	}

	/* Y position input decimal */
	char bufy[64] = "";
	std::strcpy(bufy, std::to_string(primitivePosition.y).c_str());
	if (ImGui::InputText("y", bufy, 64, InspectorInterface::INPUT_DECIMAL_FLAGS)) {
		IFT_LOG << "Change y value to " << bufy;
		float disp = atof(bufy) - primitivePosition.y;

		for (auto node : *vectorPrimitives) {
			node->map([=](std::shared_ptr<VectorPrimitive> vectorPrimitive) {
				vectorPrimitive->POSITION_1.y += disp;
				vectorPrimitive->POSITION_2.y += disp;
			});
		}
	}
}

void InspectorInterface::drawInspector3d(std::vector<Hierarchy<Object3D>*>* object3Ds) {
	std::size_t object3DsSize = object3Ds->size();
	ofVec3f		sum(0);

	// get the mean of all selected primitive position
	for (auto node : *object3Ds) {
		if (node->getRef()->getNode() == nullptr) {
			return;
		}
		sum += sum + node->getRef()->getNode()->getPosition();
	}
	position3d.x = sum.x / object3DsSize;
	position3d.y = sum.y / object3DsSize;
	position3d.z = sum.z / object3DsSize;

	/* if several nodes are selected, apply the same name for all */
	char name[64] = "";
	std::strcpy(name, object3Ds->at(0)->getRef()->toString().c_str());
	if (ImGui::InputText("Name", name, 64, InspectorInterface::INPUT_FLAGS)) {
		for (auto node : *object3Ds) {
			node->getRef()->setName(name);
		}
	}

	/* -- POSITION -- */
	{
		float pos[3] = {position3d.x, position3d.y, position3d.z};

		if (ImGui::InputFloat3("position : x y z", pos)) {
			ofVec3f disp((ofVec3f(pos[0], pos[1], pos[2]) - position3d));
			for (auto node : *object3Ds) {
				node->map([=](std::shared_ptr<Object3D> object3D) { object3D->getNode()->setPosition(object3D->getNode()->getPosition() + disp); });
			}
		}
	}

	/* -- ROTATION -- */
	{
		Hierarchy<Object3D>* node = object3Ds->at(0);
		HelpMarker(
			"Can rotate only one selected object. The rotation is blocked if more than one is selected. If the object has children, only the selected will rotate.");

		if (object3DsSize == 1) {
			ofVec3f rotation = node->getRef()->getNode()->getOrientationEulerDeg();

			float rot[3] = {rotation.x, rotation.y, rotation.z};
			if (ImGui::InputFloat3("rotation : x y z", rot)) {
				node->getRef()->getNode()->setGlobalOrientation(glm::quat(ofVec3f(rot[0], rot[1], rot[2]) * DEG_TO_RAD));
			}
		}
	}

	/* -- SCALE -- */
	{
		ofVec3f scale = object3Ds->at(0)->getRef()->getNode()->getScale();
		for (size_t i = 1; i < object3Ds->size(); i++) {
			scale += object3Ds->at(i)->getRef()->getNode()->getScale();
		}
		scale = scale / object3Ds->size();

		float sc[3] = {scale.x, scale.y, scale.z};
		if (ImGui::InputFloat3("scale : x y z", sc)) {
			for (auto node : *object3Ds) {
				node->map([=](std::shared_ptr<Object3D> object3D) { object3D->getNode()->setScale(ofVec3f(sc[0], sc[1], sc[2])); });
			}
		}
	}

	textureOptions(*object3Ds->at(0)->getRef());
	lightOptions(*object3Ds->at(0)->getRef());
}

/**
 * @brief Set up the options for textures in the inspector
 * - The filter applied
 * - The preview of the texture
 * 
 * @param object 
 */
void InspectorInterface::textureOptions(Object3D& object) {
	if (object.getType() != ObjectType::Model3D)
		return;

	ImGui::Separator();

	const char* items[] = {"No filter", "Sobel filter", "Grayscale", "Gaussian filter"};

	ImGui::Text("Texture filters:");
	if (ImGui::BeginListBox("##listbox")) {
		for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
			const bool is_selected = (object.getModel()->usingShader == n);
			if (ImGui::Selectable(items[n], is_selected))
				object.getModel()->usingShader = (ShaderType)n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	ofTexture* tex = object.getModel()->getTexture();
	if (tex->isAllocated())
		ofxImGui::AddImage(*tex, ofVec2f(ImGui::GetWindowWidth(), ImGui::GetWindowWidth() * (tex->getHeight() / tex->getWidth())));
}

void InspectorInterface::lightOptions(Object3D& object) {
	if (object.getType() != ObjectType::Light)
		return;

	ofLight* light		= (ofLight*)object.getNode();
	ImVec4	 lightColor = light->getDiffuseColor();
	ImGui::ColorEdit4("Light diffuse color", (float*)&lightColor);
	light->setDiffuseColor(lightColor);

	// light->setAreaLight();
	// light->setPointLight();
	// light->setSpotlight();
	// light->setDirectional();
	light->getType();
}

} // namespace ift3100