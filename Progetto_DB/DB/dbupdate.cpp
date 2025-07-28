#include "dbupdate.h"
using std::string;
#include <QSqlQuery>
#include <QSqlError>

// Ho incluso solo gli header necessari per evitare dipendenze ridondanti,
// poiché le classi come Cd, Film, Manga e Riviste includono già le dipendenze comuni
// come Multimedia, Biblioteca e Libri.

#include "../modello_logico/manga.h"
#include "../modello_logico/riviste.h"
#include "../modello_logico/cd.h"
#include "../modello_logico/film.h"

DbUpdate::DbUpdate(QList<Biblioteca*>& newbiblioteca) : biblioteca(newbiblioteca) {}

void DbUpdate::addObserver(DbObserver* observer){
    observers.append(observer);
}

void DbUpdate::notifyObservers(QList<Biblioteca *> &newBiblioteca){
    //dato che so di avere solo due observer admin e user posso
    //chiamare la funzione senza fare i dynamic_cast
    for(DbObserver* observer : observers){
        observer->onBibliotecaUpdated(newBiblioteca);
    }
}

void DbUpdate::updateObject(int id, Biblioteca* biblio){
    QSqlQuery query;
    query.prepare("SELECT classe FROM BIBLIOTECA WHERE ID = :id");
    query.bindValue(":id", id);
    query.exec();
    if(!query.next())
        qDebug() << "Problema con l'id in updateObject di dbUpdate" <<query.lastError().text();
    QString classe = query.value("classe").toString().toLower();
    if (classe == "manga"){
        updateManga(static_cast<Manga*>(biblio));
    }
    else if (classe == "libri"){
        updateLibri(static_cast<Libri*>(biblio));
    }
    else if (classe == "riviste"){
        qDebug() << "Modifica rivista in atto";
        updateRiviste(static_cast<Riviste*>(biblio));
    }
    else if (classe == "film"){
        updateFilm(static_cast<Film*>(biblio));
    }
    else if (classe == "cd"){
        updateCd(static_cast<Cd*>(biblio));
    }
    notifyObservers(biblioteca);
}

void DbUpdate::updateRiviste(Riviste* rivista){
    if (!rivista) return;

    int id = rivista->getId();
    string titolo = rivista->getTitolo();
    string genere = rivista->getGenere();
    int anno = rivista->getAnno();
    double costo = rivista->getCosto();
    string immagine = rivista->getImmagine();
    int nprestiti = rivista->getNprestiti();
    string autore = rivista->getAutore();
    string editore = rivista->getEditore();
    string diffusion = rivista->diffusionToString();

    for (int i = 0; i < biblioteca.size(); ++i) {
        if (biblioteca[i]->getId() == id) {
            Biblioteca* obj = biblioteca[i];
            qDebug() << "ok rivista dbupdate";

            obj->setTitolo(titolo);
            obj->setGenere(genere);
            obj->setAnno(anno);
            obj->setCosto(costo);
            obj->setImmagine(immagine);
            obj->setNprestiti(nprestiti);

            if (auto cart = dynamic_cast<Cartaceo*>(obj)) {
                cart->setAutore(autore);
                cart->setEditore(editore);
            }

            if (auto riv = dynamic_cast<Riviste*>(obj)) {
                riv->setDiffusione(rivista->getDiffusione());
            }

            // 1) Aggiorna BIBLIOTECA
            QSqlQuery query1;
            query1.prepare("UPDATE BIBLIOTECA SET "
                        "titolo = :titolo, "
                        "genere = :genere, "
                        "anno = :anno, "
                        "costo = :costo, "
                        "immagine = :immagine, "
                        "nprestiti = :nprestiti "
                        "WHERE ID = :id");
            query1.bindValue(":titolo", QString::fromStdString(titolo));
            query1.bindValue(":genere", QString::fromStdString(genere));
            query1.bindValue(":anno", anno);
            query1.bindValue(":costo", costo);
            query1.bindValue(":immagine", QString::fromStdString(immagine));
            query1.bindValue(":nprestiti", nprestiti);
            query1.bindValue(":id", id);
            query1.exec();

            // 2) Aggiorna CARTACEO
            QSqlQuery query2;
            query2.prepare("UPDATE CARTACEO SET "
                        "autore = :autore, "
                        "editore = :editore "
                        "WHERE ID = :id");
            query2.bindValue(":autore", QString::fromStdString(autore));
            query2.bindValue(":editore", QString::fromStdString(editore));
            query2.bindValue(":id", id);
            query2.exec();

            // 3) Aggiorna RIVISTE
            QSqlQuery query3;
            query3.prepare("UPDATE RIVISTE SET "
                        "diffusion = :diffusion "
                        "WHERE ID = :id");
            query3.bindValue(":diffusion", QString::fromStdString(diffusion));
            query3.bindValue(":id", id);
            query3.exec();

        }
    }
}


void DbUpdate::updateLibri(Libri* libro){
    if (!libro) return;

    int id = libro->getId();
    string titolo = libro->getTitolo();
    string genere = libro->getGenere();
    int anno = libro->getAnno();
    double costo = libro->getCosto();
    string immagine = libro->getImmagine();
    int nprestiti = libro->getNprestiti();
    string autore = libro->getAutore();
    string editore = libro->getEditore();
    string linguaoriginale = libro->getLanguage();
    int nvolumi = libro->getNvolumi();

    for (int i = 0; i < biblioteca.size(); ++i) {
        if (biblioteca[i]->getId() == id) {
            Biblioteca* obj = biblioteca[i];
            qDebug() << "ok libro dbupdate";

            obj->setTitolo(titolo);
            obj->setGenere(genere);
            obj->setAnno(anno);
            obj->setCosto(costo);
            obj->setImmagine(immagine);
            obj->setNprestiti(nprestiti);

            if (auto cart = dynamic_cast<Cartaceo*>(obj)) {
                cart->setAutore(autore);
                cart->setEditore(editore);
            }

            if (auto lib = dynamic_cast<Libri*>(obj)) {
                lib->setLanguage(linguaoriginale);
                lib->setNvolumi(nvolumi);
            }

            // 1) Aggiorna BIBLIOTECA
            QSqlQuery query1;
            query1.prepare("UPDATE BIBLIOTECA SET "
                        "titolo = :titolo, "
                        "genere = :genere, "
                        "anno = :anno, "
                        "costo = :costo, "
                        "immagine = :immagine, "
                        "nprestiti = :nprestiti "
                        "WHERE ID = :id");
            query1.bindValue(":titolo", QString::fromStdString(titolo));
            query1.bindValue(":genere", QString::fromStdString(genere));
            query1.bindValue(":anno", anno);
            query1.bindValue(":costo", costo);
            query1.bindValue(":immagine", QString::fromStdString(immagine));
            query1.bindValue(":nprestiti", nprestiti);
            query1.bindValue(":id", id);
            query1.exec();

            // 2) Aggiorna CARTACEO
            QSqlQuery query2;
            query2.prepare("UPDATE CARTACEO SET "
                        "autore = :autore, "
                        "editore = :editore "
                        "WHERE ID = :id");
            query2.bindValue(":autore", QString::fromStdString(autore));
            query2.bindValue(":editore", QString::fromStdString(editore));
            query2.bindValue(":id", id);
            query2.exec();

            // 3) Aggiorna LIBRI
            QSqlQuery query3;
            query3.prepare("UPDATE LIBRI SET "
                        "linguaoriginale = :linguaoriginale, "
                        "nvolumi = :nvolumi "
                        "WHERE ID = :id");
            query3.bindValue(":linguaoriginale", QString::fromStdString(linguaoriginale));
            query3.bindValue(":nvolumi", nvolumi);
            query3.bindValue(":id", id);
            query3.exec();
        }
    }
}

void DbUpdate::updateManga(Manga* manga){
    int id = manga->getId();
    string titolo = manga->getTitolo();
    string genere = manga->getGenere();
    int anno = manga->getAnno();
    double costo = manga->getCosto();
    string immagine = manga->getImmagine();
    int nprestiti = manga->getNprestiti();
    string autore = manga->getAutore();
    string editore = manga->getEditore();
    string linguaoriginale = manga->getLanguage();
    int nvolumi = manga->getNvolumi();
    bool concluso = manga ->getConcluso();
    for (int i = 0; i < biblioteca.size(); ++i) {
        if(biblioteca[i]->getId() == manga->getId()){
            qDebug() << "ok manga dbupdate";
            Biblioteca* obj = biblioteca[i];

            obj->setTitolo(titolo);
            obj->setGenere(genere);
            obj->setAnno(anno);
            obj->setCosto(costo);
            obj->setImmagine(immagine);
            obj->setNprestiti(nprestiti);

            if (auto cart = dynamic_cast<Cartaceo*>(obj)) {
                cart->setAutore(autore);
                cart->setEditore(editore);
            }

            if (auto lib = dynamic_cast<Libri*>(obj)) {
                lib->setLanguage(linguaoriginale);
                lib->setNvolumi(nvolumi);
            }

            if(auto mang = dynamic_cast<Manga*>(obj)){
                mang->setConcluso(concluso);
            }

            // 1) Aggiorna BIBLIOTECA
            QSqlQuery query1;
            query1.prepare("UPDATE BIBLIOTECA SET "
                        "titolo = :titolo, "
                        "genere = :genere, "
                        "anno = :anno, "
                        "costo = :costo, "
                        "immagine = :immagine, "
                        "nprestiti = :nprestiti "
                        "WHERE ID = :id");
            query1.bindValue(":titolo", QString::fromStdString(titolo));
            query1.bindValue(":genere", QString::fromStdString(genere));
            query1.bindValue(":anno", anno);
            query1.bindValue(":costo", costo);
            query1.bindValue(":immagine", QString::fromStdString(immagine));
            query1.bindValue(":nprestiti", nprestiti);
            query1.bindValue(":id", id);
            query1.exec();

            // 2) Aggiorna CARTACEO
            QSqlQuery query2;
            query2.prepare("UPDATE CARTACEO SET "
                        "autore = :autore, "
                        "editore = :editore "
                        "WHERE ID = :id");
            query2.bindValue(":autore", QString::fromStdString(autore));
            query2.bindValue(":editore", QString::fromStdString(editore));
            query2.bindValue(":id", id);
            query2.exec();

            // 3) Aggiorna LIBRI
            QSqlQuery query3;
            query3.prepare("UPDATE LIBRI SET "
                        "linguaoriginale = :linguaoriginale, "
                        "nvolumi = :nvolumi "
                        "WHERE ID = :id");
            query3.bindValue(":linguaoriginale", QString::fromStdString(linguaoriginale));
            query3.bindValue(":nvolumi", nvolumi);
            query3.bindValue(":id", id);
            query3.exec();

            // 4) Aggiorna MANGA
            QSqlQuery query4;
            query4.prepare("UPDATE MANGA SET "
                        "concluso = :concluso "
                        "WHERE ID = :id");
            query4.bindValue(":concluso", concluso);
            query4.bindValue(":id", id);
            query4.exec();
        }
    }
}

void DbUpdate::updateFilm(Film* film){
    int id = film->getId();
    string titolo = film->getTitolo();
    string genere = film->getGenere();
    int anno = film->getAnno();
    double costo = film->getCosto();
    string immagine = film->getImmagine();
    int nprestiti = film->getNprestiti();
    int durata = film->getDurata();
    string studio = film->getStudio();
    string regista = film->getRegista();
    string attoreprotagonista = film->getAttore();
    for (int i = 0; i < biblioteca.size(); ++i) {
        if(biblioteca[i]->getId() == film->getId()){
            qDebug() << "ok film dbupdate";
            Biblioteca* obj = biblioteca[i];

            obj->setTitolo(titolo);
            obj->setGenere(genere);
            obj->setAnno(anno);
            obj->setCosto(costo);
            obj->setImmagine(immagine);
            obj->setNprestiti(nprestiti);

            if(auto multi = dynamic_cast<Multimedia*>(obj)){
                multi->setDurata(durata);
                multi->setStudio(studio);
            }

            if (auto fil = dynamic_cast<Film*>(obj)){
                fil->setAttore(attoreprotagonista);
                fil->setRegista(regista);
            }

            // 1) Aggiorna BIBLIOTECA
            QSqlQuery query1;
            query1.prepare("UPDATE BIBLIOTECA SET "
                        "titolo = :titolo, "
                        "genere = :genere, "
                        "anno = :anno, "
                        "costo = :costo, "
                        "immagine = :immagine, "
                        "nprestiti = :nprestiti "
                        "WHERE ID = :id");
            query1.bindValue(":titolo", QString::fromStdString(titolo));
            query1.bindValue(":genere", QString::fromStdString(genere));
            query1.bindValue(":anno", anno);
            query1.bindValue(":costo", costo);
            query1.bindValue(":immagine", QString::fromStdString(immagine));
            query1.bindValue(":nprestiti", nprestiti);
            query1.bindValue(":id", id);
            query1.exec();

            // 2) Aggiorna MULTIMEDIA
            QSqlQuery query2;
            query2.prepare("UPDATE MULTIMEDIA SET "
                        "durata = :durata, "
                        "studio = :studio "
                        "WHERE ID = :id");
            query2.bindValue(":durata", durata);
            query2.bindValue(":studio", QString::fromStdString(studio));
            query2.bindValue(":id", id);
            query2.exec();

            // 3) Aggiorna FILM
            QSqlQuery query3;
            query3.prepare("UPDATE FILM SET "
                        "attoreprotagonista = :attoreprotagonista, "
                        "regista = :regista "
                        "WHERE ID = :id");
            query3.bindValue(":attoreprotagonista", QString::fromStdString(attoreprotagonista));
            query3.bindValue(":regista", QString::fromStdString(regista));
            query3.bindValue(":id", id);
            query3.exec();
        }
    }
}

void DbUpdate::updateCd(Cd* cd){
    int id = cd->getId();
    string titolo = cd->getTitolo();
    string genere = cd->getGenere();
    int anno = cd->getAnno();
    double costo = cd->getCosto();
    string immagine = cd->getImmagine();
    int nprestiti = cd->getNprestiti();
    int durata = cd->getDurata();
    string studio = cd->getStudio();
    int ntracce = cd->getTracce();
    string artista = cd->getArtista();
    for (int i = 0; i < biblioteca.size(); ++i) {
        if(biblioteca[i]->getId() == cd->getId()){
            qDebug() << "ok cd dbupdate";
            Biblioteca* obj = biblioteca[i];

            obj->setTitolo(titolo);
            obj->setGenere(genere);
            obj->setAnno(anno);
            obj->setCosto(costo);
            obj->setImmagine(immagine);
            obj->setNprestiti(nprestiti);

            if(auto multi = dynamic_cast<Multimedia*>(obj)){
                multi->setDurata(durata);
                multi->setStudio(studio);
            }

            if (auto cd = dynamic_cast<Cd*>(obj)){
                cd->setTracce(ntracce);
                cd->setArtista(artista);
            }

            // 1) Aggiorna BIBLIOTECA
            QSqlQuery query1;
            query1.prepare("UPDATE BIBLIOTECA SET "
                        "titolo = :titolo, "
                        "genere = :genere, "
                        "anno = :anno, "
                        "costo = :costo, "
                        "immagine = :immagine, "
                        "nprestiti = :nprestiti "
                        "WHERE ID = :id");
            query1.bindValue(":titolo", QString::fromStdString(titolo));
            query1.bindValue(":genere", QString::fromStdString(genere));
            query1.bindValue(":anno", anno);
            query1.bindValue(":costo", costo);
            query1.bindValue(":immagine", QString::fromStdString(immagine));
            query1.bindValue(":nprestiti", nprestiti);
            query1.bindValue(":id", id);
            query1.exec();

            // 2) Aggiorna MULTIMEDIA
            QSqlQuery query2;
            query2.prepare("UPDATE MULTIMEDIA SET "
                        "durata = :durata, "
                        "studio = :studio "
                        "WHERE ID = :id");
            query2.bindValue(":durata", durata);
            query2.bindValue(":studio", QString::fromStdString(studio));
            query2.bindValue(":id", id);
            query2.exec();

            // 3) Aggiorna CD
            QSqlQuery query3;
            query3.prepare("UPDATE CD SET "
                           "artista = :artista, "
                           "ntracce = :ntracce "
                           "WHERE ID = :id");
            query3.bindValue(":artista", QString::fromStdString(artista));
            query3.bindValue(":ntracce", ntracce);
            query3.bindValue(":id", id);
            query3.exec();
        }
    }
}