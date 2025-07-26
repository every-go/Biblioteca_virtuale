#ifndef MULTIMEDIA_H
#define MULTIMEDIA_H
#include "biblioteca.h"
using std::string;

class Multimedia : public Biblioteca
{
private:
    //per durata si intende a prescindere in minuti,
    //ho deciso di non impostare la classe orario, seppur facile,
    //perché solitamente i film vengono descritti con durata di 90 min
    //(ad esempio), idem per i Cd (per semplicità è int e non double)
    int durata;
    string studio;
public:
    Multimedia(int id_db, string title, string genre, int year, double price, bool disponibile,
               int copies, int prest, string image, int time, string studios);
    virtual ~Multimedia() =default;
    int getDurata() const;
    string getStudio() const;
    void setDurata(const int& newdurata);
    void setStudio(const string& newstudio);
    virtual void accept(Visitor* visitor) override = 0;
};

#endif // MULTIMEDIA_H
