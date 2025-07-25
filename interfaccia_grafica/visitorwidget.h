#ifndef VISITORWIDGET_H
#define VISITORWIDGET_H

#include "dependencies.h"

#include "visitor.h"

class Biblioteca;
class Cartaceo;
class Multimedia;

class VisitorWidget : public Visitor
{
private:
    QWidget* widget;
    QLabel* artwork;
    QGridLayout* layout;
public:
    VisitorWidget();
    QWidget* getWidget() const;
    void visit(Manga* manga) override;
    void visit(Libri* libro) override;
    void visit(Riviste* rivista) override;
    void visit(Film* film) override;
    void visit(Cd* cd) override;
    void visit(Biblioteca* biblio);
    void visit(Multimedia* multi);
    void visit(Cartaceo* carta);
};

#endif
