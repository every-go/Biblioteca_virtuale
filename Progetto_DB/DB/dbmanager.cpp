#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include "dbmanager.h"

// Ho incluso solo gli header necessari per evitare dipendenze ridondanti,
// poiché le classi come Cd, Film, Manga e Riviste includono già le dipendenze comuni
// come Multimedia, Biblioteca e Libri.

#include "../modello_logico/manga.h"
#include "../modello_logico/riviste.h"
#include "../modello_logico/cd.h"
#include "../modello_logico/film.h"

#define PG_HOST "localhost"
#define PG_USER "postgres"
#define PG_DB "biblioteca"
#define PG_PASS "password"
#define PG_PORT 5432

bool DbManager::connettiAlDatabase() {
   QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");  // driver PostgreSQL
   db.setHostName(PG_HOST);          // o IP se in rete
   db.setDatabaseName(PG_DB);  // il nome del tuo database
   db.setUserName(PG_USER);         // es: "postgres"
   db.setPassword(PG_PASS);       // es: "admin"

   if (!db.open())
      return false;
   
   return true;
}

QList<Biblioteca*> DbManager::loadBibliotecaFromDb() {
   QList<Biblioteca*> biblioteca;
   QSqlQuery query;
   query.prepare("SELECT * FROM BIBLIOTECA");
   if (!query.exec()) {
      qDebug() << "Errore nella query:" << query.lastError().text();
      return biblioteca;
   }
   while (query.next()) {
      Biblioteca* biblio = nullptr;
      QString type = query.value("classe").toString().toLower();
      int id = query.value("id").toInt();
      if(type == "cd")
         biblio = loadCd(id, query);
      else if(type == "film")
         biblio = loadFilm(id, query);
      else if(type == "manga")
         biblio = loadManga(id, query);
      else if(type == "riviste")
         biblio = loadRiviste(id, query);
      else if(type == "libri")
         biblio = loadLibri(id, query);
      else
         // Gestione di tipo sconosciuto
         qDebug() << "Tipo sconosciuto:" << type;
      if (biblio)
         biblioteca.append(biblio);
      else
         qDebug() << "Errore nel caricamento dell'oggetto dalla query:" << query.lastError().text();
   }
   return biblioteca;
}

DbManager::BibliotecaInfo DbManager::loadBiblioteca(const QSqlQuery& query) {
   DbManager::BibliotecaInfo info;
   info.id_db = query.value("id").toInt();
   info.titolo = query.value("titolo").toString();
   info.genere = query.value("genere").toString();
   info.anno = query.value("anno").toInt();
   info.costo = query.value("costo").toDouble();
   info.disponibile = query.value("disponibile").toBool();
   info.copie = query.value("copie_disponibili").toInt();
   info.nprestiti = query.value("nprestiti").toInt();
   info.immagine = query.value("immagine").toString();
   return info;
}

DbManager::CartaceoInfo DbManager::loadCartaceo(int id) {
   DbManager::CartaceoInfo info;
   QSqlQuery query;
   query.prepare("SELECT * FROM CARTACEO WHERE id = :id");
   query.bindValue(":id", id);
   query.exec();
   if (!query.next()) {
      qWarning() << "Nessun record trovato per l'ID:" << id;
      return info; // o gestisci l'errore come preferisci
   }
   info.autore = query.value("autore").toString();
   info.editore = query.value("editore").toString();
   info.letto = query.value("letto").toBool();
   return info;
}

DbManager::MultimediaInfo DbManager::loadMultimedia(int id) {
   DbManager::MultimediaInfo info;
   QSqlQuery query;
   query.prepare("SELECT * FROM MULTIMEDIA WHERE id = :id");
   query.bindValue(":id", id);
   query.exec();
   if (!query.next()) {
      qWarning() << "Nessun record trovato per l'ID:" << id;
      return info; // o gestisci l'errore come preferisci
   }
   info.durata = query.value("durata").toInt();
   info.studio = query.value("studio").toString();
   return info;
}

Biblioteca* DbManager::loadFilm(int id, const QSqlQuery& query) {
   DbManager::BibliotecaInfo base = loadBiblioteca(query);
   DbManager::MultimediaInfo multibase = loadMultimedia(base.id_db);
   QSqlQuery query2;
   query2.prepare("SELECT * FROM FILM WHERE id = :id");
   query2.bindValue(":id", id);
   query2.exec();
   QString regista;
   QString protagonista;
   bool visto;
   if(query2.next()){
      regista = query2.value("regista").toString();
      protagonista = query2.value("attoreprotagonista").toString();
      visto = query2.value("visto").toBool();
   }
   else {
      qWarning() << "Nessun film trovato con id:" << id;
      return nullptr; // Gestione dell'errore se il film non esiste
   }
   return new Film(base.id_db, base.titolo.toStdString(), base.genere.toStdString(), base.anno,
                   base.costo, base.disponibile, base.copie, base.nprestiti,
                   base.immagine.toStdString(), multibase.durata, multibase.studio.toStdString(),
                   regista.toStdString(), protagonista.toStdString(), visto);
}

Biblioteca* DbManager::loadCd(int id, const QSqlQuery& query) {
   DbManager::BibliotecaInfo base = loadBiblioteca(query);
   DbManager::MultimediaInfo multibase = loadMultimedia(base.id_db);
   QSqlQuery query2;
   query2.prepare("SELECT * FROM CD WHERE id = :id");
   query2.bindValue(":id", id);
   query2.exec();
   QString artista;
   bool ascoltato;
   int ntracce;
   if(query2.next()){
      artista = query2.value("artista").toString();
      ascoltato = query2.value("ascoltato").toBool();
      ntracce = query2.value("ntracce").toInt();
   }
   else {
      qWarning() << "Nessun CD trovato con id:" << id;
      return nullptr; // Gestione dell'errore se il CD non esiste
   }
   return new Cd(base.id_db, base.titolo.toStdString(), base.genere.toStdString(), base.anno,
                   base.costo, base.disponibile, base.copie, base.nprestiti,
                   base.immagine.toStdString(), multibase.durata, multibase.studio.toStdString(),
                   artista.toStdString(), ntracce, ascoltato);
}

Biblioteca* DbManager::loadRiviste(int id, const QSqlQuery& query) {
   DbManager::BibliotecaInfo base = loadBiblioteca(query);
   DbManager::CartaceoInfo cartbase = loadCartaceo(base.id_db);
   QSqlQuery query2;
   query2.prepare("SELECT * FROM RIVISTE WHERE id = :id");
   query2.bindValue(":id", id);
   query2.exec();
   Riviste::Diffusione diff;
   if(query2.next()){
      QString diffusione = query2.value("diffusion").toString();
      if (diffusione == "Provinciale")
         diff = Riviste::Provinciale;
      else if (diffusione == "Regionale")
         diff = Riviste::Regionale;
      else if (diffusione == "Nazionale")
         diff = Riviste::Nazionale;
      else if (diffusione == "Internazionale")
         diff = Riviste::Internazionale;
      else{
         //valore default
         diff= Riviste::Provinciale;
         qWarning() << "Nessun tipo di diffusione presente";
      }
   }
   else{
      qWarning() << "Nessun record trovato per l'ID:" << id;
      return nullptr; // o gestisci l'errore come preferisci
   }
   return new Riviste(base.id_db, base.titolo.toStdString(), base.genere.toStdString(), base.anno,
                   base.costo, base.disponibile, base.copie, base.nprestiti,
                   base.immagine.toStdString(), cartbase.autore.toStdString(), cartbase.editore.toStdString(),
                   cartbase.letto, diff);
}

Biblioteca* DbManager::loadLibri(int id, const QSqlQuery& query) {
   DbManager::BibliotecaInfo base = loadBiblioteca(query);
   DbManager::CartaceoInfo cartbase = loadCartaceo(base.id_db);
   QSqlQuery query2;
   query2.prepare("SELECT * FROM LIBRI WHERE id = :id");
   query2.bindValue(":id", id);
   query2.exec();
   QString language;
   int number;
   if(query2.next()){
      language = query2.value("linguaoriginale").toString();
      number = query2.value("nvolumi").toInt();
   }
   else {
      qWarning() << "Nessun libro trovato con id:" << id;
      return nullptr; // Gestione dell'errore se il libro non esiste
   }
   return new Libri(base.id_db, base.titolo.toStdString(), base.genere.toStdString(), base.anno,
                   base.costo, base.disponibile, base.copie, base.nprestiti,
                   base.immagine.toStdString(), cartbase.autore.toStdString(), cartbase.editore.toStdString(),
                   cartbase.letto, language.toStdString(), number);
}

Biblioteca* DbManager::loadManga(int id, const QSqlQuery& query) {
   DbManager::BibliotecaInfo base = loadBiblioteca(query);
   DbManager::CartaceoInfo cartbase = loadCartaceo(base.id_db);
   QSqlQuery query2;
   query2.prepare("SELECT * FROM MANGA M JOIN LIBRI L ON L.id = M.id WHERE M.id = :id");
   query2.bindValue(":id", id);
   query2.exec();
   QString language;
   int number;
   bool concluso;
   if(query2.next()){
      language = query2.value("linguaoriginale").toString();
      number = query2.value("nvolumi").toInt();
      concluso = query2.value("concluso").toBool();
   }
   else {
      qWarning() << "Nessun manga trovato con id:" << id;
      return nullptr; // Gestione dell'errore se il manga non esiste
   }
   return new Manga(base.id_db, base.titolo.toStdString(), base.genere.toStdString(), base.anno,
                   base.costo, base.disponibile, base.copie, base.nprestiti,
                   base.immagine.toStdString(), cartbase.autore.toStdString(), cartbase.editore.toStdString(),
                   cartbase.letto, language.toStdString(), number, concluso);
}

void DbManager::savePrenota(int id){
   QSqlQuery query;
   query.prepare("UPDATE Biblioteca SET copie_disponibili = copie_disponibili - 1 WHERE id = :id");
   query.bindValue(":id", id);
   if (!query.exec()) {
      qDebug() << "Errore nell'aggiornamento del prestito:" << query.lastError().text();
      return;
   }
   QSqlQuery checkQuery;
   checkQuery.prepare("SELECT copie_disponibili FROM Biblioteca WHERE id = :id");
   checkQuery.bindValue(":id", id);
   if (!checkQuery.exec()) {
      qDebug() << "Errore nella query di controllo:" << checkQuery.lastError().text();
      return;
   }
   if (!checkQuery.next()) {
      qDebug() << "Nessun record trovato per l'ID:" << id;
      return;
   }
   if(checkQuery.value("copie_disponibili").toInt() == 0) {
      qDebug() << "Nessuna copia disponibile, aggiornamento della disponibilità a FALSE";
      QSqlQuery updateQuery;
      updateQuery.prepare("UPDATE Biblioteca SET disponibile = FALSE WHERE id = :id");
      updateQuery.bindValue(":id", id);
      if (!updateQuery.exec())
         qDebug() << "Errore nell'aggiornamento della disponibilità:" << updateQuery.lastError().text();
   }
}

void DbManager::saveRestituisci(int id){
   QSqlQuery query;
   query.prepare("UPDATE Biblioteca SET copie_disponibili = copie_disponibili + 1 WHERE id = :id");
   query.bindValue(":id", id);
   if (!query.exec()) {
      qDebug() << "Errore nell'aggiornamento del prestito:" << query.lastError().text();
      return;
   }
   qDebug() << "Copie disponibili, aggiornamento della disponibilità a TRUE";
   QSqlQuery updateQuery;
   updateQuery.prepare("UPDATE Biblioteca SET disponibile = TRUE WHERE id = :id");
   updateQuery.bindValue(":id", id);
   if (!updateQuery.exec())
      qDebug() << "Errore nell'aggiornamento della disponibilità:" << updateQuery.lastError().text();
}

void DbManager::saveLetto(int id, Cartaceo* carta){
   QSqlQuery query;
   query.prepare("UPDATE Cartaceo SET letto = :letto WHERE id = :id");
   query.bindValue(":letto", carta->getLetto());
   query.bindValue(":id", id);
   if (!query.exec()) {
      qDebug() << "Errore nell'aggiornamento del libro letto:" << query.lastError().text();
      return;
   }
}

void DbManager::saveAscoltato(int id, Cd* cd){
   QSqlQuery query;
   query.prepare("UPDATE Cd SET ascoltato = :ascoltato WHERE id = :id");
   query.bindValue(":ascoltato", cd->getAscoltato());
   query.bindValue(":id", id);
   if (!query.exec()) {
      qDebug() << "Errore nell'aggiornamento del CD ascoltato:" << query.lastError().text();
      return;
   }
}

void DbManager::saveVisto(int id, Film* film){
   QSqlQuery query;
   query.prepare("UPDATE Film SET visto = :visto WHERE id = :id");
   query.bindValue(":visto", film->getVisto());
   query.bindValue(":id", id);
   if (!query.exec()) {
      qDebug() << "Errore nell'aggiornamento del film visto:" << query.lastError().text();
      return;
   }
}