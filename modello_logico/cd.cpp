#include "cd.h"
using std::string;

Cd::Cd(string title, string genre, int year, double price, bool disponibile, int copies,
       int prest, string image, int time, string studios, string artist, int ntracks, bool listened)
        :Multimedia(title, genre, year, price, disponibile, copies, prest, image, time, studios),
        artista(artist), ntracce(ntracks>1 ? ntracks : 1), ascoltato(listened) {}

string Cd::getArtista() const{
    return artista;
}

int Cd::getTracce() const{
    return ntracce;
}

bool Cd::getAscoltato() const{
    return ascoltato;
}

void Cd::segnaAscoltato(){
    ascoltato = !ascoltato;
}

void Cd::setArtista(const string &newartista){
    artista = newartista;
}

void Cd::setTracce(const int &newntracce){
    ntracce=newntracce;
}

void Cd::accept(VisitorWidget& visitor){
    visitor.visit(this);
}
