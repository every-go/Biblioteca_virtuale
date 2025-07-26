#include "biblioteca.h"
using std::string;

Biblioteca::Biblioteca(int id_db, string title, string genre, int year,
                       double price, bool disp, int copies, int prest, string image)
    : id(id_db), titolo(title), genere(genre), anno(year), costo(price), disponibile(disp),
    copie_disponibili(copies>=0 ? copies : 0), immagine(image){
    if(prest>copies)
        nprestiti=prest;
    else
        nprestiti=copies;
}

void Biblioteca::aggiornaDisponibilita(){
    disponibile = (copie_disponibili>0);
}

int Biblioteca::getId() const{
    return id;
}

string Biblioteca::getTitolo() const{
    return titolo;
}

string Biblioteca::getGenere() const{
    return genere;
}

int Biblioteca::getAnno() const{
    return anno;
}

double Biblioteca::getCosto() const{
    return costo;
}

bool Biblioteca::getDisponibilita() const{
    return disponibile;
}

int Biblioteca::getCopie() const{
    return copie_disponibili;
}

int Biblioteca::getNprestiti() const{
    return nprestiti;
}

string Biblioteca::getImmagine() const{
    return immagine;
}

void Biblioteca::setTitolo(const string &newtitle){
    titolo = newtitle;
}

void Biblioteca::setGenere(const string &newgenre){
    genere = newgenre;
}

void Biblioteca::setAnno(const int &newanno){
    anno = newanno;
}

void Biblioteca::setCosto(const double& newcosto){
    costo = newcosto;
}

void Biblioteca::setImmagine(const string& image){
    immagine=image;
}

bool Biblioteca::setNprestiti(const int& newnprest){
    if(newnprest>0 && newnprest>=copie_disponibili){
        nprestiti = newnprest;
        return true;
    }
    return false;
}

bool Biblioteca::prenota(){
    if(copie_disponibili==0){
        return false;
    }
    copie_disponibili-=1;
    aggiornaDisponibilita();
    return true;
}

bool Biblioteca::restituisci(){
    if(copie_disponibili<0 || copie_disponibili>=nprestiti)
        return false;
    copie_disponibili+=1;
    aggiornaDisponibilita();
    return true;
}
