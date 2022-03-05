#ifndef RENDERER_H
#define RENDERER_H

#include "ofMain.h"
#include "Curseur.h"
#include "InterfaceUtils.h"
#include "Constant.h"
#include "VectorPrimitive.h"


namespace ift3100 {
	class Renderer {
	public:
		std::vector<VectorPrimitive> primitives{};
		std::stack<VectorPrimitive> redoPrimitives{};
		ofColor backgroundColor;

		Curseur curseur;

		void addPrimitive(const ofVec4f& pos, const PrimitiveType& type, 
				float strokeWidth, ofColor strokeColor,
				bool fill, ofColor fillColor, int ttl = -1);
		void undoPrimitive();
		void redoPrimitive();
		void setup();
		void update();
		void draw();
	};
}

#endif // !RENDERER_H