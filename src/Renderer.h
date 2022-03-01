#ifndef RENDERER_H
#define RENDERER_H

#include "ofMain.h"
#include "Curseur.h"
#include "Interface.h"
#include "VectorPrimitive.h"
#include "Hierarchy.h"

namespace ift3100 {
	class Renderer {
	public:
		std::vector<VectorPrimitive> primitives{};
		std::stack<VectorPrimitive> redoPrimitives{};
		ofColor backgroundColor;

		Curseur curseur;


		float _time_current;
		float _time_last;
		float _time_elapsed;

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