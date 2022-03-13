#include "Animator.h"
#include "Logger.h"

namespace ift3100 {

Animator::Animator()
	: _target(nullptr)
	, _indexLastKeyFrame(-1)
	, _indexNextKeyFrame(-1)
	, _interpolationType(InterpolationType::Linear)
	, _currentFrame(0)
	, _paused(true) { }

Animator::Animator(ofNode* target, InterpolationType type)
	: _target(target)
	, _indexLastKeyFrame(-1)
	, _indexNextKeyFrame(-1)
	, _interpolationType(type)
	, _currentFrame(0)
	, _paused(true) { }

Animator::Animator(ofNode* target, const std::vector<Keyframe> keyframes, InterpolationType type)
	: _target(target)
	, _indexLastKeyFrame(-1)
	, _indexNextKeyFrame(-1)
	, _interpolationType(type)
	, _currentFrame(0)
	, _paused(true)
	, _keyframes(keyframes) {
	if (_keyframes.size() >= 2) {
		_indexLastKeyFrame = 0;
		_indexNextKeyFrame = 1;
	}
}

void Animator::setInterpolationType(InterpolationType type) { _interpolationType = type; }

void Animator::setTarget(ofNode* target) { _target = target; }

/**
 * @brief Add a keyframe to the animation. 
 * The keyframes are sort by frame.
 * 
 * @param position 
 * @param rotation 
 * @param frame 
 */
void Animator::addKeyframe(const ofVec3f& position, const ofVec3f& rotation, uint64_t frame) {
	Keyframe kf;
	kf.position = position;
	kf.rotation = rotation;
	kf.frame	= frame;

	std::size_t keyframes_size = _keyframes.size();

	if (keyframes_size == 0) {
		_keyframes.push_back(kf);
		return;
	}

	if (keyframes_size == 1) {
		if (_keyframes[0].frame > frame)
			_keyframes.insert(_keyframes.begin(), kf);
		else
			_keyframes.push_back(kf);

		return;
	}

	std::vector<Keyframe>::iterator it;
	for (it = std::begin(_keyframes); it != std::end(_keyframes); ++it) {
		if ((*it).frame > frame) {
			_keyframes.insert(it - 1, kf);
			break;
		}
	}

	if (it == _keyframes.end())
		_keyframes.push_back(kf);

	IFT_LOG << "Add keyframe pos :" << position << " | rot : " << rotation << " | frame : " << frame;
}

void Animator::setup() { }

void Animator::update() {
	if (!_paused && _target != nullptr) {
		computeInterpolation();
		_currentFrame++;
	}
}

/**
 * @brief Reset the animation and pause it.
 * Set the target to the first animation position and rotation.
 * 
 */
void Animator::reset() {
	_currentFrame = 0;
	_paused		  = true;

	if (_keyframes.size() >= 2) {
		_indexLastKeyFrame = 0;
		_indexNextKeyFrame = 1;

		_target->setPosition(_keyframes[_indexLastKeyFrame].position);
		_target->setGlobalOrientation(glm::quat(_keyframes[_indexLastKeyFrame].rotation * DEG_TO_RAD));
	} else {
		_indexLastKeyFrame = -1;
		_indexNextKeyFrame = -1;
	}
}

void Animator::pause() { _paused = true; }

void Animator::resume() { _paused = false; }

void Animator::computePiecewiseInterpolation() {
	Keyframe* lastKeyFrame = &_keyframes[_indexLastKeyFrame];
	Keyframe* nextKeyFrame = &_keyframes[_indexNextKeyFrame];

	if ((_currentFrame - lastKeyFrame->frame) < ((nextKeyFrame->frame - lastKeyFrame->frame) / 2)) {
		_target->setPosition(lastKeyFrame->position);
		_target->setGlobalOrientation(glm::quat(lastKeyFrame->rotation * DEG_TO_RAD));
	} else {
		_target->setPosition(nextKeyFrame->position);
		_target->setGlobalOrientation(glm::quat(nextKeyFrame->rotation * DEG_TO_RAD));
	}
}

void Animator::computeLinearInterpolation() {
	Keyframe* lastKeyFrame = &_keyframes[_indexLastKeyFrame];
	Keyframe* nextKeyFrame = &_keyframes[_indexNextKeyFrame];

	float	alpha = (float)(_currentFrame - lastKeyFrame->frame) / (float)(nextKeyFrame->frame - lastKeyFrame->frame);
	ofVec3f lerpPosition(ofLerp(lastKeyFrame->position.x, nextKeyFrame->position.x, alpha),
		ofLerp(lastKeyFrame->position.y, nextKeyFrame->position.y, alpha),
		ofLerp(lastKeyFrame->position.z, nextKeyFrame->position.z, alpha));

	ofVec3f lerpRotation(ofLerpDegrees(lastKeyFrame->rotation.x, nextKeyFrame->rotation.x, alpha),
		ofLerpDegrees(lastKeyFrame->rotation.y, nextKeyFrame->rotation.y, alpha),
		ofLerpDegrees(lastKeyFrame->rotation.z, nextKeyFrame->rotation.z, alpha));

	_target->setPosition(lerpPosition);
	_target->setGlobalOrientation(glm::quat(lerpRotation * DEG_TO_RAD));
}

void Animator::computeInterpolation() {
	if (_indexNextKeyFrame == _keyframes.size() || _indexLastKeyFrame == -1 || _indexNextKeyFrame == -1) {
		IFT_LOG << "reset";
		reset();
		resume(); // infinitely looping if resume here an unpausing
		return;
	}

	if (_keyframes[_indexNextKeyFrame].frame <= _currentFrame) {
		_indexLastKeyFrame = _indexNextKeyFrame;
		_indexNextKeyFrame++;
	}

	switch (_interpolationType) {
		case InterpolationType::Piecewise: computePiecewiseInterpolation(); break;
		case InterpolationType::Linear: computeLinearInterpolation(); break;
		default: break;
	}
}

} // namespace ift3100