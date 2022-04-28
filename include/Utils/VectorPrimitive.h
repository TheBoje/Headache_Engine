#ifndef VECTOR_PRIMITIVE_H
#define VECTOR_PRIMITIVE_H

#include "Constant.h"
#include "HierarchyItem.h"
#include "InterfaceUtils.h"

namespace ift3100 {

/**
	 * @brief Class used to store each Vector Primitive attributs
	 */
class VectorPrimitive : public HierarchyItem {
	PrimitiveType _type;

public:
	ofVec2f POSITION_1;
	ofVec2f POSITION_2;
	float STROKE_WIDTH;
	ofColor STROKE_COLOR;
	bool FILL;
	ofColor FILL_COLOR;
	std::string NAME;
	int TTL; // Time to live, number of frames, -1 = +infinity

	VectorPrimitive(const ofVec4f& pos,
		const PrimitiveType& type,
		float strokeWidth,
		ofColor strokeColor,
		bool fill,
		ofColor fillColor,
		int ttl = -1,
		std::string name = "root")
		: _type(type)
		, STROKE_WIDTH(strokeWidth)
		, STROKE_COLOR(strokeColor)
		, FILL(fill)
		, FILL_COLOR(fillColor)
		, NAME(name)
		, TTL(ttl) {
		POSITION_1.x = pos.x;
		POSITION_1.y = pos.y;
		POSITION_2.x = pos.z;
		POSITION_2.y = pos.w;
	}

	PrimitiveType getPrimitiveType() {
		return _type;
	}

	std::string toString() const {
		return NAME;
	}
};
} // namespace ift3100

#endif