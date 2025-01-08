#ifndef CD_DVD_H
#define CD_DVD_H
#include "multimedia.h"
using std::string;
class Cd : public Multimedia
{
private:
    string artista;
    int ntracce;
    bool ascoltato;
public:
    Cd(string title, string genre, int year, double price, bool disponibile,
       int copies, int prest, string image, int time, string studios,
       string artist, int ntracks, bool listened);
    virtual ~Cd() = default;
    string getArtista() const;
    int getTracce() const;
    bool getAscoltato() const;
    //cambia stato ascolto cd
    void segnaAscoltato();
    void setArtista(const string& newartista);
    void setTracce(const int& newntracce);
    void accept(VisitorWidget& visitor) override;
};

#endif // CD_DVD_H
