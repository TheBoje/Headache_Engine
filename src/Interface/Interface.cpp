#include "Interface.h"

using namespace ift3100;

void Interface::setup() {
    _gui.setup();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    primitiveStrokeWidth = DEFAULT_STROKE_WIDTH;
    primitiveStrokeColor = ofColor::white;
    primitiveStrokeColor.w = 1;
    primitiveFillColor = ofColor::gray;
    primitiveFillColor.w = 1;
    primitiveFill = true;
    mouseAction = None;

    ofLog() << "<interface::setup> primitiveFillColor:" << primitiveFillColor;
}

void Interface::draw() {
    _gui.begin();
    ImGui::Begin("Main menu");
    {
        if (ImGui::CollapsingHeader("Debug")) {
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        if (ImGui::CollapsingHeader("Hierarchy")) {
            ImGui::Text("This is empty... for now");
        }

        if (ImGui::CollapsingHeader("Drawing")) {
            int drawModeTmp = drawMode;
            ImGui::RadioButton("Line", &drawModeTmp, 0); ImGui::SameLine();
            ImGui::RadioButton("Rectangle", &drawModeTmp, 1); ImGui::SameLine();
            ImGui::RadioButton("Ellipse", &drawModeTmp, 2); ImGui::SameLine();
            drawMode = InterfaceUtils::intToPrimitiveType(drawModeTmp);

            ImGui::SliderFloat("Stroke Width", &primitiveStrokeWidth, 0.0f, 100.0f);
            ImGui::ColorEdit4("Stroke Color", (float*)&primitiveStrokeColor);
            ImGui::ColorEdit4("Fill Color", (float*)&primitiveFillColor);
            ImGui::Checkbox("Enable fill", &primitiveFill);

            bool isDrawPrimitive = mouseAction == DrawPrimitive;
            if (ImGui::Checkbox("Draw", &isDrawPrimitive)) {
                if (isDrawPrimitive) mouseAction = DrawPrimitive;
                else mouseAction = None;
            }

            if (ImGui::Button("Undo")) {
                primitiveUndo = true;
            } ImGui::SameLine();
            if (ImGui::Button("Redo")) {
                primitiveRedo = true;
            }
        }

    }
    _gui.end();

    
}

void Interface::button_pressed()
{
    ofLog() << "<button pressed>";
}