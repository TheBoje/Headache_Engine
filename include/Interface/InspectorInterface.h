#ifndef INSPECTOR_INTERFACE_H
#define INSPECTOR_INTERFACE_H

#include <vector>

#include "Hierarchy.h"
#include "Logger.h"
#include "VectorPrimitive.h"
#include "Object3D.h"

namespace ift3100 {
class InspectorInterface {
	ImVec2	primitivePosition;
	ofVec3f position3d;

public:
	void setup();
	void drawInspectorVectorPrimitive(std::vector<Hierarchy<VectorPrimitive>*>* vvp);
	void drawInspector3d(std::vector<Hierarchy<Object3D>*>* v3d);
};
} // namespace ift3100

#endif