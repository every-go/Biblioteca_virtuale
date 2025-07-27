#ifndef DBSAVE_H
#define DBSAVE_H


#include <QObject>
#include "dbobserver.h"

class DbCreate : public QObject {
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
      BibliotecaInfo createBiblioteca();
      CartaceoInfo createCartaceo();
      MultimediaInfo createMultimedia();
      Biblioteca* createRiviste();
      Biblioteca* createLibri();
      Biblioteca* createManga();
      Biblioteca* createFilm();
      Biblioteca* createCd();
   public slots:
      void createnewObject(Biblioteca* biblio);
   public:
      DbCreate() = default;
      void addObserver(DbObserver* observer);
      void notifyObservers(QList<Biblioteca*>& newBiblioteca);
};

#endif // DBSAVE_H