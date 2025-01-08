#include "adminarea.h"
#include "visitorwidget.h"
#include "../JSON/jsonmanager.h"
#include <QToolBar>
#include <QLineEdit>
#include <QPushButton>
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
    cercaOggetto->setPlaceholderText("Cerca per titolo, genere, attore, regista o autore");
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
    layout->setSpacing(0);

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
    connect(cercaOggetto, &QLineEdit::returnPressed, this, [this, cercaOggetto]() {
        testo = cercaOggetto->text();  // Ottieni il testo scritto nel QLineEdit
        cercaDigitato(testo);  // Passa il testo alla funzione cerca
        cercaOggetto->clear();
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
    QMenu* fileMenu = menuBar()->findChild<QMenu*>("Cerca");
    if (fileMenu)
        for (QAction* action : fileMenu->actions())
            action->setChecked(false);
    stack->setCurrentIndex(1);
}

void AdminArea::aggiungi(){
    QStringList classes{"Libri", "Riviste", "Manga", "Cd", "Film"};
    bool ok;
    QString selectedClass = QInputDialog::getItem(this, "Crea oggetto",
                                                  "Seleziona il tipo della classe", classes,
                                                  0, false, &ok);
    if(!ok || selectedClass.isEmpty()){
        QMessageBox::warning(this, "", "Operazione annullata");
        return;
    }
    emit objectCreationRequested(selectedClass);
}

void AdminArea::chiudi(){
    stack->close();
}

void AdminArea::showMain(){
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
    int i=0, j=0; //variabili per righe e per colonne
    for(auto cit = oggetti.begin(); cit!= oggetti.end(); cit++){
        VisitorWidget visitor;
        (*cit)->accept(visitor);
        QWidget* widget = visitor.getWidget();
        widget->installEventFilter(this);
        widget->setProperty("Biblioteca", QVariant::fromValue(*cit));
        layout->addWidget(widget, i, j, 1, 1, Qt::AlignCenter);
        j++;
        if(j%3==0){
            j=0;
            i++;
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
    int i=0, j=0;
    for (auto cit = oggetti.begin(); cit != oggetti.end(); ++cit) {
        QByteArray typeName = typeid(**cit).name();
        // Verifica se il tipo esiste nella mappa e se il flag è true
        if (typeFlags.contains(typeName) && *typeFlags[typeName]) {
            VisitorWidget visitor;
            (*cit)->accept(visitor);
            QWidget* widget = visitor.getWidget();
            widget->installEventFilter(this);
            //gli dico che il nuovo widget ha la proprietà di essere effettivamente
            //un Biblioteca*
            widget->setProperty("Biblioteca", QVariant::fromValue(*cit));
            layout->addWidget(widget, i, j, 1, 1, Qt::AlignCenter);
            j++;
            if(j%3==0){
                j=0;
                i++;
            }
        }
    }
}

bool AdminArea::eventFilter(QObject *watched, QEvent *event){
    if(event->type() == QEvent::MouseButtonDblClick){
        auto* clickedWidget = qobject_cast<QWidget*>(watched);
        if (clickedWidget) {
            QVariant prop = clickedWidget->property("Biblioteca");
            if (prop.isValid()) {
                Biblioteca* biblioteca = prop.value<Biblioteca*>();
                if (biblioteca) {
                    mostraMenu(biblioteca);
                    return true;
                }
            }
        }
    }
    return QObject::eventFilter(watched, event);
}

void AdminArea::mostraMenu(Biblioteca* biblio) {
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle(QString::fromStdString(biblio->getTitolo()));
    dialog->setWindowFlags(dialog->windowFlags() | Qt::WindowStaysOnTopHint);
    dialog->resize(300, 300);

    QVBoxLayout* dialogLayout = new QVBoxLayout(dialog);

    QPushButton* modifica = new QPushButton("Modifica", dialog);
    QPushButton* elimina = new QPushButton("Elimina", dialog);
    QPushButton* annulla = new QPushButton("Annulla", dialog);

    dialogLayout->addWidget(modifica);
    dialogLayout->addWidget(elimina);
    dialogLayout->addWidget(annulla);

    connect(modifica, &QPushButton::clicked, this, [this, biblio, dialog](){
        emit modifyObject(biblio);
        dialog->reject();
    });
    connect(elimina, &QPushButton::clicked, this,[this, biblio, dialog](){
        QMessageBox messageBox(this);
        messageBox.setWindowTitle("Conferma eliminazione");
        messageBox.setText("Sei sicuro di voler eliminare?");
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        messageBox.setDefaultButton(QMessageBox::No);
        if(messageBox.exec() == QMessageBox::Yes)
            emit removeObject(biblio);
        dialog->reject();
    });
    connect(annulla, &QPushButton::clicked, dialog, [dialog]() {
        dialog->reject();
    });
    dialog->exec();
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
    int i=0, j=0;
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
            widget->installEventFilter(this);
            widget->setProperty("Biblioteca", QVariant::fromValue(*it));
            layout->addWidget(widget, i, j, 1, 1, Qt::AlignCenter);
            j++;
            if(j%3==0){
                j=0;
                i++;
            }
        }
    }
}
