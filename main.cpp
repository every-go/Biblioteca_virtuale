#include "interfaccia_grafica/mainwindow.h"
#include "interfaccia_grafica/userarea.h"
#include "interfaccia_grafica/adminarea.h"
#include "interfaccia_grafica/librarymanager.h"
#include "JSON/jsonmanager.h"

// Ho incluso solo gli header necessari per evitare dipendenze ridondanti,
// poiché le classi come Cd, Film, Manga e Riviste includono già le dipendenze comuni
// come Multimedia, Biblioteca e Libri.
#include "modello_logico/cd.h"
#include "modello_logico/film.h"
#include "modello_logico/manga.h"
#include "modello_logico/riviste.h"

#include <algorithm>
#include <QApplication>
#include <QStackedWidget>
#include <QObject>
#include <QFileDialog>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStackedWidget stackWidget;
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Scegli Json", "", "Json (*.json)");
    JsonManager* manager = new JsonManager(filePath);
    QList<Biblioteca*> oggetti = manager->loadBibliotecaListFromJson(filePath);
    MainWindow* mainWindows = new MainWindow(&stackWidget);
    UserArea* userArea = new UserArea(oggetti, &stackWidget);
    AdminArea* adminArea = new AdminArea(oggetti, &stackWidget);
    LibraryManager* library = new LibraryManager(&stackWidget);
    manager->addObserver(userArea);
    manager->addObserver(adminArea);
    stackWidget.addWidget(mainWindows);
    stackWidget.addWidget(userArea);
    stackWidget.addWidget(adminArea);
    stackWidget.addWidget(library);
    stackWidget.setCurrentIndex(0);
    stackWidget.show();

    QObject::connect(library, &LibraryManager::handle, adminArea, &AdminArea::handlePostAction);
    QObject::connect(library, &LibraryManager::update, manager, &JsonManager::updateObject);
    QObject::connect(library, &LibraryManager::newObject, manager, &JsonManager::savenewObject);
    QObject::connect(adminArea, &AdminArea::modifyObject, library, &LibraryManager::modifyObject);
    QObject::connect(adminArea, &AdminArea::removeObject, manager, &JsonManager::deleteObject);
    QObject::connect(adminArea, &AdminArea::createNewObject, library, &LibraryManager::createObject);
    QObject::connect(userArea, &UserArea::prenota, manager, &JsonManager::savePrenota);
    QObject::connect(userArea, &UserArea::ascoltato, manager, &JsonManager::saveAscoltato);
    QObject::connect(userArea, &UserArea::visto, manager, &JsonManager::saveVisto);
    QObject::connect(userArea, &UserArea::letto, manager, &JsonManager::saveLetto);

    return app.exec();
}

