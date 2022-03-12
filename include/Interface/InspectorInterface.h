#ifndef INSPECTOR_INTERFACE_H
#define INSPECTOR_INTERFACE_H

#include <vector>

#include "Hierarchy.h"
#include "Logger.h"
#include "VectorPrimitive.h"

namespace ift3100 {
class InspectorInterface {
	ImVec2 primitivePosition;

public:
	void setup();
	void drawInspectorVectorPrimitive(std::vector<Hierarchy<VectorPrimitive>*>* vvp);
};
} // namespace ift3100

#endif