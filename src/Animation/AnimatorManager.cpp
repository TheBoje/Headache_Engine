#include "AnimatorManager.h"
#include "InspectorInterface.h"

namespace ift3100 {

AnimatorManager::AnimatorManager() { }

void AnimatorManager::addAnimator(std::shared_ptr<Object3D> target) {
	for (Animator& anim : _animators) {
		if (anim.getTarget() == target) {
			IFT_LOG_WARNING << "A target can only get one animation at a time";
			return;
		}
	}

	_animators.emplace_back(Animator(target));
}

Animator& AnimatorManager::getAnimator(size_t i) {
	assert(i >= 0 && i < _animators.size());
	return _animators.at(i);
}

void AnimatorManager::setup() { }

void AnimatorManager::update() {
	std::vector<size_t> to_remove;
	for (size_t i = 0; i < _animators.size(); i++) {
		if (_animators[i].getTarget().use_count() > 2)
			_animators[i].update();
		else
			to_remove.emplace_back(i);
	}

	for (int i : to_remove) {
		_animators.erase(_animators.begin() + i);
		IFT_LOG << "Removed animator " << i;
	}
}

void AnimatorManager::drawUI() {
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

void AnimatorManager::keyframesUI(std::vector<Keyframe>& keyframes) {
	for (size_t key = 0; key < keyframes.size(); key++) {
		float position[3] = {keyframes[key].position.x, keyframes[key].position.y, keyframes[key].position.z};
		float rotation[3] = {keyframes[key].rotation.x, keyframes[key].rotation.y, keyframes[key].rotation.z};

		ImGui::InputFloat3(("key" + std::to_string(key) + " : Position x y z").c_str(), position);
		keyframes[key].position.set(position[0], position[1], position[2]);

		ImGui::InputFloat3(("key" + std::to_string(key) + " : Rotation x y z").c_str(), rotation);
		keyframes[key].rotation.set(rotation[0], rotation[1], rotation[2]);

		char frame[64] = "";
		std::strcpy(frame, std::to_string(keyframes[key].frame).c_str());
		if (ImGui::InputText(("frame" + std::to_string(key)).c_str(), frame, 64, InspectorInterface::INPUT_DECIMAL_FLAGS)) {
			keyframes[key].frame = atof(frame);
		}
	}
}

} // namespace ift3100