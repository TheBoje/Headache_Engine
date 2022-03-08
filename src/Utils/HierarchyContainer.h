#ifndef HIERARCHY_CONTAINER_H
#define HIERARCHY_CONTAINER_H

#include "ofxImGui.h"
#include "Hierarchy.h"

namespace ift3100
{
    /**
     * @brief Container for hierarchy containing the root and
     * the selected nodes of the hierarchy.
     * 
     * @tparam T 
     */
    template <class T>
    class HierarchyContainer
    {
    private:        
        int CURRENT_INDEX = 1;

        Hierarchy<T> _root;

    public:
        std::vector<Hierarchy<T>*> selected_nodes;

        HierarchyContainer() : _root(Hierarchy<T>()) {};
        HierarchyContainer(std::shared_ptr<T> root_ref) : _root(Hierarchy<T>(root_ref)) {}
        ~HierarchyContainer() = default;

        void setRoot(std::shared_ptr<T> ref) {
            _root.setRef(ref);
        }

        bool isRoot(const Hierarchy<T>& h) {
            return h == _root;
        }

        void clear() {
            _root.clear();
        }

        /**
         * @brief Add child to the root and attributing him
         * a new index.
         * 
         * @param ref 
         */
        void addChild(std::shared_ptr<T> ref) {
            
            if(selected_nodes.empty())
                _root.addChild(ref, CURRENT_INDEX);
            else
                selected_nodes[0]->addChild(ref, CURRENT_INDEX);
            CURRENT_INDEX++;
        }

        void drawGUI() {
            _root.drawGUIHierarchy(selected_nodes);
        }
    };
} // namespace ift3100


#endif