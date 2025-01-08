#include "film.h"
using std::string;

Film::Film(string title, string genre, int year, double price,
           bool disponibile, int copies, int prest,
           string image, int time, string studios,
           string director, string actor, bool view) :
           Multimedia(title, genre, year, price, disponibile, copies,
                    prest, image, time, studios),
           regista(director), attore_protagonista(actor), visto(view) {}

string Film::getRegista() const{
    return regista;
}

string Film::getAttore() const{
    return attore_protagonista;
}

bool Film::getVisto() const{
    return visto;
}

void Film::setRegista(const string &newregista){
    regista = newregista;
}

void Film::setAttore(const string &newattore){
    attore_protagonista = newattore;
}

void Film::segnaVisto(){
    visto = !visto;
}

void Film::accept(VisitorWidget& visitor){
    visitor.visit(this);
}
