#ifndef INSPECTOR_INTERFACE_H
#define INSPECTOR_INTERFACE_H

#include <vector>

#include "Hierarchy.h"
#include "Logger.h"
#include "Object3D.h"
#include "VectorPrimitive.h"

namespace ift3100 {
class InspectorInterface {
	ImVec2	primitivePosition;
	ofVec3f position3d;

public:
	// TODO: les mettre dans constant.h
	static const ImGuiInputTextFlags INPUT_FLAGS		 = ImGuiInputTextFlags_EnterReturnsTrue;
	static const ImGuiInputTextFlags INPUT_DECIMAL_FLAGS = INPUT_FLAGS | ImGuiInputTextFlags_EnterReturnsTrue;

	void setup();
	void drawInspectorVectorPrimitive(std::vector<Hierarchy<VectorPrimitive>*>* vectorPrimitives);
	void drawInspector3d(std::vector<Hierarchy<Object3D>*>* object3Ds);

	void textureOptions(Object3D& object3Ds);
	void lightOptions(Object3D& light);
};
} // namespace ift3100

#endif