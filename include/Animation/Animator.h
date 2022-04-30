#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "ofMain.h"
#include "Logger.h"
#include "Object3D.h"

#include <vector>

namespace ift3100 {

enum InterpolationType { Piecewise, Linear };

struct Keyframe {
	ofVec3f position;
	ofVec3f rotation;
	uint64_t frame;
};

/**
 * @brief Animate a target using a vector of @see Keyframe.
 * For now the target can only is an Object3D derivated object.
 * You can change position and rotation for now.
 * As keyframe depends of frame, animation speed depends of the framerate.
 */
class Animator {
	std::shared_ptr<Object3D> _target;
	int _indexLastKeyFrame;
	int _indexNextKeyFrame;

	InterpolationType _interpolationType;
	uint64_t _currentFrame;

	bool _paused;
	std::vector<Keyframe> _keyframes;

	void computePiecewiseInterpolation();
	void computeLinearInterpolation();

	void computeInterpolation();

public:
	Animator();
	Animator(std::shared_ptr<Object3D> target, InterpolationType type = InterpolationType::Linear);
	Animator(std::shared_ptr<Object3D> target, const std::vector<Keyframe> keyframes, InterpolationType type = InterpolationType::Linear);

	void setInterpolationType(InterpolationType type);
	void setTarget(std::shared_ptr<Object3D> target);
	void addKeyframe(const ofVec3f& position, const ofVec3f& rotation, uint64_t frame);

	void setup();
	void update();
	void reset();
	void pause();
	void resume();

	inline std::vector<Keyframe>& getKeyframes() {
		return _keyframes;
	}
	inline bool isPaused() {
		return _paused;
	}
	inline std::shared_ptr<Object3D> getTarget() {
		return _target;
	}
};

} // namespace ift3100

#endif