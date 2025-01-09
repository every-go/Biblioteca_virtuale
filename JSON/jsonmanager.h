#ifndef JSONMANAGER_H
#define JSONMANAGER_H

#include <QWidget>
#include <QString>
#include <QJsonObject>
#include <QDialog>
#include <QGridLayout>
#include "../interfaccia_grafica/adminarea.h"
#include "../interfaccia_grafica/userarea.h"
#include <climits>
#include <cfloat>
//ritorna una coppia da una funzione (libreria std)
#include <utility>

class Biblioteca;
class Multimedia;
class Cartaceo;
class Libri;
class Manga;
class Cd;
class Riviste;
class Film;

class JsonManager : public QWidget {
    Q_OBJECT
private:
    QList<JsonObserver*> observers;
    QList<Biblioteca*> biblioteca;
    QString filePath;
public slots:
    void handleObjectCreation(QString selectedClass);
    void deleteObject(Biblioteca* biblio);
    void modifyObject(Biblioteca* biblio);
    void savePrenota(Biblioteca* biblio);
    void saveLetto(Cartaceo* carta);
    void saveAscoltato(Cd* cd);
    void saveVisto(Film* film);
public:
    JsonManager(const QString& fileName, QWidget* parent = nullptr);
    void addObserver(JsonObserver* observer);
    void notifyObservers(const QList<Biblioteca*>& newBiblioteca);
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
    QWidget* getAdminArea();
    void mostraMenu(Biblioteca* biblio, QDialog* dialog, QGridLayout* dialogLayout, QWidget* parent);
    void mostraMenu(Cartaceo* carta, QDialog* dialog, QGridLayout* dialogLayout, QWidget* parent);
    void mostraMenu(Multimedia* multi, QDialog* dialog, QGridLayout* dialogLayout, QWidget* parent);
    void mostraMenu(Cd* cd, QDialog* dialog, QGridLayout* dialogLayout, QWidget* parent);
    void mostraMenu(Riviste* rivista, QDialog* dialog, QGridLayout* dialogLayout, QWidget* parent);
    void mostraMenu(Film* film, QDialog* dialog, QGridLayout* dialogLayout, QWidget* parent);
    void mostraMenu(Manga* manga, QDialog* dialog, QGridLayout* dialogLayout, QWidget* parent);
    void mostraMenu(Libri* libro, QDialog* dialog, QGridLayout* dialogLayout, QWidget* parent);
    void updateObject(Biblioteca* biblio);
    Biblioteca* createRiviste();
    Biblioteca* createLibri();
    Biblioteca* createManga();
    Biblioteca* createCd();
    Biblioteca* createFilm();
    std::pair<JsonManager::BibliotecaBase, bool> createBiblioteca();
    std::pair<MultimediaBase, bool> createMultimedia();
    std::pair<JsonManager::CartaceoBase, bool> createCartaceo();
    std::pair<QString, bool> getValidTextInput(QWidget* parent, const QString& title, const QString& label, const QString& defaultText = "");
    std::pair<int, bool> getValidIntInput(QWidget* parent, const QString& title, const QString& label, int defaultValue = 0, int min = INT_MIN, int max = INT_MAX, int step = 1);
    std::pair<double, bool> getValidDoubleInput(QWidget* parent, const QString& title, const QString& label, double defaultValue = 0.0, double min = -DBL_MAX, double max = DBL_MAX, double decimals = 2);
    std::pair<bool, bool> getBooleanInputFromList(QWidget* parent, const QString& title, const QString& label);
    std::pair<QString, bool> getImageInput(QWidget* parent, const QString& title, const QString& label);
    void savenewObject(Biblioteca* biblio);
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
