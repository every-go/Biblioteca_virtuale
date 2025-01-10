#include "adminarea.h"
#include "visitorwidget.h"
#include "../JSON/jsonmanager.h"
#include <QToolBar>
#include <QLineEdit>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QInputDialog>

// Ho incluso solo gli header necessari per evitare dipendenze ridondanti,
// poiché le classi come Cd, Film, Manga e Riviste includono già le dipendenze comuni
// come Multimedia, Biblioteca e Libri.

#include "../modello_logico/cd.h"
#include "../modello_logico/film.h"
#include "../modello_logico/manga.h"
#include "../modello_logico/riviste.h"

AdminArea::AdminArea(QList<Biblioteca*> objects, QStackedWidget* stackWidget, QWidget *parent) :
    QMainWindow(parent), oggetti(objects),
    stack(stackWidget), film(false), libri(false),
    manga(false), riviste(false), cd(false)
{
    setWindowTitle("Parte admin");
    resize(800,600);
    //crea la QScrollArea
    scrollArea = new QScrollArea(this);
    scrollContent = new QWidget(this);
    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea{"
                              "background-image: url(:/images/biblioteca.png);"
                              "background-position: center;"
                              "}");
    scrollContent->setStyleSheet("QWidget {"
                                 "background: transparent;"
                                 "}");

    typeFlags = {
        {typeid(Film).name(), &film},
        {typeid(Cd).name(), &cd},
        {typeid(Libri).name(), &libri},
        {typeid(Riviste).name(), &riviste},
        {typeid(Manga).name(), &manga}
    };

    // Crea la barra degli strumenti
    QToolBar *toolBar = new QToolBar("Main Toolbar", this);
    // Aggiungi la toolbar nella finestra principale
    addToolBar(Qt::TopToolBarArea, toolBar);
    QPushButton* indietro = new QPushButton("Torna indietro", scrollContent);
    toolBar->addWidget(indietro);
    QPushButton* user = new QPushButton("Area utente", scrollContent);
    toolBar->addWidget(user);
    QPushButton* close = new QPushButton("Chiudi", scrollContent);
    toolBar->addWidget(close);
    QPushButton* aggiunta = new QPushButton("Aggiungi nuovo elemento", scrollContent);
    toolBar->addWidget(aggiunta);
    QLineEdit* cercaOggetto = new QLineEdit(scrollContent);
    cercaOggetto->setPlaceholderText("Cerca per titolo, genere, attore, regista, artista o autore");
    toolBar->addWidget(cercaOggetto);

    // Crea la barra dei menu
    QMenuBar* menuBar = this->menuBar();
    QMenu* fileMenu = menuBar->addMenu("Cerca per");
    fileMenu->setObjectName("Cerca");

    QAction *all = fileMenu->addAction("Tutto");
    QAction *similiRiviste = fileMenu->addAction("Riviste");
    similiRiviste->setCheckable(true);
    QAction *similiLibri = fileMenu->addAction("Libri");
    similiLibri->setCheckable(true);
    QAction *similiManga = fileMenu->addAction("Manga");
    similiManga->setCheckable(true);
    QAction *similiCd = fileMenu->addAction("Cd");
    similiCd->setCheckable(true);
    QAction *similiFilm = fileMenu->addAction("Film");
    similiFilm->setCheckable(true);

    // Impostazioni layout del contenuto
    layout = new QGridLayout(scrollContent);
    layout->setContentsMargins(1, 1, 1, 1);
    layout->setSpacing(10);

    // Imposta il layout e il widget centrale
    scrollContent->setLayout(layout);
    setCentralWidget(scrollArea);  // Imposta la QScrollArea come widget centrale

    //visualizza ogni elemento
    showAll();

    // Connessioni dei filtri e dei pulsanti
    connect(all, &QAction::triggered, this, &AdminArea::showAll);
    connect(similiRiviste, &QAction::toggled, this, [this](bool checked){
        riviste = checked;
        showTipi();
    });
    connect(similiLibri, &QAction::toggled, this, [this](bool checked){
        libri = checked;
        showTipi();
    });
    connect(similiManga, &QAction::toggled, this, [this](bool checked){
        manga = checked;
        showTipi();
    });
    connect(similiFilm, &QAction::toggled, this, [this](bool checked){
        film = checked;
        showTipi();
    });
    connect(similiCd, &QAction::toggled, this, [this](bool checked){
        cd = checked;
        showTipi();
    });
    connect(indietro, &QPushButton::clicked, this, &AdminArea::tornaIndietro);
    connect(close, &QPushButton::clicked, this, &AdminArea::chiudi);
    connect(user, &QPushButton::clicked, this, &AdminArea::showUser);
    connect(aggiunta, &QPushButton::clicked, this, &AdminArea::aggiungi);
    connect(cercaOggetto, &QLineEdit::textChanged, this, [this, cercaOggetto](const QString& textChecked){
        testo = textChecked;
        cercaDigitato(testo);
    });
}

AdminArea::~AdminArea(){}

void AdminArea::onBibliotecaUpdated(const QList<Biblioteca*>& newBiblioteca){
    oggetti = newBiblioteca;
    showTipi();
}

void AdminArea::setOggetti(const QList<Biblioteca*>& newBiblioteca){
    oggetti = newBiblioteca;
}

void AdminArea::tornaIndietro(){
    showMain();
}

void AdminArea::showUser(){
    QMessageBox::information(this, "Parte utente", "Stai accedendo alla parte utente");
    film = false; riviste = false; cd = false;
    libri = false; manga = false;
    testo = "";
    QMenu* fileMenu = menuBar()->findChild<QMenu*>("Cerca");
    if (fileMenu)
        for (QAction* action : fileMenu->actions())
            action->setChecked(false);
    stack->setCurrentIndex(1);
}

void AdminArea::aggiungi(){
    stack->setCurrentIndex(3);
    emit createNewObject();
}

void AdminArea::chiudi(){
    stack->close();
}

void AdminArea::showMain(){
    film = false; riviste = false; cd = false;
    libri = false; manga = false;
    testo = "";
    stack->setCurrentIndex(0);
}

void AdminArea::showAll(){
    testo="";
    clearLayout(layout);
    libri = false; manga = false; cd = false;
    riviste = false; film = false;
    QMenu* fileMenu = menuBar()->findChild<QMenu*>("Cerca");
    if (fileMenu) {
        for (QAction* action : fileMenu->actions()){
            action->blockSignals(true);
            action->setChecked(false);
            action->blockSignals(false);
        }
    }
    int row=0, col=0; //variabili per righe e per colonne
    for(auto it = oggetti.begin(); it!= oggetti.end(); it++){
        VisitorWidget visitor;
        (*it)->accept(visitor);
        QWidget* widget = visitor.getWidget();
        QHBoxLayout* buttonLayout = new QHBoxLayout();

        QPushButton* elimina = new QPushButton("Elimina", widget);
        QPushButton* modifica = new QPushButton("Modifica", widget);
        gestisciPulsanti(*it, modifica, elimina);

        buttonLayout->addWidget(modifica);
        buttonLayout->addWidget(elimina);

        QWidget* containerWidget = new QWidget();
        QVBoxLayout* containerLayout = new QVBoxLayout();

        containerLayout->addLayout(buttonLayout);
        containerLayout->addWidget(widget);
        containerWidget->setLayout(containerLayout);
        containerWidget->setStyleSheet("QWidget{"
                                       "background-color: lightgray"
                                       "}");
        containerWidget->setFixedSize(475, 475);
        layout->addWidget(containerWidget, row, col, 1, 1, Qt::AlignCenter);
        col++;
        if (col % 3 == 0) {
            col = 0;
            row++;
        }
    }
}

void AdminArea::showTipi(){
    testo="";
    if(!film && !libri && !manga && !riviste && !cd){
        showAll();
        return;
    }
    clearLayout(layout);
    int row=0, col=0;
    for (auto it = oggetti.begin(); it != oggetti.end(); ++it) {
        QByteArray typeName = typeid(**it).name();
        // Verifica se il tipo esiste nella mappa e se il flag è true
        if (typeFlags.contains(typeName) && *typeFlags[typeName]) {
            VisitorWidget visitor;
            (*it)->accept(visitor);
            QWidget* widget = visitor.getWidget();
            QHBoxLayout* buttonLayout = new QHBoxLayout();

            QPushButton* elimina = new QPushButton("Elimina", widget);
            QPushButton* modifica = new QPushButton("Modifica", widget);
            gestisciPulsanti(*it, modifica, elimina);

            buttonLayout->addWidget(modifica);
            buttonLayout->addWidget(elimina);

            QWidget* containerWidget = new QWidget();
            QVBoxLayout* containerLayout = new QVBoxLayout();

            containerLayout->addLayout(buttonLayout);
            containerLayout->addWidget(widget);
            containerWidget->setLayout(containerLayout);
            containerWidget->setStyleSheet("QWidget{"
                                           "background-color: lightgray"
                                           "}");
            containerWidget->setFixedSize(475, 475);
            layout->addWidget(containerWidget, row, col, 1, 1, Qt::AlignCenter);
            col++;
            if (col % 3 == 0) {
                col = 0;
                row++;
            }
        }
    }
}

//funzione chiamata da showAll e showTipi per pulire il layout prima di aggiungere i nuovi widget
void AdminArea::clearLayout(QLayout* layout) {
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr)
        if (item->widget())
            //deleteLater serve per non rischiare problemi di accesso alla memoria
            item->widget()->deleteLater();
}

void AdminArea::cercaDigitato(const QString& testo){
    clearLayout(layout);
    QMenu* fileMenu = menuBar()->findChild<QMenu*>("Cerca");
    if (fileMenu) {
        for (QAction* action : fileMenu->actions()) {
            action->blockSignals(true);
            action->setChecked(false);
            action->blockSignals(false);
        }
    }
    film = false; cd = false; riviste = false;
    libri = false; manga = false;
    int row=0, col=0;
    bool match;
    for(auto it = oggetti.begin(); it!= oggetti.end(); it++){
        match = false;
        if((QString::fromStdString((*it)->getGenere())).startsWith(testo, Qt::CaseInsensitive))
            match = true;
        if(!match && (QString::fromStdString((*it)->getTitolo())).startsWith(testo, Qt::CaseInsensitive))
            match = true;
        if(!match && dynamic_cast<Cd*>(*it))
            if((QString::fromStdString(static_cast<Cd*>(*it)->getArtista())).startsWith(testo, Qt::CaseInsensitive))
                match = true;
        if(!match && dynamic_cast<Film*>(*it)){
            Film* film = static_cast<Film*>(*it);
            if((QString::fromStdString(film->getAttore())).startsWith(testo, Qt::CaseInsensitive) ||
                ((QString::fromStdString(film->getRegista())).startsWith(testo, Qt::CaseInsensitive)))
                match = true;
        }
        if(!match && dynamic_cast<Cartaceo*>(*it))
            if((QString::fromStdString(static_cast<Cartaceo*>(*it)->getAutore())).startsWith(testo, Qt::CaseInsensitive))
                match = true;
        if(match){
            VisitorWidget visitor;
            (*it)->accept(visitor);
            QWidget* widget = visitor.getWidget();
            QHBoxLayout* buttonLayout = new QHBoxLayout();

            QPushButton* elimina = new QPushButton("Elimina", widget);
            QPushButton* modifica = new QPushButton("Modifica", widget);
            gestisciPulsanti(*it, modifica, elimina);

            buttonLayout->addWidget(modifica);
            buttonLayout->addWidget(elimina);

            QWidget* containerWidget = new QWidget();
            QVBoxLayout* containerLayout = new QVBoxLayout();

            containerLayout->addLayout(buttonLayout);
            containerLayout->addWidget(widget);
            containerWidget->setLayout(containerLayout);
            containerWidget->setStyleSheet("QWidget{"
                                           "background-color: lightgray"
                                           "}");
            containerWidget->setFixedSize(475, 475);
            layout->addWidget(containerWidget, row, col, 1, 1, Qt::AlignCenter);
            col++;
            if (col % 3 == 0) {
                col = 0;
                row++;
            }
        }
    }
}

void AdminArea::gestisciPulsanti(Biblioteca* biblio, QPushButton* modifica, QPushButton* elimina)
{
    modifica->setStyleSheet("QPushButton{"
                            "background-color: white;"
                            "}"
                            "QPushButton::hover{"
                            "background-color:blue;"
                            "}"
                            "QPushButton::pressed{"
                            "background-color:red;"
                            "}");
    elimina->setStyleSheet("QPushButton{"
                           "background-color: white;"
                           "}"
                           "QPushButton::hover{"
                           "background-color:blue;"
                           "}"
                           "QPushButton::pressed{"
                           "background-color:red;"
                           "}");
    connect(elimina, &QPushButton::clicked, this, [this, biblio](){
        emit removeObject(biblio);
    });
    connect(modifica, &QPushButton::clicked, this, [this, biblio](){
        stack->setCurrentIndex(3);
        emit modifyObject(biblio);
    });
}

void AdminArea::handlePostAction() {
    std::sort(oggetti.begin(), oggetti.end(), [](Biblioteca* a, Biblioteca* b) {
        return a->getTitolo() < b->getTitolo();
    });
    if(testo != "") {
        cercaDigitato(testo);
        return;
    }
    if(!manga && !riviste && !film && !cd && !libri) {
        showAll();
        return;
    }
    showTipi();
}

void AdminArea::slotHandle(){
    handlePostAction();
}
