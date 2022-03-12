#ifndef THEME_H
#define THEME_H

#include "BaseTheme.h"
#include "ofMain.h"

namespace ift3100 {
/**
     * Define ImGui style, overridding default theme.
    */
class Theme : public ofxImGui::BaseTheme {
public:
	void setup() override;
};
} // namespace ift3100

#endif //! THEME_H