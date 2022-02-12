//
// Created by vincent on 2022-02-07.
//

#include "Interface.h"
using namespace ift3100;

void Interface::construire_interface()
{
    gui.setup("Parametres");



    //Bouton pour mettre le curseur en mode sélection
    buttonSelection.setup("Selectionner");
    buttonSelection.addListener(this, &Interface::button_pressed);
    gui.add(&buttonSelection);


    //Bouton pour mettre le curseur en mode Déplacement
    buttonDeplacement.setup("Deplacer");
    buttonDeplacement.addListener(this, &Interface::button_pressed);
    gui.add(&buttonDeplacement);

    //Bouton pour mettre le curseur en mode Suppression
    buttonSuppression.setup("Supprimer");
    buttonSuppression.addListener(this, &Interface::button_pressed);
    gui.add(&buttonSuppression);

    //Menu pour les Outils de dessin (Forme vectorielle / Primitives Vectorielles
    OutilsDessin.setup("Outils de dessin");
    color_picker_background.set("Couleur de fond", ofColor(31), ofColor(0, 0), ofColor(255, 255));
    color_picker_object.set("Couleur de l'objet", ofColor(31), ofColor(0, 0), ofColor(255, 255));
    OutilsDessin.add(color_picker_background);
    OutilsDessin.add(color_picker_object);
    gui.add(&OutilsDessin);

    OutilsDessin.add(odPrimitives.setup("Primitive"));
    odPrimitives.add(xInterface.setup("Position x", 0, 0, width));
    odPrimitives.add(yInterface.setup("Position y", 30, 0, height));
    odPrimitives.add(rayonInterface.setup("Rayon", 60, 0, 256));
    odPrimitives.add(rInterface.setup("Rouge", 0, 0, 255));
    odPrimitives.add(gInterface.setup("Vert", 60, 0, 255));
    odPrimitives.add(bInterface.setup("Bleu", 255, 0, 255));
    odPrimitives.add(aInterface.setup("Alpha", 0, 0, 255));

    //Bouton pour générer l'histogramme
    buttonHistogramme.setup("Generer l'Histogramme");
    buttonHistogramme.addListener(this, &Interface::button_pressed);
    gui.add(&buttonHistogramme);

    //Bouton pour importer une image
    buttonImportimage.setup("Importer une image");
    buttonImportimage.addListener(this, &Interface::button_pressed);
    gui.add(&buttonImportimage);

    //Activer l'option de dessiner les arrêtes d'une boîte
    checkboxBoiteDelimitation.setName("Boite de Délimitation");
    gui.add(checkboxBoiteDelimitation);
    checkboxBoiteDelimitation = false;

    PointVue.setup("Point de vue");
    PointVue.add(buttonPV1.setup("Point de vue #1"));
    PointVue.add(buttonPV2.setup("Point de vue #2"));
    PointVue.add(buttonPV3.setup("Point de vue #3"));
    gui.add(&PointVue);

    Projection.setup("Mode de projection");
    Projection.add(buttonPerspective.setup("Perspective"));
    Projection.add(buttonOrthogonale.setup("Orthogonale"));
    gui.add(&Projection);

    //textbox.set("text", "ift3100");
    //gui.add(textbox);

    checkbox.setName("Fermer menu");
    gui.add(checkbox);

    checkbox = true;
}

void Interface::button_pressed()
{
    ofLog() << "<button pressed>";
}