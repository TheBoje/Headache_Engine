#include "AnimatorManager.h"
#include "InspectorInterface.h"

namespace ift3100 {

AnimatorManager::AnimatorManager() { }

void AnimatorManager::addAnimator(ofNode* target) {
	_animators.emplace_back(Animator(target));
}

Animator& AnimatorManager::getAnimator(size_t i) {
	assert(i >= 0 && i < _animators.size());
	return _animators.at(i);
}

void AnimatorManager::setup() { }

void AnimatorManager::update() {
	for (Animator& anim : _animators) {
		anim.update();
	}
}

void AnimatorManager::drawUI() {
	ImGui::Begin("Animator manager");
	{
		for (size_t i = 0; i < _animators.size(); i++) {
			std::string name = ("Anim " + std::to_string(i));

			if (ImGui::CollapsingHeader(name.c_str())) {
				if (_animators[i].isPaused()) {
					if (ImGui::Button("Resume")) {
						_animators[i].resume();
					}
				} else {
					if (ImGui::Button("Pause")) {
						_animators[i].pause();
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Reset")) {
					_animators[i].reset();
				}
				ImGui::Separator();

				keyframesUI(_animators[i].getKeyframes());

				if (ImGui::Button("Add keyframe")) {
					_animators[i].addKeyframe(ofVec3f(0, 0, 0), ofVec3f(0, 0, 0), 0);
				}
			}
		}
	}
}

void AnimatorManager::keyframesUI(std::vector<Keyframe>& keyframes) {
	for (size_t key = 0; key < keyframes.size(); key++) {
		char posx[64] = "";
		std::strcpy(posx, std::to_string(keyframes[key].position.x).c_str());
		if (ImGui::InputText(("x" + std::to_string(key)).c_str(), posx, 64, InspectorInterface::INPUT_DECIMAL_FLAGS)) {
			keyframes[key].position.x = atof(posx);
		}

		char posy[64] = "";
		std::strcpy(posy, std::to_string(keyframes[key].position.y).c_str());
		if (ImGui::InputText(("y" + std::to_string(key)).c_str(), posy, 64, InspectorInterface::INPUT_DECIMAL_FLAGS)) {
			keyframes[key].position.y = atof(posy);
		}

		char posz[64] = "";
		std::strcpy(posz, std::to_string(keyframes[key].position.z).c_str());
		if (ImGui::InputText(("z" + std::to_string(key)).c_str(), posz, 64, InspectorInterface::INPUT_DECIMAL_FLAGS)) {
			keyframes[key].position.z = atof(posz);
		}

		char frame[64] = "";
		std::strcpy(frame, std::to_string(keyframes[key].frame).c_str());
		if (ImGui::InputText(("frame" + std::to_string(key)).c_str(), frame, 64, InspectorInterface::INPUT_DECIMAL_FLAGS)) {
			keyframes[key].frame = atof(frame);
		}
	}
}

} // namespace ift3100