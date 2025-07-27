#ifndef FILM_H
#define FILM_H
#include "multimedia.h"
using std::string;

class Film : public Multimedia
{
private:
    string regista;
    string attore_protagonista;
    bool visto;
public:
    Film(int id_db, string title, string genre, int year, double price, bool disponibile,
         int copies, int prest, string image, int time, string studios,
         string director, string actor, bool view);
    virtual ~Film() = default;
    string getAttore() const;
    string getRegista() const;
    bool getVisto() const;
    void setRegista(const string& newregista);
    void setAttore(const string& newattore);
    //cambia stato visione film
    void segnaVisto();
    void accept(Visitor* visitor) override;
};

#endif // FILM_H
