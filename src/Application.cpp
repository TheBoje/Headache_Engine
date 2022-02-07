#include "Application.h"
#include "ImageUtils.h"

namespace ift3100 {
	// fonction d'initialisation de l'application
	void Application::setup()
	{
		ofSetWindowTitle("IFT-3100 Main");

		renderer.setup();

        interface.gui.setup("Parametres");
        interface.gui.add(interface.xInterface.setup("Position x", 0, 0, width));
        interface.gui.add(interface.yInterface.setup("Position y", 30, 0, height));
        interface.gui.add(interface.rayonInterface.setup("Rayon", 60, 0, 256));
        interface.gui.add(interface.rInterface.setup("Rouge", 0, 0, 255));
        interface.gui.add(interface.gInterface.setup("Vert", 60, 0, 255));
        interface.gui.add(interface.bInterface.setup("Bleu", 255, 0, 255));
        interface.gui.add(interface.aInterface.setup("Alpha", 0, 0, 255));

        interface.texture1.setup("Texture 1");
        interface.color_picker_background.set("Couleur 1", ofColor(31), ofColor(0, 0), ofColor(255, 255));
        interface.color_picker_stroke.set("Couleur 2", ofColor(255), ofColor(0, 0), ofColor(255, 255));

        interface.texture2.setup("Texture 2");
        interface.color_picker_background.set("Couleur 1", ofColor(31), ofColor(0, 0), ofColor(255, 255));
        interface.color_picker_stroke.set("Couleur 2", ofColor(255), ofColor(0, 0), ofColor(255, 255));

        interface.slider_stroke_weight.set("Largeur de ligne", 4.0f, 0.0f, 10.0f);

        interface.texture1.add(interface.color_picker_background);
        interface.texture1.add(interface.color_picker_stroke);
        interface.texture1.add(interface.slider_stroke_weight);

        interface.texture2.add(interface.color_picker_background);
        interface.texture2.add(interface.color_picker_stroke);
        interface.texture2.add(interface.slider_stroke_weight);

        interface.gui.add(&interface.texture1);
        interface.gui.add(&interface.texture2);

        interface.textbox.set("text", "ift3100");
        interface.gui.add(interface.textbox);

        interface.button.setup("button");
        interface.button.addListener(this, &Application::button_pressed);
        interface.gui.add(&interface.button);

        interface.checkbox.setName("visible");
        interface.gui.add(interface.checkbox);

        interface.checkbox = true;

		ofLog() << "<app::setup>";
	}

	// fonction de mise à jour de la logique de l'application
	void Application::update()
	{
        // assigner les états courants de l'interface
        //ofBackground(interface.color_picker_background);
        renderer.stroke_color = interface.color_picker_stroke;
        renderer.stroke_weight = interface.slider_stroke_weight;
        renderer.text = interface.textbox;
        renderer.update();
	}

	// fonction de mise à jour du rendu de la fenêtre d'affichage de l'application
	void Application::draw()
	{
        renderer.draw();
        ofBackground(interface.color_picker_background);
        if (interface.checkbox)
            ofSetColor(interface.rInterface, interface.gInterface, interface.bInterface, interface.aInterface);
            ofCircle(interface.xInterface, interface.yInterface, interface.rayonInterface);
            interface.gui.draw();
	}

	// fonction appelée juste avant de quitter l'application
	void Application::exit()
	{
        interface.button.removeListener(this, &Application::button_pressed);
        ofLog() << "<app::exit>";
	}

	void Application::addImage(ofImage * image) {
		ofLog() << "<app:addImage> x:" << image->getHeight() << " y:" << image->getWidth();
		renderer._image = image;
	}

	void Application::keyReleased(int key)
	{
  		ofLog() << "<app::keyReleased: " << key << ">";

		if (key == ' ') {
			ift3100::ImageUtils::exportImage("render.png");
		}
        if (key == 117) // touche u
        {
            interface.checkbox = !interface.checkbox;
            ofLog() << "<toggle ui: " << interface.checkbox << ">";
        }
	}
    void Application::button_pressed()
    {
        // réinitialiser la zone de texte
        interface.textbox.set("text", "Projet");
        ofClear(interface.color_picker_background);

        ofLog() << "<button pressed>";
    }

	void Application::dragEvent(ofDragInfo dragInfo) {
		ofLog() << "<app::ofDragInfo> " << dragInfo.files.at(0);

		if (renderer._image == nullptr) {
			renderer._image = new ofImage();
		}

		renderer._image->load(dragInfo.files.at(0));

		if (renderer._image->getWidth() > 0 && renderer._image->getHeight() > 0)
			ofSetWindowShape(renderer._image->getWidth(), renderer._image->getHeight());
		}

    void Application::windowResized(int w, int h)
    {
        width = w;
        height = h;
        ofLog() << "<app::windowResized: (" << w << ", " << h << ")>";
    }
}
