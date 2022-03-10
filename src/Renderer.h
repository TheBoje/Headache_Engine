#ifndef RENDERER_H
#define RENDERER_H

#include "ofMain.h"
#include "Interface.h"
#include "Cursor.h"

namespace ift3100 {
	struct VectorPrimitive {
		PrimitiveType type;            
		ofVec2f position1; 
		ofVec2f position2;
		float   strokeWidth;
		ofColor strokeColor;
		bool	fill;
		ofColor fillColor;
	};

	class Renderer {
	public:
		std::vector<VectorPrimitive> primitives{};
		std::stack<VectorPrimitive> redoPrimitives{};
        ofColor backgroundColor;

		void addPrimitive(const ofVec4f& pos, const PrimitiveType& type,
						float strokeWidth, ofColor strokeColor,
						bool fill, ofColor fillColor);
		void undoPrimitive();
		void redoPrimitive();
		void setup();
		void update();
		void draw();
	};
}

#endif // !RENDERER_H
