#include "film.h"
using std::string;

Film::Film(int id_db, string title, string genre, int year, double price,
           bool disponibile, int copies, int prest,
           string image, int time, string studios,
           string director, string actor, bool view) :
           Multimedia(id_db, title, genre, year, price, disponibile, copies,
                    prest, image, time, studios),
           regista(director), attoreprotagonista(actor), visto(view) {}

string Film::getRegista() const{
    return regista;
}

string Film::getAttore() const{
    return attoreprotagonista;
}

bool Film::getVisto() const{
    return visto;
}

void Film::setRegista(const string &newregista){
    regista = newregista;
}

void Film::setAttore(const string &newattore){
    attoreprotagonista = newattore;
}

void Film::segnaVisto(){
    visto = !visto;
}

void Film::accept(Visitor* visitor){
    visitor->visit(this);
}
