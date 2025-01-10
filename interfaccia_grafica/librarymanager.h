#ifndef LIBRARYMANAGER_H
#define LIBRARYMANAGER_H

#include <QMainWindow>
#include <QObject>
#include <QStackedWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QString>
#include <QScrollArea>

class Biblioteca;
class Cartaceo;
class Multimedia;
class Libri;
class Manga;
class Riviste;
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
public:
    struct BibliotecaInfo{

    };
    struct CartaceoInfo{

    };
    struct MultimediaInfo{

    };
    struct RivisteInfo{

    };
    struct LibriInfo{

    };
    struct MangaInfo{

    };
    struct CdInfo{

    };
    struct FilmInfo{

    };
signals:
    void update(Biblioteca* biblio);
    void newObject(Biblioteca* biblio);
public slots:
    void modifyObject(Biblioteca* biblio);
    void createObject();
    void tornaIndietro();
    void close();
public:
    LibraryManager(QStackedWidget* stackWidget, QWidget* parent = nullptr);
    void modifica(Biblioteca* biblio);
    void modifica(Cartaceo* carta);
    void modifica(Multimedia* multi);
    void modifica(Cd* cd);
    void modifica(Riviste* rivista);
    void modifica(Film* film);
    void modifica(Manga* manga);
    void modifica(Libri* libro);
    void crea(Biblioteca* biblio);
    void crea(Cartaceo* carta);
    void crea(Multimedia* multi);
    void crea(Cd* cd);
    void crea(Riviste* rivista);
    void crea(Film* film);
    void crea(Manga* manga);
    void crea(Libri* libro);
    Riviste* createRiviste();
    Libri* createLibri();
    Manga* createManga();
    Cd* createCd();
    Film* createFilm();
};

#endif // LIBRARYMANAGER_H
