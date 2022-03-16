#include "Renderer2D.h"
#include "Application.h"

namespace ift3100 {
Renderer2D* Renderer2D::_renderer2D = nullptr;

Renderer2D::Renderer2D() { }

Renderer2D::~Renderer2D() { }

Renderer2D* Renderer2D::Get() {
	if (_renderer2D == nullptr) {
		_renderer2D = new Renderer2D();
	}
	return _renderer2D;
}

void Renderer2D::setup() {
	backgroundColor = ofColor::darkGray;
	ofSetBackgroundColor(backgroundColor);

	primitives.reserve(100);
	hierarchyPrimitives.setRoot(std::make_shared<VectorPrimitive>(ofVec4f(0, 0, 0, 0), PrimitiveType::Point, 0, ofColor(0), true, ofColor(0)));

	IFT_LOG << "done";
}

void Renderer2D::update() { hierarchyPrimitives.update(); }

/**
 * Add a preview primitive for 1 frame
 *
 * @param mousePos
 * @param type
 * @param strokeWidth
 * @param strokeColor
 * @param fill whether the drawn primitive is filled
 * @param fillColor ignored if `fill` is `false`
*/
void Renderer2D::addPreviewPrimitive(
	const ofVec4f& mousePos, const PrimitiveType& type, float strokeWidth, ofColor strokeColor, bool fill, ofColor fillColor) {
	// Call proper render method based on UI state / mouse action
	// Set colors slightly transparent
	strokeColor.a = 80;
	fillColor.a	  = 80;

	// Draw transparent preview primitive for 1 frame
	addPrimitive(mousePos, type, strokeWidth, strokeColor, fill, fillColor, 1);
	// Draw bounding box of drawing
	addPrimitive(mousePos, Rectangle, 1, ofColor(0, 80), false, ofColor::white, 1);
}

/**
 * Add a primitive in the render stack
 *
 * @param mousePos position top-left & bottom-right corners of primitive (from mouse position)
 * @param type type of primitive to draw. eg Line
 * @param strokeWidth
 * @param strokeColor
 * @param fill enable primitive filling
 * @param fillColor
*/
void Renderer2D::addPrimitive(
	const ofVec4f& pos, const PrimitiveType& type, float strokeWidth, ofColor strokeColor, bool fill, ofColor fillColor, int ttl) {
	std::shared_ptr<VectorPrimitive> sharedPrimitive =
		std::make_shared<VectorPrimitive>(pos, type, strokeWidth, strokeColor, fill, fillColor, ttl, InterfaceUtils::primitiveTypeToString(type));
	primitives.push_back(sharedPrimitive);

	if (ttl == -1) {							  // If the primitive has an unlimited lifetime
		undoPrimitives.push(hierarchyPrimitives); // copy the hierarchyPrimitive when pushing it
		hierarchyPrimitives.addChild(sharedPrimitive);
		IFT_LOG << "added primitive";
	}
}

/**
 * Delete selected primitives (in UI) from renderer primitives and UI.
 *
 * Note: Deleted primitive is added to undo stack.
*/
void Renderer2D::deleteSelected() {
	undoPrimitives.push(hierarchyPrimitives);
	// Delete each selected VectorPrimitive in hierarchy
	for (Hierarchy<VectorPrimitive>* selected : hierarchyPrimitives.selected_nodes) {
		if (hierarchyPrimitives.isRoot(*selected))
			hierarchyPrimitives.clear();
		else
			delete selected;
	}

	for (auto it = primitives.begin(); it != primitives.end(); it++) {
		// remove shared_ptr that are only in the primitives vector (meaning that there are not in the hierarchy)
		if (it->use_count() == 1) {
			primitives.erase(it--);
		}
	}
	hierarchyPrimitives.selected_nodes.clear();
}

/**
 * Undo the last primitive added to the primitive stack (via Renderer2D::addPrimitive).
*/
void Renderer2D::undoPrimitive() {
	if (!undoPrimitives.empty()) {
		// pop the previous hierarchy and apply it to the current hierarchy
		HierarchyContainer<VectorPrimitive> p = undoPrimitives.top();
		undoPrimitives.pop();
		redoPrimitives.push(hierarchyPrimitives);
		hierarchyPrimitives = p;
		primitives.clear();
		primitives = hierarchyPrimitives.flattenRef();
	} else {
		IFT_LOG << "nothing to undo";
	}
}

/**
 * Redo the last undo primitive (via Renderer2D::undoPrimitive).
*/
void Renderer2D::redoPrimitive() {
	// Push the newest primitive in stack history and remove it from stack
	if (!redoPrimitives.empty()) {
		HierarchyContainer<VectorPrimitive> p = redoPrimitives.top();
		redoPrimitives.pop();
		undoPrimitives.push(hierarchyPrimitives);
		hierarchyPrimitives = p;
		primitives.clear();
		primitives = p.flattenRef();
	} else {
		IFT_LOG << "nothing to redo";
	}
}

void Renderer2D::draw() {
	ofSetBackgroundColor(backgroundColor);
	// Draw primitives based on their data
	for (auto p : primitives) {
		ofSetLineWidth(p->STROKE_WIDTH);
		for (int i = 0; i < 2; i++) {
			if (i == 0 && p->FILL) {
				ofFill();
				ofSetColor(p->FILL_COLOR);
			} else {
				ofNoFill();
				ofSetColor(p->STROKE_COLOR);
			}

			switch (p->getPrimitiveType()) {
				case Point:
					ofDrawRectRounded(p->POSITION_2 - (p->STROKE_WIDTH / 2.0f), p->STROKE_WIDTH, p->STROKE_WIDTH, DEFAULT_RECTANGLE_ROUNDING);
					break;
				case Line: ofDrawLine(p->POSITION_1, p->POSITION_2); break;
				case Rectangle:
					ofDrawRectRounded(
						p->POSITION_1, p->POSITION_2.x - p->POSITION_1.x, p->POSITION_2.y - p->POSITION_1.y, DEFAULT_RECTANGLE_ROUNDING);
					break;
				case Ellipse:
					ofDrawEllipse(p->POSITION_1.x + (p->POSITION_2.x - p->POSITION_1.x) / 2.0f,
						p->POSITION_1.y + (p->POSITION_2.y - p->POSITION_1.y) / 2.0f,
						p->POSITION_2.x - p->POSITION_1.x,
						p->POSITION_2.y - p->POSITION_1.y);
					break;
				case Triangle:
					ofDrawTriangle(p->POSITION_1.x,
						p->POSITION_2.y,
						p->POSITION_1.x + (p->POSITION_2.x - p->POSITION_1.x) / 2.0f,
						p->POSITION_1.y,
						p->POSITION_2.x,
						p->POSITION_2.y);
					break;
				case Cross: {
					int sizeX = p->POSITION_1.x - p->POSITION_2.x;
					int sizeY = p->POSITION_1.y - p->POSITION_2.y;
					ofDrawRectRounded(p->POSITION_2.x, p->POSITION_2.y + 2.0f * (sizeY / 5.0f), sizeX, sizeY / 5.0f, DEFAULT_RECTANGLE_ROUNDING);
					ofDrawRectRounded(p->POSITION_2.x + 2.0f * (sizeX / 5.0f), p->POSITION_2.y, sizeX / 5.0f, sizeY, DEFAULT_RECTANGLE_ROUNDING);
					break;
				}
				case Star: {
					int sizeX = p->POSITION_1.x - p->POSITION_2.x;
					int sizeY = p->POSITION_1.y - p->POSITION_2.y;
					ofDrawTriangle(p->POSITION_2.x,
						p->POSITION_2.y + (sizeY / 4.0f),
						p->POSITION_1.x,
						p->POSITION_2.y + (sizeY / 4.0f),
						p->POSITION_2.x + (sizeX / 2.0f),
						p->POSITION_1.y);
					ofDrawTriangle(p->POSITION_2.x,
						p->POSITION_1.y - (sizeY / 4.0f),
						p->POSITION_1.x,
						p->POSITION_1.y - (sizeY / 4.0f),
						p->POSITION_2.x + (sizeX / 2.0f),
						p->POSITION_2.y);
					break;
				}
			}
		}
		// Update time to live
		p->TTL -= 1;
	}
	// Remove ttl = 0 (dead) primitives
	primitives.erase(
		std::remove_if(primitives.begin(), primitives.end(), [](const std::shared_ptr<VectorPrimitive> p) { return p->TTL == 0; }), primitives.end());
}
} // namespace ift3100