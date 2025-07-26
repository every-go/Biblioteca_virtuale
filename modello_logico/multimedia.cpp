#include "multimedia.h"
using std::string;

Multimedia::Multimedia(int id_db, string title, string genre, int year, double price, bool disponibile,
                     int copies, int prest, string image, int time, string studios) :
    Biblioteca(id_db, title, genre, year, price, disponibile, copies, prest, image), durata(time>0 ? time : 1), studio(studios){}

int Multimedia::getDurata() const{
    return durata;
}

string Multimedia::getStudio() const{
    return studio;
}

void Multimedia::setDurata(const int &newdurata){
    durata = newdurata;
}

void Multimedia::setStudio(const string &newstudio){
    studio = newstudio;
}
