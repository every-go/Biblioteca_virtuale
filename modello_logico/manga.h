#ifndef MANGA_H
#define MANGA_H
#include "libri.h"
using std::string;

class Manga : public Libri
{
private:
    bool concluso;
public:
    Manga(int id_db, string title, string genre, int year, double price, bool disponibile,
          int copies, int prest, string image, string author, string editor,
          bool read, string language, int number, bool finish);
    virtual ~Manga() = default;
    bool getConcluso() const;
    void setConcluso(const bool& newconcluso);
    void statoConcluso();
    void accept(Visitor* visitor) override;
};

#endif // MANGA_H
