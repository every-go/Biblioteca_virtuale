#ifndef CARTACEO_H
#define CARTACEO_H
#include "biblioteca.h"
using std::string;
class Cartaceo : public Biblioteca
{
private:
    string autore;
    string editore;
    bool letto;
public:
    Cartaceo(int id_db, string title, string genre, int year, double price, bool disponibile,
            int copies, int prest, string image, string author, string editor, bool read);
    virtual ~Cartaceo() = default;
    string getAutore() const;
    string getEditore() const;
    void setAutore(const string& newautore);
    void setEditore(const string& neweditore);
    bool getLetto() const;
    void segnaLetto();
    virtual void accept(Visitor* visitor) override = 0;
};

#endif // CARTACEO_H
