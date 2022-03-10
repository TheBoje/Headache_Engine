#ifndef RENDERER_2D_H
#define RENDERER_2D_H

#include "ofMain.h"
#include "Interface.h"
#include "Cursor.h"
#include "InterfaceUtils.h"
#include "Constant.h"
#include "VectorPrimitive.h"
#include "ofxAssimpModelLoader.h"
#include "HierarchyItem.h"
#include "HierarchyContainer.h"
#include "Logger.h"



namespace ift3100 {
	class Application;
	class Renderer2D {
	public:
		Application& application;

		HierarchyContainer<VectorPrimitive> hierarchyPrimitives;
		std::vector<std::shared_ptr<VectorPrimitive>> primitives{};
		std::stack<VectorPrimitive> redoPrimitives{};

		ofColor backgroundColor;

		Renderer2D(Application& _application);
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

#endif // !RENDERER_2D_H