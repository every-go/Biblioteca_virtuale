#include "dbcreate.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

// Ho incluso solo gli header necessari per evitare dipendenze ridondanti,
// poiché le classi come Cd, Film, Manga e Riviste includono già le dipendenze comuni
// come Multimedia, Biblioteca e Libri.

#include "../modello_logico/manga.h"
#include "../modello_logico/riviste.h"
#include "../modello_logico/cd.h"
#include "../modello_logico/film.h"

DbCreate::DbCreate(QList<Biblioteca*>& newbiblioteca) : biblioteca(newbiblioteca) {}

void DbCreate::addObserver(DbObserver* observer){
    observers.append(observer);
}

void DbCreate::notifyObservers(QList<Biblioteca *>& newBiblioteca){
    //dato che so di avere solo due observer admin e user posso
    //chiamare la funzione senza fare i dynamic_cast
    for(DbObserver* observer : observers){
        observer->onBibliotecaUpdated(newBiblioteca);
    }
}

void DbCreate::onBibliotecaUpdated(const QList<Biblioteca*>& newBiblioteca){
    biblioteca = newBiblioteca;
}

void DbCreate::createnewObject(Biblioteca* biblio){
    if(biblio){
        if(auto manga = dynamic_cast<Manga*>(biblio)){
            createManga(manga);
        }
        else if(auto libro = dynamic_cast<Libri*>(biblio)){
            createLibri(libro);
        }
        else if(auto rivista = dynamic_cast<Riviste*>(biblio)){
            createRiviste(rivista);
        }
        else if(auto cd = dynamic_cast<Cd*>(biblio)){
            createCd(cd);
        }
        else if(auto film = dynamic_cast<Film*>(biblio)){
            createFilm(film);
        }
    }
    notifyObservers(biblioteca);
}

void DbCreate::createBiblioteca(Biblioteca* biblio){
    QSqlQuery query;
    query.prepare("UPDATE Biblioteca SET "
                  "titolo = :titolo, "
                  "genere = :genere, "
                  "anno = :anno, "
                  "costo = :costo, "
                  "disponibile = :disponibile, "
                  "copie_disponibili = :copie_disponibili, "
                  "nprestiti = :nprestiti, "
                  "immagine = :immagine "
                  "WHERE id = :id");

    query.bindValue(":titolo", QString::fromStdString(biblio->getTitolo()));
    query.bindValue(":genere", QString::fromStdString(biblio->getGenere()));
    query.bindValue(":anno", biblio->getAnno());
    query.bindValue(":costo", biblio->getCosto());
    query.bindValue(":disponibile", biblio->getDisponibilita());
    query.bindValue(":copie_disponibili", biblio->getCopie());
    query.bindValue(":nprestiti", biblio->getNprestiti());
    query.bindValue(":immagine", QString::fromStdString(biblio->getImmagine()));
    query.bindValue(":id", biblio->getId());

    if (!query.exec()) {
        qDebug() << "Errore UPDATE Biblioteca:" << query.lastError().text();
    }
}

void DbCreate::createMultimedia(Multimedia* multi){
    QSqlQuery query;
    query.prepare("INSERT INTO Multimedia (id, durata, studio) "
                  "VALUES (:id, :durata, :studio)");
    query.bindValue(":durata", multi->getDurata());
    query.bindValue(":studio", QString::fromStdString(multi->getStudio()));
    query.bindValue(":id", multi->getId());

    if(!query.exec()) {
        qDebug() << "Errore UPDATE Multimedia:" << query.lastError().text();
    }
}

void DbCreate::createCartaceo(Cartaceo* carta){
    QSqlQuery query;
    query.prepare("INSERT INTO Cartaceo (id, autore, editore, letto) "
                  "VALUES (:id, :autore, :editore, :letto)");
    query.bindValue(":autore", QString::fromStdString(carta->getAutore()));
    query.bindValue(":editore", QString::fromStdString(carta->getEditore()));
    query.bindValue(":letto", carta->getLetto());
    query.bindValue(":id", carta->getId());

    if(!query.exec()) {
        qDebug() << "Errore UPDATE Cartaceo:" << query.lastError().text();
    }
}

void DbCreate::createRiviste(Riviste* rivista){
    qDebug() << "Create Riviste";
    QSqlQuery preQuery;
    preQuery.prepare("INSERT INTO Biblioteca (classe) VALUES ('riviste') RETURNING id;");
    if (preQuery.exec() && preQuery.next()) {
        int newId = preQuery.value(0).toInt();
        rivista->setId(newId);  // imposti l'id effettivo
    }
    else {
        qDebug() << "Errore INSERT Riviste:" << preQuery.lastError().text();
    }
    createBiblioteca(rivista);
    createCartaceo(rivista);

    QSqlQuery query;
    query.prepare("INSERT INTO riviste (id, diffusion)"
                  "VALUES (:id, CAST(:diffusione AS Diffusione))");

    query.bindValue(":id", rivista->getId());
    query.bindValue(":diffusione", QString::fromStdString(rivista->diffusionToString()));

    if (!query.exec()) {
        qDebug() << "Errore INSERT Riviste:" << query.lastError().text();
    }
    else{
        biblioteca.append(rivista);
    }
}

void DbCreate::createLibri(Libri* libro){
    QSqlQuery preQuery;
    preQuery.prepare("INSERT INTO Biblioteca (classe) VALUES ('libri') RETURNING id;");
    if (preQuery.exec() && preQuery.next()) {
        int newId = preQuery.value(0).toInt();
        libro->setId(newId);  // imposti l'id effettivo
    }
    else {
        qDebug() << "Errore INSERT Libri:" << preQuery.lastError().text();
    }

    createBiblioteca(libro);
    createCartaceo(libro);

    QSqlQuery query;
    query.prepare("INSERT INTO Libri (id, linguaoriginale, nvolumi) "
                  "VALUES (:id, :linguaoriginale, :nvolumi)");
    query.bindValue(":linguaoriginale", QString::fromStdString(libro->getLanguage()));
    query.bindValue(":nvolumi", libro->getNvolumi());
    query.bindValue(":id", libro->getId());
    if(!query.exec()) {
        qDebug() << "Errore INSERT Libri:" << query.lastError().text();
    }
    else{
        biblioteca.append(libro);
    }
}

void DbCreate::createManga(Manga* manga){
    QSqlQuery preQuery;
    preQuery.prepare("INSERT INTO Biblioteca (classe) VALUES ('manga') RETURNING id;");
    if (preQuery.exec() && preQuery.next()) {
        int newId = preQuery.value(0).toInt();
        manga->setId(newId);  // imposti l'id effettivo
    }
    else {
        qDebug() << "Errore INSERT Manga:" << preQuery.lastError().text();
    }

    createBiblioteca(manga);
    createCartaceo(manga);

    QSqlQuery query;
    query.prepare("INSERT INTO Libri (id, linguaoriginale, nvolumi) "
                  "VALUES (:id, :linguaoriginale, :nvolumi)");
    query.bindValue(":linguaoriginale", QString::fromStdString(manga->getLanguage()));
    query.bindValue(":nvolumi", manga->getNvolumi());
    query.bindValue(":id", manga->getId());
    if(!query.exec()) {
        qDebug() << "Errore INSERT Libri nel Manga:" << query.lastError().text();
        return;
    }

    QSqlQuery query2;
    query2.prepare("INSERT INTO MANGA (id, concluso) "
                   "VALUES (:id, :concluso)");
    query2.bindValue(":concluso", manga->getConcluso());
    query2.bindValue(":id", manga->getId());
    if(!query2.exec()) {
        qDebug() << "Errore INSERT Manga:" << query.lastError().text(); 
    }
    else{
        biblioteca.append(manga);
    }
}

void DbCreate::createFilm(Film* film){
    QSqlQuery preQuery;
    preQuery.prepare("INSERT INTO Biblioteca (classe) VALUES ('film') RETURNING id;");
    if (preQuery.exec() && preQuery.next()) {
        int newId = preQuery.value(0).toInt();
        film->setId(newId);  // imposti l'id effettivo
    }
    else {
        qDebug() << "Errore INSERT Film:" << preQuery.lastError().text();
    }

    createBiblioteca(film);
    createMultimedia(film);

    QSqlQuery query;
    query.prepare("INSERT INTO Film (id, regista, attoreprotagonista, visto) "
                  "VALUES (:id, :regista, :attoreprotagonista, :visto)");
    query.bindValue(":regista", QString::fromStdString(film->getRegista()));
    query.bindValue(":attoreprotagonista", QString::fromStdString(film->getAttore()));
    query.bindValue(":visto", film->getVisto());
    query.bindValue(":id", film->getId());
    if(!query.exec()) {
        qDebug() << "Errore UPDATE Film:" << query.lastError().text();
    }
    else{
        biblioteca.append(film);
    }
}

void DbCreate::createCd(Cd* cd){
    QSqlQuery preQuery;
    preQuery.prepare("INSERT INTO Biblioteca (classe) VALUES ('cd') RETURNING id;");
    if (preQuery.exec() && preQuery.next()) {
        int newId = preQuery.value(0).toInt();
        cd->setId(newId);  // imposti l'id effettivo
    }
    else {
        qDebug() << "Errore INSERT Cd:" << preQuery.lastError().text();
    }

    createBiblioteca(cd);
    createMultimedia(cd);

    QSqlQuery query;
    query.prepare("INSERT INTO CD (id, artista, ntracce, ascoltato) "
                  "VALUES (:id, :artista, :ntracce, :ascoltato)");
    query.bindValue(":artista", QString::fromStdString(cd->getArtista()));
    query.bindValue(":ntracce", cd->getTracce());
    query.bindValue(":ascoltato", cd->getAscoltato());
    query.bindValue(":id", cd->getId());
    if(!query.exec()) {
        qDebug() << "Errore UPDATE Cd:" << query.lastError().text();
    }
    else{
        biblioteca.append(cd);
    }
}