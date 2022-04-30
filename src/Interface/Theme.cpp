#include "Theme.h"

namespace ift3100 {
void Theme::setup() {
	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowMinSize = ImVec2(160, 65);
	style->FramePadding = ImVec2(4, 2);
	style->ItemSpacing = ImVec2(6, 2);
	style->ItemInnerSpacing = ImVec2(6, 4);
	style->Alpha = 1;
	style->WindowRounding = 4;
	style->FrameRounding = 4;
	style->IndentSpacing = 6.0f;
	style->ItemInnerSpacing = ImVec2(2, 4);
	style->ColumnsMinSpacing = 50.0f;
	style->GrabMinSize = 14.0f;
	style->GrabRounding = 0.0f;
	style->ScrollbarSize = 12.0f;
	style->ScrollbarRounding = 0.0f;
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(ofColor::darkGray);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(ofColor::darkGray);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(ofColor::darkGray);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(ofColor::gray, 60);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(ofColor::lightGray, 60);
}
} // namespace ift3100