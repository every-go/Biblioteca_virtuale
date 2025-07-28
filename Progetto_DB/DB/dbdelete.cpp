#include "dbdelete.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

#include "../modello_logico/biblioteca.h"

DbDelete::DbDelete(QList<Biblioteca*>& newbiblioteca) : biblioteca(newbiblioteca) {}

void DbDelete::addObserver(DbObserver* observer){
    observers.append(observer);
}

void DbDelete::notifyObservers(QList<Biblioteca *> &newBiblioteca){
    //dato che so di avere solo due observer admin e user posso
    //chiamare la funzione senza fare i dynamic_cast
    for(DbObserver* observer : observers){
        observer->onBibliotecaUpdated(newBiblioteca);
    }
}

void DbDelete::deleteObject(int id, Biblioteca* biblio){
    QSqlQuery query;
    query.prepare("DELETE FROM Biblioteca WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec())
        qDebug() << "Errore DELETE:" << query.lastError().text();

    // Rimuovi dalla lista e libera la memoria
    for (int i = 0; i < biblioteca.size(); ++i) {
        if (biblioteca[i]->getId() == biblio->getId()) {
            delete biblioteca[i];
            biblioteca.removeAt(i);
            break;
        }
    }

    notifyObservers(biblioteca);
}