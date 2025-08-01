#ifndef RIVISTE_H
#define RIVISTE_H
#include "cartaceo.h"
using std::string;

class Riviste : public Cartaceo
{
public:
    enum Diffusione{Provinciale,Regionale,Nazionale,Internazionale};
private:
    Diffusione diffusion;
public:
    Riviste(int id_db, string title, string genre, int year, double price,
            bool disponibile, int copies, int prest, string image,
            string author, string editor, bool read, Diffusione diff);
    virtual ~Riviste() = default;
    string diffusionToString() const;
    void setDiffusione(const Diffusione& newdiffusion);
    Diffusione getDiffusione() const;
    void accept(Visitor* visitor) override;
};

#endif // RIVISTE_H
