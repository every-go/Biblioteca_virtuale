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
signals:
    void objectCreationRequested(QString selectedClass);
    void removeObject(Biblioteca* biblio);
    void modifyObject(Biblioteca* biblio);
private slots:
    void tornaIndietro();
    void showUser();
    void aggiungi();
    void chiudi();
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
public:
    AdminArea(QList<Biblioteca*>objects, QStackedWidget* stackWidget, QWidget* parent = nullptr);
    ~AdminArea();
    void onBibliotecaUpdated(const QList<Biblioteca*>& newBiblioteca) override;
    void setOggetti(const QList<Biblioteca*>& newBiblioteca);
    void showMain();
    void showAll();
    void showTipi();
    void clearLayout(QLayout* layout);
    void mostraMenu(Biblioteca* biblio);
    void cercaDigitato(const QString& testo);
};

#endif // ADMINAREA_H
