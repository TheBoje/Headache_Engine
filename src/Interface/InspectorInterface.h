#ifndef INSPECTOR_INTERFACE_H
#define INSPECTOR_INTERFACE_H

#include <vector>

#include "Hierarchy.h"
#include "VectorPrimitive.h"
#include "Logger.h"

namespace ift3100 {
    class InspectorInterface {
        ImVec2 primitivePosition;
    
    public:
        void setup();
        void drawInspectorVectorPrimitive(std::vector<Hierarchy<VectorPrimitive> *> * vvp);
    };
}

#endif