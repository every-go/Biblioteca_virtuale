#include "cartaceo.h"
using std::string;

Cartaceo::Cartaceo(string title, string genre, int year, double price, bool disponibile,
                   int copies, int prest, string image, string author, string editor, bool read)
    : Biblioteca(title, genre, year, price, disponibile, copies, prest, image), autore(author), editore(editor), letto(read){}

string Cartaceo::getAutore() const{
    return autore;
}

string Cartaceo::getEditore() const{
    return editore;
}

void Cartaceo::setAutore(const string &newautore){
    autore = newautore;
}

void Cartaceo::setEditore(const string &neweditore){
    editore = neweditore;
}

bool Cartaceo::getLetto() const{
    return letto;
}

void Cartaceo::segnaLetto(){
    letto = !letto;
}
