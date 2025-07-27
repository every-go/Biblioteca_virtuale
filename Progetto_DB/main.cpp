#include "interfaccia_grafica/mainwindow.h"
#include "interfaccia_grafica/userarea.h"
#include "interfaccia_grafica/adminarea.h"
#include "interfaccia_grafica/librarymanager.h"
#include "DB/dbmanager.h"
#include "DB/dbdelete.h"
#include "DB/dbupdate.h"
#include "DB/dbcreate.h"

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
    DbDelete* dbDelete = new DbDelete(oggetti);
    DbUpdate* dbUpdate = new DbUpdate();
    DbCreate* dbCreate = new DbCreate();
    std::sort(oggetti.begin(), oggetti.end(), [](Biblioteca* a, Biblioteca* b){
        return a->getTitolo() < b->getTitolo();
    });
    MainWindow* mainWindows = new MainWindow(&stackWidget);
    UserArea* userArea = new UserArea(oggetti, &stackWidget);
    AdminArea* adminArea = new AdminArea(oggetti, &stackWidget);
    LibraryManager* library = new LibraryManager(&stackWidget);
    dbDelete->addObserver(userArea);
    dbDelete->addObserver(adminArea);
    dbUpdate->addObserver(userArea);
    dbUpdate->addObserver(adminArea);
    dbCreate->addObserver(userArea);
    dbCreate->addObserver(adminArea);
    stackWidget.addWidget(mainWindows);
    stackWidget.addWidget(userArea);
    stackWidget.addWidget(adminArea);
    stackWidget.addWidget(library);
    stackWidget.setCurrentIndex(0);
    stackWidget.show();

    QObject::connect(library, &LibraryManager::handle, adminArea, &AdminArea::handlePostAction);
    QObject::connect(library, &LibraryManager::update, dbUpdate, &DbUpdate::updateObject);
    QObject::connect(library, &LibraryManager::newObject, dbCreate, &DbCreate::createnewObject);
    QObject::connect(adminArea, &AdminArea::modifyObject, library, &LibraryManager::modifyObject);
    QObject::connect(adminArea, &AdminArea::removeObject, dbDelete, &DbDelete::deleteObject);
    QObject::connect(adminArea, &AdminArea::createNewObject, library, &LibraryManager::createObject);
    QObject::connect(userArea, &UserArea::prenota, dbManager, &DbManager::savePrenota);
    QObject::connect(userArea, &UserArea::restituisci, dbManager, &DbManager::saveRestituisci);
    QObject::connect(userArea, &UserArea::ascoltato, dbManager, &DbManager::saveAscoltato);
    QObject::connect(userArea, &UserArea::visto, dbManager, &DbManager::saveVisto);
    QObject::connect(userArea, &UserArea::letto, dbManager, &DbManager::saveLetto);

    return app.exec();
}

