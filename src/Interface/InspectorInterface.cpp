#include "InspectorInterface.h"

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

void InspectorInterface::setup() { primitivePosition = ImVec2(0, 0); }

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
void InspectorInterface::drawInspectorVectorPrimitive(std::vector<Hierarchy<VectorPrimitive>*>* vvp) {
	std::size_t vvp_size = vvp->size();
	ImVec2		sum(0, 0);

	// get the mean of all selected primitive position
	for (auto node : *vvp) { sum = sum + (node->getRef()->POSITION_1 + node->getRef()->POSITION_2) / 2; }
	primitivePosition.x = sum.x / vvp_size;
	primitivePosition.y = sum.y / vvp_size;

	/* NAME primitive input */
	/* if several nodes are selected, apply the same name for all */
	char name[64] = "";
	strcpy(name, vvp->at(0)->getRef()->NAME.c_str());
	if (ImGui::InputText("Name", name, 64, InspectorInterface::INPUT_FLAGS)) {
		for (auto node : *vvp) { node->getRef()->NAME = name; }
	}

	/* X position input decimal */
	char bufx[64] = "";
	strcpy(bufx, std::to_string(primitivePosition.x).c_str());
	if (ImGui::InputText("x", bufx, 64, InspectorInterface::INPUT_DECIMAL_FLAGS)) {
		IFT_LOG << "Change x value to " << bufx;
		float disp = atof(bufx) - primitivePosition.x;

		for (auto node : *vvp) {
			node->map([=](std::shared_ptr<VectorPrimitive> vp) {
				vp->POSITION_1.x += disp;
				vp->POSITION_2.x += disp;
			});
		}
	}

	/* Y position input decimal */
	char bufy[64] = "";
	strcpy(bufy, std::to_string(primitivePosition.y).c_str());
	if (ImGui::InputText("y", bufy, 64, InspectorInterface::INPUT_DECIMAL_FLAGS)) {
		IFT_LOG << "Change y value to " << bufy;
		float disp = atof(bufy) - primitivePosition.y;

		for (auto node : *vvp) {
			node->map([=](std::shared_ptr<VectorPrimitive> vp) {
				vp->POSITION_1.y += disp;
				vp->POSITION_2.y += disp;
			});
		}
	}
}

void InspectorInterface::drawInspector3d(std::vector<Hierarchy<Object3D>*>* v3d) {
	std::size_t v3d_size = v3d->size();
	ofVec3f		sum(0);

	// get the mean of all selected primitive position
	for (auto node : *v3d) {
		if (node->getRef()->getNode() == nullptr) {
			return;
		}
		sum += sum + node->getRef()->getNode()->getPosition();
	}
	position3d.x = sum.x / v3d_size;
	position3d.y = sum.y / v3d_size;
	position3d.z = sum.z / v3d_size;

	/* if several nodes are selected, apply the same name for all */
	char name[64] = "";
	strcpy(name, v3d->at(0)->getRef()->toString().c_str());
	if (ImGui::InputText("Name", name, 64, InspectorInterface::INPUT_FLAGS)) {
		for (auto node : *v3d) { node->getRef()->setName(name); }
	}

	/* -- POSITION -- */
	ImGui::Text("Position");
	/* X position input decimal */
	char bufx[64] = "";
	strcpy(bufx, std::to_string(position3d.x).c_str());
	if (ImGui::InputText("x", bufx, 64, InspectorInterface::INPUT_DECIMAL_FLAGS)) {
		IFT_LOG << "Change x value to " << bufx;
		float disp = atof(bufx) - position3d.x;

		for (auto node : *v3d) {
			node->map([=](std::shared_ptr<Object3D> vp) { vp->getNode()->setPosition(vp->getNode()->getPosition() + (ofVec3f(1, 0, 0) * disp)); });
		}
	}

	/* Y position input decimal */
	char bufy[64] = "";
	strcpy(bufy, std::to_string(position3d.y).c_str());
	if (ImGui::InputText("y", bufy, 64, InspectorInterface::INPUT_DECIMAL_FLAGS)) {
		float disp = atof(bufy) - position3d.y;

		for (auto node : *v3d) {
			node->map([=](std::shared_ptr<Object3D> vp) { vp->getNode()->setPosition(vp->getNode()->getPosition() + (ofVec3f(0, 1, 0) * disp)); });
		}
	}

	/* Z position input decimal */
	char bufz[64] = "";
	strcpy(bufz, std::to_string(position3d.z).c_str());
	if (ImGui::InputText("z", bufz, 64, InspectorInterface::INPUT_DECIMAL_FLAGS)) {
		IFT_LOG << "Change z value to " << bufz;
		float disp = atof(bufz) - position3d.z;

		for (auto node : *v3d) {
			node->map([=](std::shared_ptr<Object3D> vp) { vp->getNode()->setPosition(vp->getNode()->getPosition() + (ofVec3f(0, 0, 1) * disp)); });
		}
	}

	/* -- ROTATION -- */
	ImGui::Text("Rotation");
	HelpMarker(
		"Can rotate only one selected object. The rotation is blocked if more than one is selected. If the object has children, only the selected will rotate.");

	if (v3d_size != 1)
		return;

	Hierarchy<Object3D>* node	  = v3d->at(0);
	ofVec3f				 rotation = node->getRef()->getNode()->getOrientationEulerDeg();

	/* X position input decimal */
	char rotx[64] = "";
	strcpy(rotx, std::to_string(rotation.x).c_str());
	if (ImGui::InputText("rot x", rotx, 64, InspectorInterface::INPUT_DECIMAL_FLAGS)) {
		IFT_LOG << "Change rotation x value to " << rotx;
		node->getRef()->getNode()->setGlobalOrientation(glm::quat(ofVec3f(atof(rotx), rotation.y, rotation.z) * DEG_TO_RAD));

		// node->map([=](std::shared_ptr<Object3D> vp) { vp->getNode()->rotateDeg(atof(rotx), ofVec3f(1, 0, 0)); });
	}

	/* Y position input decimal */
	char roty[64] = "";
	strcpy(roty, std::to_string(rotation.y).c_str());
	if (ImGui::InputText("rot y", roty, 64, InspectorInterface::INPUT_DECIMAL_FLAGS)) {
		IFT_LOG << "Change rotation y value to " << roty;
		node->getRef()->getNode()->setGlobalOrientation(glm::quat(ofVec3f(rotation.x, atof(roty), rotation.z) * DEG_TO_RAD));
	}

	/* Z position input decimal */
	char rotz[64] = "";
	strcpy(rotz, std::to_string(rotation.z).c_str());
	if (ImGui::InputText("rot z", rotz, 64, InspectorInterface::INPUT_DECIMAL_FLAGS)) {
		IFT_LOG << "Change rotation z value to " << rotz;
		node->getRef()->getNode()->setGlobalOrientation(glm::quat(ofVec3f(rotation.x, rotation.y, atof(rotz)) * DEG_TO_RAD));
	}
}
} // namespace ift3100