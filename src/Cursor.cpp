
#include "ofMain.h"
#include "Cursor.h"

namespace ift3100 {
    void Cursor::setup(){
        ofHideCursor();
        IFT_LOG << "loading cursors from disk";
        cursorDefault.load("../../data/default.png");
        cursorDraw.load("../../data/draw.png");
        cursorStar.load("../../data/star.png");
        cursorCross.load("../../data/cross.png");
        IFT_LOG << "loading done";
    }

    // fonction qui dessine un curseur
    void Cursor::draw(int mouseX, int mouseY, MouseAction mouseAction, PrimitiveType type) {
        // paramètres de dessin
        int size = 30;

        switch (mouseAction) {
            case DrawPrimitive:
                ofSetColor(ofColor::white);
                cursorDraw.draw(mouseX - size, mouseY- size, size, size);
                switch (type) {
                    case Point:
                        ofSetColor(ofColor::black);
                        ofDrawCircle(mouseX-1.2*size, mouseY-1.2*size,0.2*size);
                        break;
                    case Line:
                        ofSetColor(ofColor::black);
                        ofDrawLine(mouseX-2*size, mouseY-2*size,mouseX-size,mouseY-1.5*size);
                        break;
                    case Rectangle:
                        ofSetColor(ofColor::black);
                        ofDrawRectRounded(mouseX-2.2*size, mouseY-2*size, size, 1.5*size, DEFAULT_RECTANGLE_ROUNDING);
                        break;
                    case Ellipse:
                        ofSetColor(ofColor::black);
                        ofDrawEllipse(mouseX-1.5*size, mouseY-1.5*size, size*1.5, size);
                        break;
                    case Triangle:
                        ofSetColor(ofColor::black);
                        ofDrawTriangle(mouseX-1.5*size, mouseY-1.5*size, mouseX-0.75*size, mouseY-1.5*size, mouseX-1.125*size, mouseY-2.05*size);
                        break;
                    case Cross:
                        ofSetColor(ofColor::black);
                        cursorCross.draw(mouseX - 2*size, mouseY- 2*size, size, size);
                        break;

                    case Star:
                        ofSetColor(ofColor::black);
                        cursorStar.draw(mouseX - 2*size, mouseY- 2*size, size, size);
                        break;
                }

                break;

            case None:
                cursorDefault.draw(mouseX - (size / 2), mouseY, size, size);
                break;

            default:
                IFT_LOG << "unknown type of cursor";
                break;
        }
    }
}