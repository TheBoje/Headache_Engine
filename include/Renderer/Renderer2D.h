#ifndef RENDERER_2D_H
#define RENDERER_2D_H

#include "Constant.h"
#include "Cursor.h"
#include "HierarchyContainer.h"
#include "HierarchyItem.h"
#include "Interface.h"
#include "InterfaceUtils.h"
#include "Logger.h"
#include "VectorPrimitive.h"
#include "ofMain.h"

namespace ift3100 {
class Application;
class Renderer2D {
private:
	static Renderer2D* _renderer2D;

protected:
	Renderer2D();

public:
	Renderer2D(const Renderer2D&) = delete;
	~Renderer2D();

	static Renderer2D* Get();

	Renderer2D& operator=(const Renderer2D&) = delete;

	HierarchyContainer<VectorPrimitive> hierarchyPrimitives;
	std::vector<std::shared_ptr<VectorPrimitive>> primitives {};
	std::stack<HierarchyContainer<VectorPrimitive>> undoPrimitives {};
	std::stack<HierarchyContainer<VectorPrimitive>> redoPrimitives {};

	ofColor backgroundColor;

	void setup();
	void update();
	void draw();

	void addPreviewPrimitive(
		const ofVec4f& mousePos, const PrimitiveType& type, float strokeWidth, ofColor strokeColor, bool fill, ofColor fillColor);
	void addPrimitive(
		const ofVec4f& pos, const PrimitiveType& type, float strokeWidth, ofColor strokeColor, bool fill, ofColor fillColor, int ttl = -1);
	void deleteSelected();
	void undoPrimitive();
	void redoPrimitive();
};
} // namespace ift3100

#endif // !RENDERER_2D_H