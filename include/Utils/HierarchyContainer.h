#ifndef HIERARCHY_CONTAINER_H
#define HIERARCHY_CONTAINER_H

#include "Hierarchy.h"
#include "Object3D.h"
#include "VectorPrimitive.h"
#include "ofxImGui.h"

#include <memory>

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

	~HierarchyContainer() {
		selected_nodes.clear();
	}

	void setRoot(std::shared_ptr<T> ref) {
		_root.setRef(ref);
	}

	bool isRoot(const Hierarchy<T>& h) {
		return h == _root;
	}

	void clear() {
		_root.clear();
	}

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
		IFT_LOG << "added item to hierarchy";
	}

	void addChildren(std::vector<std::shared_ptr<T>>& children, std::shared_ptr<T> parent) {
		if (selected_nodes.empty()) {
			_root.addChild(parent, CURRENT_INDEX++);
			int p_index = _root.getChildrenSize() - 1;
			for (std::shared_ptr<T>& child : children) {
				_root.at(p_index)->addChild(child, CURRENT_INDEX++);
			}
		} else {
			selected_nodes[0]->addChild(parent, CURRENT_INDEX++);
			int p_index = _root.getChildrenSize() - 1;
			for (std::shared_ptr<T>& child : children) {
				selected_nodes[0]->at(p_index)->addChild(child, CURRENT_INDEX++);
			}
		}
	}

	void mapChildren(std::function<void(std::shared_ptr<T>)> func) {
		for (size_t i = 0; i < _root.getChildrenSize(); i++) {
			_root.at(i)->map(func);
		}
	}

	void drawUI() {
		if (ImGui::TreeNodeEx((void*)(intptr_t)0, ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth, "root", 0)) {
			for (size_t i = 0; i < _root.getChildrenSize(); i++) {
				_root.at(i)->drawUI(selected_nodes);
			}
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
void HierarchyContainer<VectorPrimitive>::update();

template <>
void HierarchyContainer<Object3D>::update();
} // namespace ift3100

#endif