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
            // ImGui::RadioButton("Line", &drawModeTmp, 0); ImGui::SameLine();
            // ImGui::RadioButton("Rectangle", &drawModeTmp, 1); ImGui::SameLine();
            // ImGui::RadioButton("Ellipse", &drawModeTmp, 2); ImGui::SameLine();
            const char* items[] = {"Point", "Line", "Rectangle", "Ellipse", "Triangle", "Cross", "Star"};
            int drawModeCurrentIndex = drawMode;
            if (ImGui::BeginCombo("Primitive type", items[drawModeCurrentIndex])) {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                bool is_selected = (drawModeCurrentIndex == n);
                if (ImGui::Selectable(items[n], is_selected))
                    drawModeCurrentIndex = n;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
            }
            ImGui::EndCombo();
            }
            drawMode = InterfaceUtils::intToPrimitiveType(drawModeCurrentIndex);

            ImGui::SliderFloat("Stroke Width", &primitiveStrokeWidth, 0.0f, 20.0f);
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