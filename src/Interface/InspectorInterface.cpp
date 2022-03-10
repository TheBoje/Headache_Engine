#include "InspectorInterface.h"

using namespace ift3100;

void InspectorInterface::setup() {
    primitivePosition = ImVec2(0, 0);
}

/**
 * @brief Draw the inspector for the vector primitive including 
 * - position x y
 * - name
 * 
 * When multiple nodes are selected, compute the transformation from 
 * the mean of the selected nodes position and apply it to every nodes.
 * 
 * @param vvp selected nodes from interface for vector primitive hierarchy container 
 */
void InspectorInterface::drawInspectorVectorPrimitive(std::vector<Hierarchy<VectorPrimitive> *> * vvp) {
    std::size_t vvp_size = vvp->size(); 
    ImVec2 sum(0, 0);

    // get the mean of all selected primitive position
    for(auto node : *vvp) {
        sum = sum + (node->getRef()->POSITION_1 + node->getRef()->POSITION_2)/2;
    }
    primitivePosition.x = sum.x / vvp_size;
    primitivePosition.y = sum.y / vvp_size;

    /* X position input decimal */
    char bufx[64] = "";
    strcpy(bufx, std::to_string(primitivePosition.x).c_str());    
    if(ImGui::InputText("x", bufx, 64, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue)) {
        IFT_LOG << "Change x value to " << bufx;
        float disp = atof(bufx) - primitivePosition.x;
        
        for(auto node : *vvp) {
            node->map([=](std::shared_ptr<VectorPrimitive> vp) {
                vp->POSITION_1.x += disp;
                vp->POSITION_2.x += disp;
            });
        }
    }
    
    /* Y position input decimal */
    char bufy[64] = "";
    strcpy(bufy, std::to_string(primitivePosition.y).c_str());
    if(ImGui::InputText("y", bufy, 64, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue)) {
        IFT_LOG << "Change y value to " << bufy;
        float disp = atof(bufx) - primitivePosition.y;
        
        for(auto node : *vvp) {
            node->map([=](std::shared_ptr<VectorPrimitive> vp) {
                vp->POSITION_1.y += disp;
                vp->POSITION_2.y += disp;
            });
        }
    }

    /* NAME primitive input */
    /* if several nodes are selected, apply the same name for all */
    char name[64] = "";
    strcpy(name, vvp->at(0)->getRef()->NAME.c_str());
    if(ImGui::InputText("Name", name, 64, ImGuiInputTextFlags_EnterReturnsTrue)) {
        for(auto node : *vvp) {
            node->getRef()->NAME = name;
        }
    }
}