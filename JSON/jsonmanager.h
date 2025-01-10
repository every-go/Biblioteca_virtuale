#ifndef JSONMANAGER_H
#define JSONMANAGER_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include "../interfaccia_grafica/adminarea.h"
#include "../interfaccia_grafica/userarea.h"

class Biblioteca;
class Multimedia;
class Cartaceo;
class Libri;
class Manga;
class Cd;
class Riviste;
class Film;
class JsonObserver;

class JsonManager : public QObject {
    Q_OBJECT
private:
    QList<JsonObserver*> observers;
    QList<Biblioteca*> biblioteca;
    QString filePath;
public slots:
    void deleteObject(Biblioteca* biblio);
    void savePrenota(Biblioteca* biblio);
    void saveLetto(Cartaceo* carta);
    void saveAscoltato(Cd* cd);
    void saveVisto(Film* film);
    void savenewObject(Biblioteca* biblio);
    void updateObject(Biblioteca* biblio);
public:
    JsonManager(const QString& fileName);
    void addObserver(JsonObserver* observer);
    void notifyObservers(QList<Biblioteca*>& newBiblioteca);
    struct BibliotecaBase {
        QString titolo;
        QString genere;
        int anno;
        double costo;
        bool disponibile;
        int copie;
        int nprestiti;
        QString immagine;
    };
    struct CartaceoBase{
        QString autore;
        QString editore;
        bool letto;
    };
    struct MultimediaBase{
        int durata;
        QString studio;
    };
    QList<Biblioteca*> loadBibliotecaListFromJson(const QString& filename);
    void setOggetti(const QList<Biblioteca*>& newBiblioteca);
    Biblioteca* loadRiviste(const QJsonObject& obj);
    Biblioteca* loadLibri(const QJsonObject& obj);
    Biblioteca* loadManga(const QJsonObject& obj);
    Biblioteca* loadCd(const QJsonObject& obj);
    Biblioteca* loadFilm(const QJsonObject& obj);
    BibliotecaBase loadBiblioteca(const QJsonObject& obj);
    CartaceoBase loadCartaceo(const QJsonObject& obj);
    MultimediaBase loadMultimedia(const QJsonObject& obj);
    void saveBiblioteca(Biblioteca* biblio, QJsonObject& obj);
    void saveMultimedia(Multimedia* multi, QJsonObject& obj);
    void saveCartaceo(Cartaceo* carta, QJsonObject& obj);
    QJsonObject saveLibri(Libri* libro);
    QJsonObject saveManga(Manga* manga);
    QJsonObject saveCd(Cd* cd);
    QJsonObject saveFilm(Film* film);
    QJsonObject saveRiviste(Riviste* rivista);
};

#endif // JSONVIEWERWIDGET_H
