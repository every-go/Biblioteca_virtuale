#include "libri.h"
using std::string;

Libri::Libri(string title, string genre, int year, double price, bool disponibile,
             int copies, int prest, string image, string author, string editor, bool read,
            string language, int number):
    Cartaceo(title, genre, year, price, disponibile, copies, prest, image, author, editor, read),
    lingua_originale(language), nvolumi(number>1 ? number : 1){}

string Libri::getLanguage() const{
    return lingua_originale;
}

int Libri::getNvolumi() const{
    return nvolumi;
}

void Libri::setLanguage(const string &newlanguage){
    lingua_originale = newlanguage;
}

bool Libri::setNvolumi(const int& newnVolumi){
    if(newnVolumi>0){
        nvolumi = newnVolumi;
        return true;
    }
    return false;
}

void Libri::accept(Visitor* visitor){
    visitor->visit(this);
}
