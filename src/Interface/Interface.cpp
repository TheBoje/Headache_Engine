#include "Interface.h"
#include "ofImage.h"
#include "ImageUtils.h"

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

    isHistComputed = false;


    std::shared_ptr<std::string> s0(new std::string("salut"));
    std::shared_ptr<std::string> s1(new std::string("salut 1"));
    std::shared_ptr<std::string> s2(new std::string("aurevoir 1"));
    std::shared_ptr<std::string> s3(new std::string("salut 2"));
    std::shared_ptr<std::string> s4(new std::string("salut 3"));


    tree = new Hierarchy<std::string>(s0);
    tree->addChild(s1);
    tree->at(0)->addChild(s2);
    tree->addChild(s3);
    tree->addChild(s4);

    ofLog() << "<interface::setup> primitiveFillColor:" << primitiveFillColor;
}

float getter(void * data, int index) {
    return (float)((int*)data)[index];
}

void Interface::loadImage(std::string path) {
    image.load(path);
    textureSourceID = _gui.loadTexture(textureSource, path);
}

void Interface::imageInterface() {
    if(ImGui::Button("compute histogram") && image.isAllocated()){
        _rgb = ImageUtils::computeHistRGB(image);
        isHistComputed = true;
    }

    if(isHistComputed) {
        ImGui::PlotHistogram("R", &getter, _rgb[0], 256, 0, NULL, 0.0f, 70000.0f, ImVec2(0,80)); ImGui::NewLine();
        ImGui::PlotHistogram("G", &getter, _rgb[1], 256, 0, NULL, 0.0f, 70000.0f, ImVec2(0,80)); ImGui::NewLine();
        ImGui::PlotHistogram("B", &getter, _rgb[2], 256, 0, NULL, 0.0f, 70000.0f, ImVec2(0,80)); ImGui::NewLine();
    }
    
    if(image.isAllocated()) 
        ImGui::Image((ImTextureID)(uintptr_t)textureSourceID, ImVec2(textureSource.getWidth()/ 4, textureSource.getHeight()/4));
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
            imageInterface();
        }

        if(ImGui::CollapsingHeader("tree")) {
            tree->drawGUIHierarchy();
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