#include "riviste.h"
using std::string;

Riviste::Riviste(int id_db, string title, string genre, int year, double price,
                bool disponibile, int copies, int prest, string image,
                string author, string editor, bool read, Diffusione diff) :
                Cartaceo(id_db, title, genre, year, price, disponibile, copies, prest, image, author, editor, read),
                diffusion(diff) {}

void Riviste::setDiffusione(const Riviste::Diffusione& newdiffusion){
    diffusion = newdiffusion;
}

Riviste::Diffusione Riviste::getDiffusione() const {
    return diffusion;
}

string Riviste::diffusionToString() const {
    switch (diffusion) {
    case Provinciale: return "Provinciale";
    case Regionale: return "Regionale";
    case Nazionale: return "Nazionale";
    case Internazionale: return "Internazionale";
    default: return "Sconosciuto";
    }
}

void Riviste::accept(Visitor* visitor){
    visitor->visit(this);
}
