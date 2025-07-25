#ifndef ADMINAREA_H
#define ADMINAREA_H

#include "mainwindow.h"
#include "../JSON/jsonobserver.h"
#include <QObject>
#include <QWidget>
#include <QMainWindow>
#include <QScrollArea>
#include <QGridLayout>
#include <QMap>
#include <QByteArray>
#include <QStackedWidget>
#include <QTypeInfo>
#include <QPushButton>

class Biblioteca;

class AdminArea : public QMainWindow, public JsonObserver
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
    bool film, libri, manga, riviste, cd;
    void gestisciPulsanti(Biblioteca* biblio, QPushButton* modifica,
                         QPushButton* elimina);
signals:
    void createNewObject();
    void removeObject(Biblioteca* biblio);
    void modifyObject(Biblioteca* biblio);
private slots:
    void tornaIndietro();
    void showUser();
    void aggiungi();
    void chiudi();
public:
    AdminArea(QList<Biblioteca*>objects, QStackedWidget* stackWidget, QWidget* parent = nullptr);
    ~AdminArea();
    void onBibliotecaUpdated(const QList<Biblioteca*>& newBiblioteca) override;
    void showMain();
    void showAll();
    void showTipi();
    void clearLayout(QLayout* layout);
    void cercaDigitato(const QString& testo);
    void handlePostAction();
public slots:
    void slotHandle();
};

#endif // ADMINAREA_H