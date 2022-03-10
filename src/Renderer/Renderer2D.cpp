#include "Renderer2D.h"
#include "Application.h"

namespace ift3100 {
	Renderer2D::Renderer2D(Application& _application) : application(_application) {}

	void Renderer2D::setup() {
		ofSetFrameRate(60);
		ofSetCircleResolution(32);
		backgroundColor = ofColor::darkGray;
		primitives.reserve(1000);
		hierarchyPrimitives.setRoot(std::make_shared<VectorPrimitive>(ofVec4f(0, 0, 0, 0), PrimitiveType::Point, 0, ofColor(0), true, ofColor(0)));
		IFT_LOG << "done";
	}

	void Renderer2D::update() {
		// Low framerate warning
		if (ofGetFrameRate() < 5 && ofGetFrameNum() > 5) {
			IFT_LOG_WARNING << std::setprecision(2) << "frame:" << ofGetFrameNum() << " fps: " << ofGetFrameRate();
		}

		if(!hierarchyPrimitives.selected_nodes.empty()) {
			for(Hierarchy<VectorPrimitive> * selected : hierarchyPrimitives.selected_nodes) {
				selected->map([=](std::shared_ptr<VectorPrimitive> p) {
					p->FILL = application.interface.primitiveFill;
					p->FILL_COLOR = application.interface.primitiveFillColor;
					p->STROKE_WIDTH = application.interface.primitiveStrokeWidth;
					p->STROKE_COLOR = application.interface.primitiveStrokeColor;
				});
			}
		}
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
	void Renderer2D::addPrimitive(const ofVec4f& pos, const PrimitiveType& type, float strokeWidth, ofColor strokeColor, bool fill, ofColor fillColor, int ttl) {
		std::shared_ptr<VectorPrimitive> sharedPrimitive = std::make_shared<VectorPrimitive>(pos, type, strokeWidth, strokeColor, fill, fillColor, ttl, "child");
		primitives.push_back(sharedPrimitive);

		if(ttl == -1) {
			hierarchyPrimitives.addChild(sharedPrimitive);
		}
	}

	/**
	 * Undo the last primitive added to the primitive stack (via Renderer2D::addPrimitive).
	*/
	void Renderer2D::undoPrimitive() {
		// Remove last primitive and give it to redoPrimitive, serving as history stack
		if (!primitives.empty()) {
			VectorPrimitive p = *primitives.back();
			primitives.pop_back();
			redoPrimitives.push(p);
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
			primitives.push_back(std::make_shared<VectorPrimitive>(redoPrimitives.top()));
			redoPrimitives.pop();
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
					case Line:
						ofDrawLine(p->POSITION_1, p->POSITION_2);
						break;
					case Rectangle:
						ofDrawRectRounded(p->POSITION_1, p->POSITION_2.x - p->POSITION_1.x, p->POSITION_2.y - p->POSITION_1.y, DEFAULT_RECTANGLE_ROUNDING);
						break;
					case Ellipse:
						ofDrawEllipse(p->POSITION_1.x + (p->POSITION_2.x - p->POSITION_1.x) / 2.0f, p->POSITION_1.y + (p->POSITION_2.y - p->POSITION_1.y) / 2.0f, p->POSITION_2.x - p->POSITION_1.x, p->POSITION_2.y - p->POSITION_1.y);
						break;
					case Triangle:
						ofDrawTriangle(p->POSITION_1.x, p->POSITION_2.y, p->POSITION_1.x + (p->POSITION_2.x - p->POSITION_1.x) / 2.0f, p->POSITION_1.y, p->POSITION_2.x, p->POSITION_2.y);
						break;
					case Cross: {
						int sizeX = p->POSITION_1.x - p->POSITION_2.x;
						int sizeY = p->POSITION_1.y - p->POSITION_2.y;
						ofDrawRectRounded(p->POSITION_2.x, p->POSITION_2.y + 2.0f*(sizeY / 5.0f), sizeX, sizeY / 5.0f, DEFAULT_RECTANGLE_ROUNDING);
						ofDrawRectRounded(p->POSITION_2.x + 2.0f*(sizeX / 5.0f), p->POSITION_2.y, sizeX / 5.0f, sizeY, DEFAULT_RECTANGLE_ROUNDING);
						break;
					}
					case Star: {
						int sizeX = p->POSITION_1.x - p->POSITION_2.x;
						int sizeY = p->POSITION_1.y - p->POSITION_2.y;
						ofDrawTriangle(p->POSITION_2.x, p->POSITION_2.y + (sizeY / 4.0f), p->POSITION_1.x, p->POSITION_2.y + (sizeY / 4.0f), p->POSITION_2.x + (sizeX / 2.0f), p->POSITION_1.y);
						ofDrawTriangle(p->POSITION_2.x, p->POSITION_1.y - (sizeY / 4.0f), p->POSITION_1.x, p->POSITION_1.y - (sizeY / 4.0f), p->POSITION_2.x + (sizeX / 2.0f), p->POSITION_2.y);
						break;
					}
				}
			}
			// Update time to live
			p->TTL-=1;
		}
		// Remove ttl = 0 (dead) primitives
		primitives.erase(std::remove_if(primitives.begin(), primitives.end(),
		[](const std::shared_ptr<VectorPrimitive> p) {
			return p->TTL == 0;
		}), primitives.end());
    }
}