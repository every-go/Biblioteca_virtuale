#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H
#include <string>
using std::string;

#include "../interfaccia_grafica/visitorwidget.h"

class Biblioteca{
private:
    string titolo;
    string genere;
    int anno;
    double costo;
    //disponibile è di default true perché come minimo c'è una copia per oggetto
    bool disponibile;
    int copie_disponibili;
    int nprestiti;
    string immagine;
    void aggiornaDisponibilita();
public:
    Biblioteca(string title, string genre, int year, double price,
               bool disponibile, int copies, int prest, string image);
    virtual ~Biblioteca() =default;
    string getTitolo() const;
    string getGenere() const;
    int getAnno() const;
    double getCosto() const;
    bool getDisponibilita() const;
    int getCopie() const;
    int getNprestiti() const;
    string getImmagine() const;
    void setTitolo(const string& newtitle);
    void setGenere(const string& newgenre);
    void setAnno(const int& newanno);
    void setCosto(const double& newcosto);
    void setImmagine(const string& image);
    //cambia il numero di prestiti possibili
    bool setNprestiti(const int& newnprest);
    bool prenota();
    bool restituisci();
    virtual void accept(VisitorWidget& visitor) = 0;
};

//serve per dichiarare al compilatore di Qt che è un nuovo metatipo
Q_DECLARE_METATYPE(Biblioteca*)

#endif // BIBLIOTECA_H
