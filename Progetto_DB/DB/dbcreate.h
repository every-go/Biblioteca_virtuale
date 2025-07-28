#ifndef DBSAVE_H
#define DBSAVE_H


#include <QObject>
#include "dbobserver.h"

class Biblioteca;
class Cartaceo;
class Multimedia;
class Libri;
class Manga;
class Cd;
class Film;
class Riviste;

class DbCreate : public QObject, public DbObserver {
   Q_OBJECT
   private:
      QList<DbObserver*> observers;
      QList<Biblioteca*> biblioteca;
      void createBiblioteca(Biblioteca* biblio);
      void createCartaceo(Cartaceo* carta);
      void createMultimedia(Multimedia* multi);
      void createRiviste(Riviste* rivista);
      void createLibri(Libri* libro);
      void createManga(Manga* manga);
      void createFilm(Film* film);
      void createCd(Cd* cd);
   public slots:
      void createnewObject(Biblioteca* biblio);
   public:
      DbCreate(QList<Biblioteca*>& newbiblioteca);
      void addObserver(DbObserver* observer);
      void notifyObservers(QList<Biblioteca*>& newBiblioteca);
      virtual void onBibliotecaUpdated(const QList<Biblioteca*>& newBiblioteca) override;
};

#endif // DBSAVE_H