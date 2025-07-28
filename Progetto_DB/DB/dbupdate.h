#ifndef DBUPDATE_H
#define DBUPDATE_H

#include <QObject>
#include "dbobserver.h"

class Biblioteca;
class Libri;
class Manga;
class Riviste;
class Cd;
class Film;

class DbUpdate : public QObject {
   Q_OBJECT
   private:
      QList<DbObserver*> observers;
      QList<Biblioteca*> biblioteca;

      void updateRiviste(Riviste* rivista);
      void updateLibri(Libri* libro);
      void updateManga(Manga* manga);
      void updateFilm(Film* film);
      void updateCd(Cd* cd);
   public slots:
      void updateObject(int id, Biblioteca* biblio);
   public:
      DbUpdate(QList<Biblioteca*>& newbiblioteca);
      void addObserver(DbObserver* observer);
      void notifyObservers(QList<Biblioteca*>& newBiblioteca);
};

#endif // DBUPDATE_H