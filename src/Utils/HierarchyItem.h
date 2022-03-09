#ifndef HIERARCHY_ITEM_H
#define HIERARCHY_ITEM_H

namespace ift3100 {

    /**
     * @brief Interface needed for Hierarchy in order to be used in
     * the GUI.
     * @see Hierarchy::drawGUIHierarchy()
     */
    class HierarchyItem {
    public:
        virtual std::string toString() const = 0;
    };
}

#endif