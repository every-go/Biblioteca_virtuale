#include "manga.h"
using std::string;

Manga::Manga(int id_db, string title, string genre, int year, double price, bool disponibile,
             int copies, int prest, string image, string author, string editor, bool read,
             string language, int number, bool finish)
    : Libri(id_db, title, genre, year, price, disponibile, copies, prest, image, author, editor, read, language, number), concluso(finish){}

bool Manga::getConcluso() const{
    return concluso;
}

void Manga::setConcluso(const bool& newconcluso){
    concluso = newconcluso;
}

void Manga::statoConcluso(){
    concluso = !concluso;
}

void Manga::accept(Visitor* visitor){
    visitor->visit(this);
}
