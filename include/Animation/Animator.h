#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "ofMain.h"

#include <vector>

namespace ift3100 {

enum InterpolationType { Piecewise, Linear };

struct Keyframe {
	ofVec3f	 position;
	ofVec3f	 rotation;
	uint64_t frame;
};

/**
 * @brief Animate a target using a vector of @see Keyframe.
 * For now the target can only is an ofNode derivated object.
 * You can change position and rotation for now.
 * As keyframe depends of frame, animation speed depends of the framerate.
 */
class Animator {
	ofNode* _target;
	int		_indexLastKeyFrame;
	int		_indexNextKeyFrame;

	InterpolationType _interpolationType;
	uint64_t		  _currentFrame;

	bool				  _paused;
	std::vector<Keyframe> _keyframes;

	void computePiecewiseInterpolation();
	void computeLinearInterpolation();

	void computeInterpolation();

public:
	Animator();
	Animator(ofNode* target, InterpolationType type = InterpolationType::Linear);
	Animator(ofNode* target, const std::vector<Keyframe> keyframes, InterpolationType type = InterpolationType::Linear);

	void setInterpolationType(InterpolationType type);
	void setTarget(ofNode* target);
	void addKeyframe(const ofVec3f& position, const ofVec3f& rotation, uint64_t frame);

	void setup();
	void update();
	void reset();
	void pause();
	void resume();
};

} // namespace ift3100

#endif