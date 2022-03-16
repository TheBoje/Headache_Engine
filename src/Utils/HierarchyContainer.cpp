#include "HierarchyContainer.h"
#include "Interface.h"

namespace ift3100 {
template <>
void HierarchyContainer<VectorPrimitive>::update() {
	if (!selected_nodes.empty()) {
		for (Hierarchy<VectorPrimitive>* selected_node : selected_nodes) {
			selected_node->map([=](std::shared_ptr<VectorPrimitive> vectorPrimitive) {
				vectorPrimitive->FILL		  = Interface::Get()->primitiveFill;
				vectorPrimitive->FILL_COLOR	  = Interface::Get()->primitiveFillColor;
				vectorPrimitive->STROKE_WIDTH = Interface::Get()->primitiveStrokeWidth;
				vectorPrimitive->STROKE_COLOR = Interface::Get()->primitiveStrokeColor;
			});
		}
	}
}
} // namespace ift3100