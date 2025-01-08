#ifndef VISITOR_H
#define VISITOR_H
#include <QWidget>

class Manga;
class Libri;
class Riviste;
class Film;
class Cd;

class Visitor : public QWidget
{
public:
    virtual void visit(Manga* manga) =0;
    virtual void visit(Libri* libro) =0;
    virtual void visit(Riviste* rivista) =0;
    virtual void visit(Film* film) =0;
    virtual void visit(Cd* cd) =0;
};

#endif
