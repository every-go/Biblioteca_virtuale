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

void DbCreate::createBiblioteca(Biblioteca* biblio){}

void DbCreate::createMultimedia(Multimedia* multi){}

void DbCreate::createCartaceo(Cartaceo* carta){}

void DbCreate::createRiviste(Riviste* rivista){
    createBiblioteca(rivista);
    createCartaceo(rivista);
}

void DbCreate::createLibri(Libri* libro){
    createBiblioteca(libro);
    createCartaceo(libro);
}

void DbCreate::createManga(Manga* manga){
    createBiblioteca(manga);
    createCartaceo(manga);
    createLibri(manga);
}

void DbCreate::createFilm(Film* film){
    createBiblioteca(film);
    createMultimedia(film);
}

void DbCreate::createCd(Cd* cd){
    createBiblioteca(cd);
    createMultimedia(cd);
}