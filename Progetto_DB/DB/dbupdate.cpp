#include "dbupdate.h"

// Ho incluso solo gli header necessari per evitare dipendenze ridondanti,
// poiché le classi come Cd, Film, Manga e Riviste includono già le dipendenze comuni
// come Multimedia, Biblioteca e Libri.

#include "../modello_logico/manga.h"
#include "../modello_logico/riviste.h"
#include "../modello_logico/cd.h"
#include "../modello_logico/film.h"

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
   
}

DbUpdate::BibliotecaInfo DbUpdate::updateBiblioteca(){}

DbUpdate::MultimediaInfo DbUpdate::updateMultimedia(){}

DbUpdate::CartaceoInfo DbUpdate::updateCartaceo(){}

Biblioteca* DbUpdate::updateRiviste(){}

Biblioteca* DbUpdate::updateLibri(){}

Biblioteca* DbUpdate::updateManga(){}

Biblioteca* DbUpdate::updateFilm(){}

Biblioteca* DbUpdate::updateCd(){}