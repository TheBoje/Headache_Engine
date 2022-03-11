#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "HierarchyItem.h"
#include <string>

namespace ift3100 {
    class Object3D : public HierarchyItem {
        std::string _name;
    
    public:
        Object3D(std::string name) : _name(name) {}
        std::string toString() { return _name; }
    };
}

#endif