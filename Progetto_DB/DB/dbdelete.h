#ifndef DBDELETE_H
#define DBDELETE_H

#include <QObject>
#include "dbobserver.h"

class Biblioteca;

class DbDelete : public QObject {
   Q_OBJECT
   private:
      QList<DbObserver*> observers;
      QList<Biblioteca*> biblioteca;
   public slots:
      void deleteObject(int id, Biblioteca* biblio);
   public:
      DbDelete(QList<Biblioteca*>& newbiblioteca);
      void addObserver(DbObserver* observer);
      void notifyObservers(QList<Biblioteca*>& newBiblioteca);
};

#endif // DBDELETE_H