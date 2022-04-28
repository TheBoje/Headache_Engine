#include "AnimatorManager.h"
#include "Asserts.h"
#include "InspectorInterface.h"

namespace ift3100 {

AnimatorManager::AnimatorManager() { }

/**
 * @brief Add an animator to the target
 * If the target already has an animator, no other is added
 *
 * @param target
 */
void AnimatorManager::addAnimator(std::shared_ptr<Object3D> target) {
	for (Animator& anim : _animators) {
		if (anim.getTarget() == target) {
			IFT_LOG_WARNING << "A target can only get one animation at a time";
			return;
		}
	}

	_animators.emplace_back(Animator(target));
}

/**
 * @brief Get the i'th animator in the animator manager.
 *
 * @param i
 * @return Animator&
 */
Animator& AnimatorManager::getAnimator(size_t i) {
	IFT_ASSERT(i >= 0 && i < _animators.size(), "Could not get animator [" << i << "], out of range [0, " << _animators.size() << "]");
	return _animators.at(i);
}

void AnimatorManager::setup() { }

/**
 * @brief Call update on every animator presents in the
 * animator manager. If a target is unreferenced, delete the animator.
 */
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

/**
 * @brief Draw the animator manager ui. Contains :
 * - Resume/Pause and Reset buttons
 * - keyframes manager (add/delete keyframe, modify keyframe attributs)
 * When adding a new keyframe, the default values will be :
 * - if there is no precedent keyframes, the position and rotation of the target with the frame 0
 * - else the attributs of the precedent keyframe
 */
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
				Keyframe key;
				if (_animators[i].getKeyframes().empty()) {
					ofNode target = *_animators[i].getTarget()->getNode();
					key.position = target.getPosition();
					key.rotation = target.getOrientationEulerDeg();
					key.frame = 0;
				} else {
					key = _animators[i].getKeyframes()[_animators[i].getKeyframes().size() - 1];
				}
				_animators[i].addKeyframe(key.position, key.rotation, key.frame);
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
		ImGui::SameLine();
		if (ImGui::Button(("Remove key " + std::to_string(key)).c_str())) {
			keyframes.erase(keyframes.begin() + key);
		}
	}
}

} // namespace ift3100