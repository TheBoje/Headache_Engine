#include "Interface.h"
#include "Application.h"

using namespace ift3100;

Interface::Interface(Application & _application) : application(_application) {}

void Interface::setup() {
    _gui.setup();
    _gui.setTheme(new Theme());
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    primitiveStrokeWidth = DEFAULT_STROKE_WIDTH;
    primitiveStrokeColor = ofColor::white;
    primitiveStrokeColor.w = 1;
    primitiveFillColor = ofColor::gray;
    primitiveFillColor.w = 1;
    primitiveFill = true;
    mouseAction = None;

    isHistComputed = false;

    IFT_LOG << "done";
}

float getter(void * data, int index) {
    return (float)((int*)data)[index];
}

void Interface::loadImage(std::string path) {
    image.load(path);
    textureSourceID = _gui.loadTexture(textureSource, path);
}

void Interface::imageUI() {
    if(ImGui::Button("compute histogram") && image.isAllocated()){
        _rgb = ImageUtils::computeHistRGB(image);
        isHistComputed = true;
    }

    if(isHistComputed) {
        ImGui::PlotHistogram("R", &getter, _rgb[0], 256, 0, NULL, 0.0f, 70000.0f, ImVec2(0,80)); ImGui::NewLine();
        ImGui::PlotHistogram("G", &getter, _rgb[1], 256, 0, NULL, 0.0f, 70000.0f, ImVec2(0,80)); ImGui::NewLine();
        ImGui::PlotHistogram("B", &getter, _rgb[2], 256, 0, NULL, 0.0f, 70000.0f, ImVec2(0,80)); ImGui::NewLine();
    }

    ImGui::InputText("Output file name", imageRenderName, IM_ARRAYSIZE(imageRenderName));
    if (ImGui::Button("Export")) {
        application.exportRender(std::string(imageRenderName));
    }

    if(image.isAllocated())
        ImGui::Image((ImTextureID)(uintptr_t)textureSourceID, ImVec2(textureSource.getWidth()/ 4, textureSource.getHeight()/4));
}

void Interface::drawingUI() {
    const char* items[] = {"Point", "Line", "Rectangle", "Ellipse", "Triangle", "Cross", "Star"};
    int drawModeCurrentIndex = drawMode;
    if (ImGui::BeginCombo("Primitive type", items[drawModeCurrentIndex])) {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
            bool is_selected = (drawModeCurrentIndex == n);
            if (ImGui::Selectable(items[n], is_selected))
                drawModeCurrentIndex = n;
            if (is_selected)
                ImGui::SetItemDefaultFocus();
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
        application.rendererUndo();
    } ImGui::SameLine();
    if (ImGui::Button("Redo")) {
        application.rendererRedo();
    }
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

        if(ImGui::CollapsingHeader("Image")) {
            imageUI();
        }

        if(ImGui::CollapsingHeader("Tree")) {
            application.renderer.hierarchyPrimitives.drawUI();
        }

        if (ImGui::CollapsingHeader("Drawing")) {
            drawingUI();
        }
    }
    _gui.end();
}