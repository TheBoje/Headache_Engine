#ifndef ANIMATOR_MANAGER_H
#define ANIMATOR_MANAGER_H

#include <vector>

#include "Object3D.h"
#include "ofxImGui.h"
#include "Animator.h"

namespace ift3100 {

class AnimatorManager {
	std::vector<Animator> _animators;

	void keyframesUI(std::vector<Keyframe>& keyframes);

public:
	AnimatorManager();
	~AnimatorManager() = default;

	void setup();
	void update();
	void drawUI();

	void addAnimator(std::shared_ptr<Object3D> target);

	Animator& getAnimator(size_t i);
};

} // namespace ift3100

#endif