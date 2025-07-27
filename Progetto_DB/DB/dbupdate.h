#ifndef DBUPDATE_H
#define DBUPDATE_H

#include <QObject>
#include "dbobserver.h"

class Biblioteca;

class DbUpdate : public QObject {
   Q_OBJECT
   private:
      QList<DbObserver*> observers;
      struct BibliotecaInfo {
         int id_db;
         QString titolo;
         QString genere;
         int anno;
         double costo;
         bool disponibile;
         int copie;
         int nprestiti;
         QString immagine;
      };
      struct CartaceoInfo {
         QString autore;
         QString editore;
         bool letto;
      };
      struct MultimediaInfo {
         int durata;
         QString studio;
      };
      BibliotecaInfo updateBiblioteca();
      CartaceoInfo updateCartaceo();
      MultimediaInfo updateMultimedia();
      Biblioteca* updateRiviste();
      Biblioteca* updateLibri();
      Biblioteca* updateManga();
      Biblioteca* updateFilm();
      Biblioteca* updateCd();
   public slots:
      void updateObject(int id, Biblioteca* biblio);
   public:
      DbUpdate() = default;
      void addObserver(DbObserver* observer);
      void notifyObservers(QList<Biblioteca*>& newBiblioteca);
};

#endif // DBUPDATE_H