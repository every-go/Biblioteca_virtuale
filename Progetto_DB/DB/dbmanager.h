#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QList>
#include <QSqlQuery>
#include <QObject>
#include "dbobserver.h"

class Biblioteca;
class Cartaceo;
class Cd;
class Film;
class Manga;
class Riviste;
class Libri;

class DbManager : public QObject {
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
      BibliotecaInfo loadBiblioteca(const QSqlQuery& query);
      CartaceoInfo loadCartaceo(int id);
      MultimediaInfo loadMultimedia(int id);
      Biblioteca* loadRiviste(int id, const QSqlQuery& query);
      Biblioteca* loadLibri(int id, const QSqlQuery& query);
      Biblioteca* loadCd(int id, const QSqlQuery& query);
      Biblioteca* loadFilm(int id, const QSqlQuery& query);
      Biblioteca* loadManga(int id, const QSqlQuery& query);
   public slots:
      void deleteObject(int id);
      void savePrenota(int id);
      void saveLetto(int id, Cartaceo* carta);
      void saveAscoltato(int id, Cd* cd);
      void saveVisto(int id, Film* film);
      void savenewObject(Biblioteca* biblio);
      void updateObject(int id, Biblioteca* biblio);
   public:
      DbManager() = default;
      void addObserver(DbObserver* observer);
      void notifyObservers(QList<Biblioteca*>& newBiblioteca);
      bool connettiAlDatabase();
      QList<Biblioteca*> loadBibliotecaFromDb();
   };

#endif
