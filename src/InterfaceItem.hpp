#ifndef INTERFACE_ITEM_HPP
#define INTERFACE_ITEM_HPP

namespace ift3100 {
    /*
    This class is an interface to mondel onto to add an UI element.
    The object implementing `InterfaceItem` needs to call the proper 
    `ImGui` methods to draw itself.
    */
    class InterfaceItem {
        public:
            /*
                Must implement `ImGui` calls to draw itself. 
                This method is called EACH FRAME, so please limit as much as
                possible every expensive operations.
            */
            virtual void draw() = 0;
    };
}

#endif // !INTERFACE_ITEM_HPP