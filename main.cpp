#include "interfaccia_grafica/mainwindow.h"
#include "interfaccia_grafica/userarea.h"
#include "interfaccia_grafica/adminarea.h"
#include "JSON/jsonmanager.h"

// Ho incluso solo gli header necessari per evitare dipendenze ridondanti,
// poiché le classi come Cd, Film, Manga e Riviste includono già le dipendenze comuni
// come Multimedia, Biblioteca e Libri.
#include "modello_logico/cd.h"
#include "modello_logico/film.h"
#include "modello_logico/manga.h"
#include "modello_logico/riviste.h"

#include <QApplication>
#include <QStackedWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStackedWidget stackWidget;
    qRegisterMetaType<Biblioteca*>("Biblioteca");
    //QString filePath = QFileDialog::getOpenFileName(nullptr, "Scegli Json", "", "Json (*.json)");
    JsonManager* manager = new JsonManager("dati.json");
    QList<Biblioteca*> oggetti = manager->loadBibliotecaListFromJson("dati.json");
    MainWindow* mainWindows = new MainWindow(&stackWidget);
    UserArea* userArea = new UserArea(oggetti, &stackWidget);
    AdminArea* adminArea = new AdminArea(oggetti, &stackWidget);
    manager->addObserver(userArea);
    manager->addObserver(adminArea);
    stackWidget.addWidget(mainWindows);
    stackWidget.addWidget(userArea);
    stackWidget.addWidget(adminArea);
    stackWidget.setCurrentIndex(0);
    stackWidget.show();

    QObject::connect(adminArea, &AdminArea::objectCreationRequested, manager, &JsonManager::handleObjectCreation);
    QObject::connect(adminArea, &AdminArea::removeObject, manager, &JsonManager::deleteObject);
    QObject::connect(adminArea, &AdminArea::modifyObject, manager, &JsonManager::modifyObject);
    QObject::connect(userArea, &UserArea::prenota, manager, &JsonManager::savePrenota);
    QObject::connect(userArea, &UserArea::ascoltato, manager, &JsonManager::saveAscoltato);
    QObject::connect(userArea, &UserArea::visto, manager, &JsonManager::saveVisto);
    QObject::connect(userArea, &UserArea::letto, manager, &JsonManager::saveLetto);

    return a.exec();
}

