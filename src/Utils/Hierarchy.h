#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <vector>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include <functional>

#include "ofxImGui.h"

namespace ift3100 {



    /**
     * A simple tree representing hierarchy.
     * The toString() method of the class T needs to be implemented
     * @param T
     */
    template <class T>
    class Hierarchy {        

        static const ImGuiTreeNodeFlags NODE_FLAGS = 
             ImGuiTreeNodeFlags_OpenOnArrow | 
             ImGuiTreeNodeFlags_OpenOnDoubleClick | 
             ImGuiTreeNodeFlags_SpanAvailWidth;

        std::shared_ptr<T> _ref;

        Hierarchy<T> * _parent;
        std::vector<Hierarchy<T> *> _children;

        int _index;

    public:
        Hierarchy() : _ref(nullptr), _index(0) {}

        Hierarchy(std::shared_ptr<T> ref, int index, Hierarchy<T> * parent = nullptr) : 
            _ref(std::move(ref)), 
            _index(index),
            _parent(parent) 
        {}

        Hierarchy(const Hierarchy<T> &cpy) : _ref(cpy._ref), _index(cpy._index) {
            int children_size = cpy._children.size();
            _children.reserve(children_size);

            for (Hierarchy<T> *child : cpy._children) {
                _children.push_back(new Hierarchy<T>(*child));
            }
        }

        ~Hierarchy() {           
            for(Hierarchy<T> *child : _children) {
                delete child;
            }
            
            if(_parent != nullptr) {
                int i;
                for(i = 0; i < _parent->_children.size(); i++) {
                    if(this == _parent->_children[i])
                        break;
                }

                _parent->_children.erase(_parent->_children.begin() + i);
                ofLog() << "<Hierarchy::drawGUIHierarchy> parent children size " << _parent->_children.size();
            }
            

            _parent = nullptr;
            _ref = nullptr; 

            ofLog() << "<Hierarchy::drawGUIHierarchy> delete node " << _index;
        }

        /**
         * Will add a child to the current node
         * @param child
         */
        void addChild(std::shared_ptr<T> child, int index) {
            _children.push_back(new Hierarchy<T>(child, index, this));
        }

        Hierarchy<T> &operator=(const Hierarchy<T> &other) {
            if(other != *this) {
                for(Hierarchy<T> *child : _children) {
                    delete child;
                }

                int children_size = other._children.size();
                _children.reserve(children_size);

                for (Hierarchy<T> *child : other._children) {
                    _children.push_back(new Hierarchy<T>(*child));  
                }
            }
        }

        bool operator==(const Hierarchy<T>& h) const {
            return h->_index == this->_index;
        }   

        /**
         * @param index
         * @return the index-th child of the node
         * @throw out_of_range
         */
        Hierarchy<T> * at(std::size_t index) {
            try {
                return _children.at(index);
            } catch (std::out_of_range & e) {
                throw e;
            }
        }

        /**
         * @brief get the number of child for the current node
         * 
         * @return std::size_t 
         */
        std::size_t getChildrenSize() { return this->_children.size(); }

        /**
         * @brief Will draw the hierarchy in the interface (need to be wrapped)
         * around gui.begin() and gui.end() from ofxImGui. Will display the name
         * of the name using the operator<< of the class T
         * @see Interface.cpp
         */
        void drawGUIHierarchy(std::vector<Hierarchy<T>*> & selected) {
            // check if the node is selected and apply the flag if true
            ImGuiTreeNodeFlags flags = NODE_FLAGS;
            int selected_index = -1;

            for(int i = 0; i < selected.size(); i++) {
                if(selected[i]->_index == _index) {
                    flags |= ImGuiTreeNodeFlags_Selected;
                    selected_index = i;
                    break;
                }
            }

            bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)_index, flags, _ref->toString().c_str(), _index); 
            if(node_open) {

                if (ImGui::IsItemClicked()) {
                    
                    if(selected_index == -1) {
                        selected.push_back(this);
                        ofLog() << "<Hierarchy::drawGUIHierarchy> select node " << _index;
                    } else if(ImGui::GetIO().KeyCtrl) { 
                        selected.erase(selected.begin() + selected_index);
                        ofLog() << "<Hierarchy::drawGUIHierarchy> unselect node " << _index;
                    }
                }

                for(auto child : _children) {
                    child->drawGUIHierarchy(selected);
                }                

                ImGui::TreePop();
            }

        }

        /**
         * Will apply "func" method to the node value and all its children (grand-children etc...)
         * Usefull for parent transformation.
         * @param func
         */
        void map(std::function<void(std::shared_ptr<T>)> func) {
            func(_ref);
            for(auto child : _children) {
                child->map(func);
            }
        }

        void setRef(std::shared_ptr<T> ref) {
            _ref = ref;
        }

        std::shared_ptr<T> getRef() {
            return _ref;
        }

        /**
         * Move the index-th element of the node to the dest node.
         * Will erase the branch from the node children and add it 
         * to the end of the dest node.
         * 
         * @param index 
         * @param dest 
         */
        void move(std::size_t index, Hierarchy<T> * dest) {
            dest->addChild(_children[index]);

            // Erase the child from the vector, do not desallocate it
            _children.erase(_children.begin() + index);
        }

        /**
         * Swap two children between them. Only available at the same depth.
         * 
         * @param srcIndex 
         * @param destIndex 
         */
        void swap(std::size_t srcIndex, std::size_t destIndex) {
            if(srcIndex == destIndex) return;

            std::iter_swap(_children.begin() + srcIndex, _children.begin() + destIndex);
        }
    };
}

#endif