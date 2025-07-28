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

void DbUpdate::onBibliotecaUpdated(const QList<Biblioteca*>& newBiblioteca){
    biblioteca = newBiblioteca;
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

void DbUpdate::updateBiblioteca(Biblioteca* biblio){
    if (!biblio) return;

    int id = biblio->getId();

    for (int i = 0; i < biblioteca.size(); ++i) {
        if (biblioteca[i]->getId() == id) {
            Biblioteca* obj = biblioteca[i];
            qDebug() << "ok biblioteca dbupdate";

            obj->setTitolo(biblio->getTitolo());
            obj->setGenere(biblio->getGenere());
            obj->setAnno(biblio->getAnno());
            obj->setCosto(biblio->getCosto());
            obj->setImmagine(biblio->getImmagine());
            obj->setNprestiti(biblio->getNprestiti());

            // Aggiorna BIBLIOTECA
            QSqlQuery query;
            query.prepare("UPDATE BIBLIOTECA SET "
                        "titolo = :titolo, "
                        "genere = :genere, "
                        "anno = :anno, "
                        "costo = :costo, "
                        "immagine = :immagine, "
                        "nprestiti = :nprestiti "
                        "WHERE ID = :id");
            query.bindValue(":titolo", QString::fromStdString(biblio->getTitolo()));
            query.bindValue(":genere", QString::fromStdString(biblio->getGenere()));
            query.bindValue(":anno", biblio->getAnno());
            query.bindValue(":costo", biblio->getCosto());
            query.bindValue(":immagine", QString::fromStdString(biblio->getImmagine()));
            query.bindValue(":nprestiti", biblio->getNprestiti());
            query.bindValue(":id", id);
            query.exec();
        }
    }
}

void DbUpdate::updateCartaceo(Cartaceo* carta){
    if(!carta) return;

    int id = carta->getId();

    for(int i = 0; i < biblioteca.size(); ++i){
        if(biblioteca[i]->getId() == id){
            Biblioteca* obj = biblioteca[i];
            qDebug() << "ok cartaceo dbupdate";

            auto cart = dynamic_cast<Cartaceo*>(obj);
            cart->setAutore(carta->getAutore());
            cart->setEditore(carta->getEditore());

            // Aggiorna CARTACEO
            QSqlQuery query;
            query.prepare("UPDATE CARTACEO SET "
                        "autore = :autore, "
                        "editore = :editore "
                        "WHERE ID = :id");
            query.bindValue(":autore", QString::fromStdString(carta->getAutore()));
            query.bindValue(":editore", QString::fromStdString(carta->getEditore()));
            query.bindValue(":id", id);
            query.exec();
        }
    }
}

void DbUpdate::updateMultimedia(Multimedia* multi){
    if(!multi) return;

    int id = multi->getId();

    for(int i = 0; i < biblioteca.size(); ++i){
        if(biblioteca[i]->getId() == id){
            Biblioteca* obj = biblioteca[i];
            qDebug() << "ok multimedia dbupdate";

            auto mult = dynamic_cast<Multimedia*>(obj);
            mult->setDurata(multi->getDurata());
            mult->setStudio(multi->getStudio());

            // Aggiorna MULTIMEDIA
            QSqlQuery query;
            query.prepare("UPDATE MULTIMEDIA SET "
                        "durata = :durata, "
                        "studio = :studio "
                        "WHERE ID = :id");
            query.bindValue(":durata", multi->getDurata());
            query.bindValue(":studio", QString::fromStdString(multi->getStudio()));
            query.bindValue(":id", id);
            query.exec();
        }
    }
}

void DbUpdate::updateRiviste(Riviste* rivista){
    if (!rivista) return;

    int id = rivista->getId();

    for (int i = 0; i < biblioteca.size(); ++i) {
        if (biblioteca[i]->getId() == id) {
            Biblioteca* obj = biblioteca[i];
            qDebug() << "ok rivista dbupdate";
            
            updateBiblioteca(rivista);
            updateCartaceo(rivista);

            auto riv = dynamic_cast<Riviste*>(obj);
            riv->setDiffusione(rivista->getDiffusione());
            
            // Aggiorna RIVISTE
            QSqlQuery query;
            query.prepare("UPDATE RIVISTE SET "
                        "diffusion = :diffusion "
                        "WHERE ID = :id");
            query.bindValue(":diffusion", QString::fromStdString(rivista->diffusionToString()));
            query.bindValue(":id", id);
            query.exec();

        }
    }
}


void DbUpdate::updateLibri(Libri* libro){
    if (!libro) return;

    int id = libro->getId();

    for (int i = 0; i < biblioteca.size(); ++i) {
        if (biblioteca[i]->getId() == id) {
            Biblioteca* obj = biblioteca[i];
            qDebug() << "ok libro dbupdate";

            updateBiblioteca(libro);
            updateCartaceo(libro);
            
            auto lib = dynamic_cast<Libri*>(obj);
            lib->setLanguage(libro->getLanguage());
            lib->setNvolumi(libro->getNvolumi());

            // Aggiorna LIBRI
            QSqlQuery query;
            query.prepare("UPDATE LIBRI SET "
                        "linguaoriginale = :linguaoriginale, "
                        "nvolumi = :nvolumi "
                        "WHERE ID = :id");
            query.bindValue(":linguaoriginale", QString::fromStdString(libro->getLanguage()));
            query.bindValue(":nvolumi", libro->getNvolumi());
            query.bindValue(":id", id);
            query.exec();
        }
    }
}

void DbUpdate::updateManga(Manga* manga){
    if(!manga) return;

    int id = manga->getId();
    
    for (int i = 0; i < biblioteca.size(); ++i) {
        if(biblioteca[i]->getId() == id){
            qDebug() << "ok manga dbupdate";
            Biblioteca* obj = biblioteca[i];

            updateBiblioteca(manga);
            updateCartaceo(manga);
            updateLibri(manga);

            auto mang = dynamic_cast<Manga*>(obj);
            mang->setConcluso(manga->getConcluso());

            QSqlQuery query;
            query.prepare("UPDATE MANGA SET "
                        "concluso = :concluso "
                        "WHERE ID = :id");
            query.bindValue(":concluso", manga->getConcluso());
            query.bindValue(":id", id);
            query.exec();
        }
    }
}

void DbUpdate::updateFilm(Film* film){
    if (!film) return;

    int id =  film->getId();

    for (int i = 0; i < biblioteca.size(); ++i) {
        if(biblioteca[i]->getId() == id){
            qDebug() << "ok film dbupdate";
            Biblioteca* obj = biblioteca[i];

            updateBiblioteca(film);
            updateMultimedia(film);

            auto fil = dynamic_cast<Film*>(obj);
            fil->setRegista(film->getRegista());
            fil->setAttore(film->getAttore());
            fil->segnaVisto();

            // Aggiorna FILM
            QSqlQuery query;
            query.prepare("UPDATE FILM SET "
                        "regista = :regista, "
                        "attoreprotagonista = :attoreprotagonista "
                        "WHERE ID = :id");
            query.bindValue(":regista", QString::fromStdString(film->getRegista()));
            query.bindValue(":attoreprotagonista", QString::fromStdString(film->getAttore()));
            query.bindValue(":id", id);
            query.exec();
        }
    }
}

void DbUpdate::updateCd(Cd* cd){
    if(!cd) return;
    
    int id = cd->getId();

    for(int i = 0; i < biblioteca.size(); ++i){
        if(biblioteca[i]->getId() == id){
            qDebug() << "ok cd dbupdate";
            Biblioteca* obj = biblioteca[i];

            updateBiblioteca(cd);
            updateMultimedia(cd);

            auto c = dynamic_cast<Cd*>(obj);
            c->setArtista(cd->getArtista());
            c->setTracce(cd->getTracce());

            // Aggiorna CD
            QSqlQuery query;
            query.prepare("UPDATE CD SET "
                        "artista = :artista, "
                        "ntracce = :ntracce "
                        "WHERE ID = :id");
            query.bindValue(":artista", QString::fromStdString(cd->getArtista()));
            query.bindValue(":ntracce", cd->getTracce());
            query.bindValue(":id", id);
            query.exec();
        }
    }
}