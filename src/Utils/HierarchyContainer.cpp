#include "HierarchyContainer.h"
#include "Interface.h"

namespace ift3100 {
template <>
void HierarchyContainer<VectorPrimitive>::update() {
	if (!selected_nodes.empty()) {
		for (Hierarchy<VectorPrimitive>* selected_node : selected_nodes) {
			selected_node->map([=](std::shared_ptr<VectorPrimitive> vectorPrimitive) {
				vectorPrimitive->FILL = Interface::Get()->primitiveFill;
				vectorPrimitive->FILL_COLOR = Interface::Get()->primitiveFillColor;
				vectorPrimitive->STROKE_WIDTH = Interface::Get()->primitiveStrokeWidth;
				vectorPrimitive->STROKE_COLOR = Interface::Get()->primitiveStrokeColor;
			});
		}
	}
}

template <>
void HierarchyContainer<Object3D>::update() {
	_root.map([=](std::shared_ptr<Object3D> o) { o->update(); });
}
} // namespace ift3100