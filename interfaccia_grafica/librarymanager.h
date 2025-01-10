#ifndef LIBRARYMANAGER_H
#define LIBRARYMANAGER_H

#include <QMainWindow>
#include <QObject>
#include <QStackedWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QString>
#include <QScrollArea>
#include <QMap>
#include <QLayout>

class Biblioteca;
class Cartaceo;
class Multimedia;
class Libri;
class Manga;
//serve per accedere all'enum
#include "../modello_logico/riviste.h"
class Cd;
class Film;

class LibraryManager : public QMainWindow
{
    Q_OBJECT
private:
    QStackedWidget* stack;
    QScrollArea* scrollArea;
    QWidget* scrollContent;
    QVBoxLayout* layout;
    QVBoxLayout* modifyLayout;
    QMap<QString, QWidget*> menuWidgets;
    void clearLayout(QLayout* layout);
    QString image;
    struct BibliotecaInfo {
        QString titolo;
        QString genere;
        int anno;
        double costo;
        bool disponibile;
        int copie;
        int nprestiti;
        QString immagine;
    };
    struct CartaceoInfo {
        QString autore;
        QString editore;
        bool letto;
    };
    struct MultimediaInfo {
        int durata;
        QString studio;
    };
signals:
    void update(Biblioteca* biblio);
    void newObject(Biblioteca* biblio);
    void handle();
private slots:
    void tornaIndietro();
    void close();
public slots:
    void modifyObject(Biblioteca* biblio);
    void createObject();
public:
    LibraryManager(QStackedWidget* stackWidget, QWidget* parent = nullptr);
    void menuBiblio(Biblioteca* biblio = nullptr);
    void menuCarta(Cartaceo* carta = nullptr);
    void menuMulti(Multimedia* multi = nullptr);
    void menuCd(Cd* cd = nullptr);
    void menuRiviste(Riviste* rivista = nullptr);
    void menuFilm(Film* film = nullptr);
    void menuManga(Manga* manga = nullptr);
    void menuLibri(Libri* libro = nullptr);
    BibliotecaInfo createBiblio();
    MultimediaInfo createMultimedia();
    CartaceoInfo createCartaceo();
    Riviste* createRiviste();
    Libri* createLibri();
    Manga* createManga();
    Cd* createCd();
    Film* createFilm();
    void save(Biblioteca* biblio);
    void save(Cartaceo* carta);
    void save(Multimedia* multi);
    void save(Riviste* rivista);
    void save(Libri* libro);
    void save(Manga* manga);
    void save(Film* film);
    void save(Cd* cd);
};

#endif // LIBRARYMANAGER_H
