#ifndef LIBRI_H
#define LIBRI_H
#include "cartaceo.h"
using std::string;

class Libri : public Cartaceo
{
private:
    string lingua_originale;
    int nvolumi;        //indica se si tratta di una saga: Harry Potter per i libri
                        //oppure come tutti i manga
public:
    Libri(string title, string genre, int year, double price, bool disponibile,
        int copies, int prest, string image, string author, string editor,
        bool read, string language, int number);
    virtual ~Libri() = default;
    string getLanguage() const;
    int getNvolumi() const;
    void setLanguage(const string& newlanguage);
    bool setNvolumi(const int& newnVolumi);
    void accept(VisitorWidget& visitor) override;
};

#endif // LIBRI_H
