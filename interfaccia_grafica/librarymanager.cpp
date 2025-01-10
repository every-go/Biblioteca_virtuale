#include "librarymanager.h"
#include <QComboBox>
#include <QToolBar>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QFileDialog>
#include <QDebug>
#include <QSpacerItem>

// Ho incluso solo gli header necessari per evitare dipendenze ridondanti,
// poiché le classi come Cd, Film, Manga e Riviste includono già le dipendenze comuni
// come Multimedia, Biblioteca e Libri.

#include "../modello_logico/manga.h"
#include "../modello_logico/riviste.h"
#include "../modello_logico/cd.h"
#include "../modello_logico/film.h"

LibraryManager::LibraryManager(QStackedWidget *stackWidget, QWidget *parent)
    : QMainWindow(parent), stack(stackWidget)
{
    setWindowTitle("Manager oggetti");
    resize(800, 600);
    scrollArea = new QScrollArea(this);
    scrollContent = new QWidget(this);
    layout = new QVBoxLayout(scrollContent);
    layout->setContentsMargins(1, 1, 1, 1);
    layout->setSpacing(5);
    layout->setAlignment(Qt::AlignTop);
    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea{"
                              "background-image: url(:/images/biblioteca.png);"
                              "background-position: center;"
                              "}");
    scrollContent->setStyleSheet("QWidget {"
                                 "background: transparent;"
                                 "}");
    QToolBar *toolBar = new QToolBar("Main Toolbar", this);
    addToolBar(Qt::TopToolBarArea, toolBar);
    QPushButton* indietro = new QPushButton("Torna indietro", scrollContent);
    toolBar->addWidget(indietro);
    QPushButton* close = new QPushButton("Chiudi", scrollContent);
    toolBar->addWidget(close);
    QPushButton* salva = new QPushButton("Salva", scrollContent);
    toolBar->addWidget(salva);
    scrollContent->setLayout(layout);
    setCentralWidget(scrollArea);
    connect(indietro, &QPushButton::clicked, this, &LibraryManager::tornaIndietro);
    connect(close, &QPushButton::clicked, this, &LibraryManager::close);
}

void LibraryManager::close(){
    stack->close();
}

void LibraryManager::tornaIndietro(){
    stack->setCurrentIndex(2);
}

void LibraryManager::modifyObject(Biblioteca* biblio){
    if(typeid(*biblio)==typeid(Manga))
        modifica(static_cast<Manga*>(biblio));
    if(typeid(*biblio)==typeid(Libri))
        modifica(static_cast<Libri*>(biblio));
    if(typeid(*biblio)==typeid(Riviste))
        modifica(static_cast<Riviste*>(biblio));
    if(typeid(*biblio)==typeid(Film))
        modifica(static_cast<Film*>(biblio));
    if(typeid(*biblio)==typeid(Cd))
        modifica(static_cast<Cd*>(biblio));
}

void LibraryManager::modifica(Biblioteca* biblio){

}

void LibraryManager::modifica(Cartaceo* carta){
    modifica(static_cast<Biblioteca*>(carta));

}

void LibraryManager::modifica(Multimedia* multi){
    modifica(static_cast<Biblioteca*>(multi));

}

void LibraryManager::modifica(Riviste* rivista){
    modifica(static_cast<Cartaceo*>(rivista));

}

void LibraryManager::modifica(Libri* libro){
    modifica(static_cast<Cartaceo*>(libro));

}

void LibraryManager::modifica(Manga* manga){
    modifica(static_cast<Libri*>(manga));

}

void LibraryManager::modifica(Film* film){
    modifica(static_cast<Multimedia*>(film));

}

void LibraryManager::modifica(Cd* cd){
    modifica(static_cast<Multimedia*>(cd));

}

void LibraryManager::createObject(){
    Biblioteca* biblio = nullptr;
    QPushButton* crea = new QPushButton("Crea", scrollContent);
    crea->setStyleSheet("QPushButton{"
                        "background-color: white;"
                        "}");
    QStringList classNames{"Libri","Manga","Riviste","Cd","Film"};
    QComboBox* selezione = new QComboBox(scrollContent);
    selezione->setEditable(true);
    selezione->lineEdit()->setAlignment(Qt::AlignCenter);
    selezione->addItems(classNames);
    selezione->setStyleSheet("QComboBox{"
                             "background-color:white;"
                             "}"
                             "QComboBox QAbstractItemView {"
                             "background-color: white;"
                             "color: black;"
                             "selection-background-color: lightblue;"
                             "selection-color: black;"
                             "}"
                             "QComboBox::item:hover {"
                             "background-color: lightblue;"
                             "color: black;"
                             "}"
                            );
    QVBoxLayout* dynamicLayout = new QVBoxLayout();
    dynamicLayout->addWidget(selezione);
    QVBoxLayout* fixedLayout = new QVBoxLayout();
    fixedLayout->addWidget(crea);
    layout->addLayout(dynamicLayout);
    layout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout->addLayout(fixedLayout);

    connect(crea, &QPushButton::clicked, this, [this, &biblio, selezione](){
        if(selezione->currentText() == "Riviste"){
            biblio = createRiviste();
        }
        if(selezione->currentText() == "Libri"){
            biblio = createLibri();
        }
        if(selezione->currentText() == "Manga"){
            biblio = createManga();
        }
        if(selezione->currentText() == "Cd"){
            biblio = createCd();
        }
        if(selezione->currentText() == "Film"){
            biblio = createFilm();
        }
        if(biblio){
            emit newObject(biblio);

        }
    });
}

void LibraryManager::crea(Biblioteca* biblio){

}

void LibraryManager::crea(Cartaceo* carta){
    crea(static_cast<Biblioteca*>(carta));

}

void LibraryManager::crea(Multimedia* multi){
    crea(static_cast<Biblioteca*>(multi));

}

void LibraryManager::crea(Riviste* rivista){
    crea(static_cast<Cartaceo*>(rivista));

}

void LibraryManager::crea(Libri* libro){
    crea(static_cast<Cartaceo*>(libro));

}

void LibraryManager::crea(Manga* manga){
    crea(static_cast<Libri*>(manga));

}

void LibraryManager::crea(Film* film){
    crea(static_cast<Multimedia*>(film));

}

void LibraryManager::crea(Cd* cd){
    crea(static_cast<Multimedia*>(cd));

}

Riviste* LibraryManager::createRiviste(){}

Libri* LibraryManager::createLibri(){}

Manga* LibraryManager::createManga(){}

Cd* LibraryManager::createCd(){}

Film* LibraryManager::createFilm(){}
