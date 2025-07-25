#ifndef JSONOBSERVER_H
#define JSONOBSERVER_H
#include <QList>

class Biblioteca;

class JsonObserver
{
public:
    virtual void onBibliotecaUpdated(const QList<Biblioteca*>& newBiblioteca) = 0;
    virtual ~JsonObserver() = default;
};

#endif // JSONOBSERVER_H