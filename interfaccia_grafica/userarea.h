#ifndef USERAREA_H
#define USERAREA_H

#include "mainwindow.h"
#include "../JSON/jsonobserver.h"
#include <QWidget>
#include <QMainWindow>
#include <QList>
#include <QScrollArea>
#include <QGridLayout>
#include <QObject>
#include <QEvent>
#include <QMap>
#include <QByteArray>
#include <QTypeInfo>
#include <QStackedWidget>
#include <QMenu>
#include <QPushButton>

// Ho incluso solo gli header necessari per evitare dipendenze ridondanti,
// poiché le classi come Cd, Film, Manga e Riviste includono già le dipendenze comuni
// come Multimedia, Biblioteca e Libri.

#include "../modello_logico/cd.h"
#include "../modello_logico/film.h"
#include "../modello_logico/manga.h"
#include "../modello_logico/riviste.h"

class UserArea : public QMainWindow, public JsonObserver
{
    Q_OBJECT
private:
    QList<Biblioteca*> oggetti;
    QStackedWidget* stack;
    QScrollArea* scrollArea;
    QWidget* scrollContent;
    QGridLayout* layout;
    QString testo;
    QMap<QByteArray, bool*> typeFlags;
    //booleano per gestire la funzione cerca
    bool film, libri, manga, riviste, cd, simili;
    void gestisciConnect(Biblioteca* biblio, QPushButton* prenota,
                         QPushButton* restituisci, QPushButton* suggerisci, QPushButton* azione);
    void gestisciAzione(Biblioteca* biblio, QPushButton* azione);
signals:
    void prenota(Biblioteca* biblio);
    void letto(Cartaceo* carta);
    void ascoltato(Cd* cd);
    void visto(Film* film);
private slots:
    void tornaIndietro();
    void chiudi();
    void admin();
public:
    UserArea(QList<Biblioteca*> objects, QStackedWidget* stackWidget, QWidget *parent = nullptr);
    ~UserArea();
    void onBibliotecaUpdated(const QList<Biblioteca*>& newBiblioteca) override;
    void setOggetti(const QList<Biblioteca*>& newBiblioteca);
    void showMain();
    void showAdmin();
    void showAll();
    void showTipi();
    void clearLayout(QLayout* layout);
    void handlePostAction(Biblioteca* biblio);
    void cercaDigitato(const QString& testo);
    void suggerisciSimili(Biblioteca* biblio);
};

#endif // USERAREA_H
