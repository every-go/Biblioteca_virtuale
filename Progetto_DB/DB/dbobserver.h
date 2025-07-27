#ifndef DBOBSERVER_H
#define DBOBSERVER_H
#include <QList>

class Biblioteca;

class DbObserver {
public:
    virtual void onBibliotecaUpdated(const QList<Biblioteca*>& newBiblioteca) = 0;
    virtual ~DbObserver() = default;
};

#endif // JSONOBSERVER_H