#ifndef HIERARCHY_CONTAINER_H
#define HIERARCHY_CONTAINER_H

#include "Hierarchy.h"
#include "Interface.h"
#include "VectorPrimitive.h"
#include "ofxImGui.h"

namespace ift3100 {
/**
     * @brief Container for hierarchy containing the root and
     * the selected nodes of the hierarchy.
     *
     * @tparam T
     */
template <class T>
class HierarchyContainer {
private:
	int CURRENT_INDEX = 1;

	Hierarchy<T> _root;

public:
	std::vector<Hierarchy<T>*> selected_nodes;

	HierarchyContainer()
		: _root(Hierarchy<T>()) {};
	HierarchyContainer(std::shared_ptr<T> root_ref)
		: _root(Hierarchy<T>(root_ref)) { }

	HierarchyContainer(const HierarchyContainer<T>& cpy)
		: _root(Hierarchy<T>(cpy._root)) { }

	~HierarchyContainer() { selected_nodes.clear(); }

	void setRoot(std::shared_ptr<T> ref) { _root.setRef(ref); }

	bool isRoot(const Hierarchy<T>& h) { return h == _root; }

	void clear() { _root.clear(); }

	void update() { }

	/**
         * @brief Add child to the root and attributing him
         * a new index.
         *
         * @param ref
         */
	void addChild(std::shared_ptr<T> ref) {
		if (selected_nodes.empty())
			_root.addChild(ref, CURRENT_INDEX);
		else
			selected_nodes[0]->addChild(ref, CURRENT_INDEX);
		CURRENT_INDEX++;
	}

	void mapChildren(std::function<void(std::shared_ptr<T>)> func) {
		for (int i = 0; i < _root.getChildrenSize(); i++) { _root.at(i)->map(func); }
	}

	void drawUI() {
		if (ImGui::TreeNodeEx((void*)(intptr_t)0, ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth, "root", 0)) {
			for (int i = 0; i < _root.getChildrenSize(); i++) { _root.at(i)->drawUI(selected_nodes); }
			ImGui::TreePop();
		}
	}

	/**
         * @brief Flatten the tree in a vector of hierarchy
         *
         * @return std::vector<Hierarchy<T>*>
         */
	std::vector<Hierarchy<T>*> flatten() {
		std::vector<Hierarchy<T>*> flat {};
		_root.flatten(flat);
		return flat;
	}

	/**
         * @brief Flatten the tree in a vector of T class in order
         * to get the object stored in the hierarchy
         *
         * @return std::vector<std::shared_ptr<T>>
         */
	std::vector<std::shared_ptr<T>> flattenRef() {
		std::vector<std::shared_ptr<T>> flatRef {};
		_root.flattenRef(flatRef);
		return flatRef;
	}

	HierarchyContainer<T>& operator=(const HierarchyContainer<T>& other) {
		if (&other != this) {
			_root = other._root;
		}
		return *this;
	}
};

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

#endif