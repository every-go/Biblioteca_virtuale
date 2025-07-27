#include "dbcreate.h"

// Ho incluso solo gli header necessari per evitare dipendenze ridondanti,
// poiché le classi come Cd, Film, Manga e Riviste includono già le dipendenze comuni
// come Multimedia, Biblioteca e Libri.

#include "../modello_logico/manga.h"
#include "../modello_logico/riviste.h"
#include "../modello_logico/cd.h"
#include "../modello_logico/film.h"

void DbCreate::addObserver(DbObserver* observer){
    observers.append(observer);
}

void DbCreate::notifyObservers(QList<Biblioteca *> &newBiblioteca){
    //dato che so di avere solo due observer admin e user posso
    //chiamare la funzione senza fare i dynamic_cast
    for(DbObserver* observer : observers){
        observer->onBibliotecaUpdated(newBiblioteca);
    }
}

void DbCreate::createnewObject(Biblioteca* biblio){

}

DbCreate::BibliotecaInfo DbCreate::createBiblioteca(){}

DbCreate::MultimediaInfo DbCreate::createMultimedia(){}

DbCreate::CartaceoInfo DbCreate::createCartaceo(){}

Biblioteca* DbCreate::createRiviste(){}

Biblioteca* DbCreate::createLibri(){}

Biblioteca* DbCreate::createManga(){}

Biblioteca* DbCreate::createFilm(){}

Biblioteca* DbCreate::createCd(){}