#ifndef INTERFACE_ITEM_HPP
#define INTERFACE_ITEM_HPP

#include "Hierarchy.h"

namespace ift3100 {
    /**
     * This class is an interface to mondel onto to add an UI element.
     * The object implementing `InterfaceItem` needs to call the proper 
     * `ImGui` methods to draw itself.
    */
    class InterfaceItem {
        public:
            /**
             * Must implement `ImGui` calls to draw itself. 
             * method is called EACH FRAME, so please limit as much as
             * every expensive operations.
            */
            virtual void draw() = 0;
    };

    /**
     * Override Hierarchy.map for InterfaceItem to draw them.
     * @param func ignored parameter
    */
    void Hierarchy<InterfaceItem>::map(void (*func)(InterfaceItem*) = nullptr) {
            _ref->draw();
            for(auto child : _children) {
                child->map(nullptr);
        }
    }
}

#endif // !INTERFACE_ITEM_HPP