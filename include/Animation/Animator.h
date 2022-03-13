#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "ofMain.h"

#include <vector>

namespace ift3100 {

enum InterpolationType { Piecewise, Linear, Polynomial, Spline };

struct Keyframe {
	ofVec3f	 position;
	ofVec3f	 rotation;
	uint64_t frame;
};

class Animator {
	InterpolationType _interpolationType;

	std::vector<Keyframe> _keyframes;
	ofNode*				  _target;

	uint64_t _currentFrame;

	bool _paused;

	int _indexLastKeyFrame;
	int _indexNextKeyFrame;

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