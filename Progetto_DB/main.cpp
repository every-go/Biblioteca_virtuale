#include "interfaccia_grafica/mainwindow.h"
#include "interfaccia_grafica/userarea.h"
#include "interfaccia_grafica/adminarea.h"
#include "interfaccia_grafica/librarymanager.h"
#include "DB/dbmanager.h"

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
    qDebug() << QSqlDatabase::drivers();
    QStackedWidget stackWidget;
    DbManager* dbManager = new DbManager();
    QList<Biblioteca*> oggetti;
    if(dbManager->connettiAlDatabase()){
        qDebug() << "Connessione al database riuscita!";
        oggetti = dbManager->loadBibliotecaFromDb();
    }
    else
        qDebug() << "Connessione al database fallita!";
    std::sort(oggetti.begin(), oggetti.end(), [](Biblioteca* a, Biblioteca* b){
        return a->getTitolo() < b->getTitolo();
    });
    MainWindow* mainWindows = new MainWindow(&stackWidget);
    UserArea* userArea = new UserArea(oggetti, &stackWidget);
    AdminArea* adminArea = new AdminArea(oggetti, &stackWidget);
    LibraryManager* library = new LibraryManager(&stackWidget);
    dbManager->addObserver(userArea);
    dbManager->addObserver(adminArea);
    stackWidget.addWidget(mainWindows);
    stackWidget.addWidget(userArea);
    stackWidget.addWidget(adminArea);
    stackWidget.addWidget(library);
    stackWidget.setCurrentIndex(0);
    stackWidget.show();

    QObject::connect(library, &LibraryManager::handle, adminArea, &AdminArea::handlePostAction);
    QObject::connect(library, &LibraryManager::update, dbManager, &DbManager::updateObject);
    QObject::connect(library, &LibraryManager::newObject, dbManager, &DbManager::savenewObject);
    QObject::connect(adminArea, &AdminArea::modifyObject, library, &LibraryManager::modifyObject);
    QObject::connect(adminArea, &AdminArea::removeObject, dbManager, &DbManager::deleteObject);
    QObject::connect(adminArea, &AdminArea::createNewObject, library, &LibraryManager::createObject);
    QObject::connect(userArea, &UserArea::prenota, dbManager, &DbManager::savePrenota);
    QObject::connect(userArea, &UserArea::ascoltato, dbManager, &DbManager::saveAscoltato);
    QObject::connect(userArea, &UserArea::visto, dbManager, &DbManager::saveVisto);
    QObject::connect(userArea, &UserArea::letto, dbManager, &DbManager::saveLetto);

    return app.exec();
}

