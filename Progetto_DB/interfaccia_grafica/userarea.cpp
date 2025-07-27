#include "userarea.h"
#include "visitorwidget.h"
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QLineEdit>
#include <QToolBar>
#include <QHBoxLayout>

UserArea::UserArea(QList<Biblioteca *> objects, QStackedWidget *stackWidget, QWidget *parent) :
    QMainWindow(parent), oggetti(objects), stack(stackWidget),
    film(false), libri(false), manga(false), riviste(false), cd(false)
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
    QPushButton* user = new QPushButton("Area admin", scrollContent);
    toolBar->addWidget(user);
    QPushButton* close = new QPushButton("Chiudi", scrollContent);
    toolBar->addWidget(close);
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
    connect(all, &QAction::triggered, this, &UserArea::showAll);
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
    connect(indietro, &QPushButton::clicked, this, &UserArea::tornaIndietro);
    connect(close, &QPushButton::clicked, this, &UserArea::chiudi);
    connect(user, &QPushButton::clicked, this, &UserArea::admin);
    connect(cercaOggetto, &QLineEdit::textChanged, this, [this, cercaOggetto](const QString& textChecked){
        testo = textChecked;
        cercaDigitato(testo);
    });
}

UserArea::~UserArea(){}

void UserArea::onBibliotecaUpdated(const QList<Biblioteca *> &newBiblioteca){
    oggetti = newBiblioteca;
    std::sort(oggetti.begin(), oggetti.end(), [](Biblioteca* a, Biblioteca* b){
        return a->getTitolo() < b->getTitolo();
    });
    showTipi();
}

//segnali
void UserArea::tornaIndietro(){
    showMain();
}

void UserArea::chiudi(){
    stack->close();
}

void UserArea::admin(){
    QDialog *loginDialog = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout(loginDialog);
    QLabel *user = new QLabel("Username",loginDialog);
    QLineEdit *log = new QLineEdit(loginDialog);
    QLabel *password = new QLabel("Password",loginDialog);
    QLineEdit *pas = new QLineEdit(loginDialog);
    pas->setEchoMode(QLineEdit::Password);  // Imposta la modalità password

    QPushButton *submitButton = new QPushButton("Accedi", loginDialog);
    layout->addWidget(user);
    layout->addWidget(log);
    layout->addWidget(password);
    layout->addWidget(pas);
    layout->addWidget(submitButton);

    loginDialog->setLayout(layout);
    loginDialog->setWindowTitle("Login");

    connect(submitButton, &QPushButton::clicked, this, [this, log, pas, loginDialog]() {
        if (log->text() == "admin" && pas->text() == "admin"){
            loginDialog->accept();
            showAdmin();
        }
        else{
            pas->clear();
            QMessageBox::warning(this, "Attenzione", "Username o password errate");
        }
    });
    loginDialog->exec();
}

void UserArea::showMain(){
    film = false; riviste = false; cd = false; libri = false; manga = false;
    simili = false; testo = "";
    stack->setCurrentIndex(0);
}

void UserArea::showAdmin(){
    QMessageBox::information(this, "Parte admin", "Stai accedendo alla parte admin");
    film = false; riviste = false; cd = false; libri = false; manga = false;
    simili = false; testo = "";
    QMenu* fileMenu = menuBar()->findChild<QMenu*>("Cerca");
    if (fileMenu)
        for (QAction* action : fileMenu->actions())
            action->setChecked(false);
    stack->setCurrentIndex(2);
}

void UserArea::showAll() {
    simili = false;
    testo = "";
    libri = false; manga = false; cd = false; riviste = false; film = false;

    clearLayout(layout);

    QMenu* fileMenu = menuBar()->findChild<QMenu*>("Cerca");
    if (fileMenu) {
        for (QAction* action : fileMenu->actions()) {
            action->blockSignals(true);
            action->setChecked(false);
            action->blockSignals(false);
        }
    }
    int row = 0, col = 0;

    for (auto cit = oggetti.begin(); cit != oggetti.end(); ++cit) {
        Visitor* visitor = new VisitorWidget();
        (*cit)->accept(visitor);
        QWidget* widget = (static_cast<VisitorWidget*>(visitor))->getWidget();
        QHBoxLayout* buttonLayout = new QHBoxLayout();

        QPushButton* prenota = new QPushButton("Prenota", widget);
        prenota->setStyleSheet("QPushButton{"
                               "background-color: green;"
                               "}"
                               "QPushButton::hover{"
                               "background-color:blue;"
                               "}"
                               "QPushButton::pressed{"
                               "background-color:red;"
                               "}");
        QPushButton* restituisci = new QPushButton("Restituisci", widget);
        restituisci->setStyleSheet("QPushButton{"
                                   "background-color: green;"
                                   "}"
                                   "QPushButton::hover{"
                                   "background-color:blue;"
                                   "}"
                                   "QPushButton::pressed{"
                                   "background-color:red;"
                                   "}");
        QPushButton* suggerisci = new QPushButton("Simili", widget);
        suggerisci->setStyleSheet("QPushButton{"
                                  "background-color: green;"
                                  "}"
                                  "QPushButton::hover{"
                                  "background-color:blue;"
                                  "}"
                                  "QPushButton::pressed{"
                                  "background-color:red;"
                                  "}");
        QPushButton* azione = new QPushButton(widget);
        azione->setStyleSheet("QPushButton{"
                              "background-color: green;"
                              "}"
                              "QPushButton::hover{"
                              "background-color:blue;"
                              "}"
                              "QPushButton::pressed{"
                              "background-color:red;"
                              "}");
        gestisciAzione(*cit, azione);

        buttonLayout->addWidget(prenota);
        buttonLayout->addWidget(restituisci);
        buttonLayout->addWidget(suggerisci);
        buttonLayout->addWidget(azione);

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
        gestisciConnect(*cit, prenota, restituisci, suggerisci);
        delete visitor;
    }
}

void UserArea::showTipi() {
    simili = false;
    testo="";
    if(!film && !libri && !manga && !riviste && !cd){
        showAll();
        return;
    }
    clearLayout(layout);
    int row=0, col=0;
    for (auto cit = oggetti.begin(); cit != oggetti.end(); ++cit) {
        QByteArray typeName = typeid(**cit).name();
        // Verifica se il tipo esiste nella mappa e se il flag è true
        if (typeFlags.contains(typeName) && *typeFlags[typeName]) {
            Visitor* visitor = new VisitorWidget();
            (*cit)->accept(visitor);
            QWidget* widget = (static_cast<VisitorWidget*>(visitor))->getWidget();
            QHBoxLayout* buttonLayout = new QHBoxLayout();
            QPushButton* prenota = new QPushButton("Prenota", widget);
            prenota->setStyleSheet("QPushButton{"
                                   "background-color: green;"
                                   "}"
                                   "QPushButton::hover{"
                                   "background-color:blue;"
                                   "}"
                                   "QPushButton::pressed{"
                                   "background-color:red;"
                                   "}");
            QPushButton* restituisci = new QPushButton("Restituisci", widget);
            restituisci->setStyleSheet("QPushButton{"
                                       "background-color: green;"
                                       "}"
                                       "QPushButton::hover{"
                                       "background-color:blue;"
                                       "}"
                                       "QPushButton::pressed{"
                                       "background-color:red;"
                                       "}");
            QPushButton* suggerisci = new QPushButton("Simili", widget);
            suggerisci->setStyleSheet("QPushButton{"
                                      "background-color: green;"
                                      "}"
                                      "QPushButton::hover{"
                                      "background-color:blue;"
                                      "}"
                                      "QPushButton::pressed{"
                                      "background-color:red;"
                                      "}");
            QPushButton* azione = new QPushButton(widget);
            azione->setStyleSheet("QPushButton{"
                                  "background-color: green;"
                                  "}"
                                  "QPushButton::hover{"
                                  "background-color:blue;"
                                  "}"
                                  "QPushButton::pressed{"
                                  "background-color:red;"
                                  "}");
            gestisciAzione(*cit, azione);

            buttonLayout->addWidget(prenota);
            buttonLayout->addWidget(restituisci);
            buttonLayout->addWidget(suggerisci);
            buttonLayout->addWidget(azione);

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
            gestisciConnect(*cit, prenota, restituisci, suggerisci);
            delete visitor;
        }
    }
}

//funzione chiamata da showAll e showTipi per pulire il layout prima di aggiungere i nuovi widget
void UserArea::clearLayout(QLayout* layout) {
    // Rimuove tutti i widget dal layout
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr)
        if (item->widget())
            item->widget()->deleteLater(); //deleteLater serve per non rischiare problemi di accesso alla memoria
}

void UserArea::handlePostAction(Biblioteca* biblio) {
    qDebug() << "Funzione handlePostAction chiamata";
    std::sort(oggetti.begin(), oggetti.end(), [](Biblioteca* a, Biblioteca* b) {
        return a->getTitolo() < b->getTitolo();
    });
    if(testo != "") {
        cercaDigitato(testo);
        return;
    }
    if(simili) {
        suggerisciSimili(biblio);
        return;
    }
    if(!manga && !riviste && !film && !cd && !libri) {
        showAll();
        return;
    }
    showTipi();
}


void UserArea::cercaDigitato(const QString& testo){
    clearLayout(layout);
    //cancella tutti i filtri assegnati in precedenza trovando il nome Cerca
    QMenu* fileMenu = menuBar()->findChild<QMenu*>("Cerca");
    if (fileMenu) {
        for (QAction* action : fileMenu->actions()) {
            // Disabilita temporaneamente il segnale
            action->blockSignals(true);
            // Modifica lo stato del checkbox
            action->setChecked(false);
            // Riapri il segnale
            action->blockSignals(false);
        }
    }
    film = false; cd = false; riviste = false;
    libri = false; manga = false;
    int row=0, col=0;
    bool match;
    for(auto cit = oggetti.begin(); cit!= oggetti.end(); cit++){
        match = false;
        if((QString::fromStdString((*cit)->getTitolo())).startsWith(testo, Qt::CaseInsensitive))
            match = true;
        if(!match && (QString::fromStdString((*cit)->getGenere())).startsWith(testo, Qt::CaseInsensitive))
            match = true;
        if(!match && dynamic_cast<Cd*>(*cit))
            if((QString::fromStdString(static_cast<Cd*>(*cit)->getArtista())).startsWith(testo, Qt::CaseInsensitive))
                match = true;
        if(!match && dynamic_cast<Film*>(*cit)){
            Film* film = static_cast<Film*>(*cit);
            if((QString::fromStdString(film->getAttore())).startsWith(testo, Qt::CaseInsensitive) ||
                ((QString::fromStdString(film->getRegista())).startsWith(testo, Qt::CaseInsensitive)))
                match = true;
        }
        if(!match && dynamic_cast<Cartaceo*>(*cit))
            if((QString::fromStdString(static_cast<Cartaceo*>(*cit)->getAutore())).startsWith(testo, Qt::CaseInsensitive))
                match = true;
        if(match){
            Visitor* visitor = new VisitorWidget();
            (*cit)->accept(visitor);
            QWidget* widget = (static_cast<VisitorWidget*>(visitor))->getWidget();
            QHBoxLayout* buttonLayout = new QHBoxLayout();
            QPushButton* prenota = new QPushButton("Prenota", widget);
            prenota->setStyleSheet("QPushButton{"
                                   "background-color: green;"
                                   "}"
                                   "QPushButton::hover{"
                                   "background-color:blue;"
                                   "}"
                                   "QPushButton::pressed{"
                                   "background-color:red;"
                                   "}");
            QPushButton* restituisci = new QPushButton("Restituisci", widget);
            restituisci->setStyleSheet("QPushButton{"
                                       "background-color: green;"
                                       "}"
                                       "QPushButton::hover{"
                                       "background-color:blue;"
                                       "}"
                                       "QPushButton::pressed{"
                                       "background-color:red;"
                                       "}");
            QPushButton* suggerisci = new QPushButton("Simili", widget);
            suggerisci->setStyleSheet("QPushButton{"
                                      "background-color: green;"
                                      "}"
                                      "QPushButton::hover{"
                                      "background-color:blue;"
                                      "}"
                                      "QPushButton::pressed{"
                                      "background-color:red;"
                                      "}");
            QPushButton* azione = new QPushButton(widget);
            azione->setStyleSheet("QPushButton{"
                                  "background-color: green;"
                                  "}"
                                  "QPushButton::hover{"
                                  "background-color:blue;"
                                  "}"
                                  "QPushButton::pressed{"
                                  "background-color:red;"
                                  "}");
            gestisciAzione(*cit, azione);

            buttonLayout->addWidget(prenota);
            buttonLayout->addWidget(restituisci);
            buttonLayout->addWidget(suggerisci);
            buttonLayout->addWidget(azione);

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
            gestisciConnect(*cit, prenota, restituisci, suggerisci);
            delete visitor;
        }
    }
}

void UserArea::suggerisciSimili(Biblioteca* biblio){
    simili = true;
    testo = "";
    clearLayout(layout);QString genere = QString::fromStdString(biblio->getGenere());
    QString artista, regista, attore, autore;
    if(dynamic_cast<Cd*>(biblio))
        artista=QString::fromStdString
            (static_cast<Cd*>(biblio)->getArtista());
    if(dynamic_cast<Film*>(biblio)){
        regista = QString::fromStdString
            (static_cast<Film*>(biblio)->getRegista());
        attore = QString::fromStdString
            (static_cast<Film*>(biblio)->getAttore());
    }
    if(dynamic_cast<Libri*>(biblio))
        autore = QString::fromStdString
            (static_cast<Libri*>(biblio)->getAutore());
    int row=0, col=0;
    for (auto cit = oggetti.begin(); cit != oggetti.end(); ++cit) {
        bool match = false;
        if (QString::fromStdString((*cit)->getGenere()) == genere)
            match = true;
        if (!match && dynamic_cast<Cd*>(*cit))
            if (QString::fromStdString(static_cast<Cd*>(*cit)->getArtista()) == artista)
                match = true;
        if (!match && dynamic_cast<Film*>(*cit))
            if (QString::fromStdString(static_cast<Film*>(*cit)->getAttore()) == attore
                || QString::fromStdString(static_cast<Film*>(*cit)->getRegista())== regista)
                match = true;
        if (!match && dynamic_cast<Libri*>(*cit))
                    if (QString::fromStdString(static_cast<Libri*>(*cit)->getAutore()) == autore)
                match = true;
        // Se almeno una condizione è soddisfatta, continua con la creazione del widget
        if (match) {
            Visitor* visitor = new VisitorWidget();
            (*cit)->accept(visitor);
            QWidget* widget = (static_cast<VisitorWidget*>(visitor))->getWidget();
            QHBoxLayout* buttonLayout = new QHBoxLayout();
            QPushButton* prenota = new QPushButton("Prenota", widget);
            prenota->setStyleSheet("QPushButton{"
                                   "background-color: green;"
                                   "}"
                                   "QPushButton::hover{"
                                   "background-color:blue;"
                                   "}"
                                   "QPushButton::pressed{"
                                   "background-color:red;"
                                   "}");
            QPushButton* restituisci = new QPushButton("Restituisci", widget);
            restituisci->setStyleSheet("QPushButton{"
                                       "background-color: green;"
                                       "}"
                                       "QPushButton::hover{"
                                       "background-color:blue;"
                                       "}"
                                       "QPushButton::pressed{"
                                       "background-color:red;"
                                       "}");
            QPushButton* suggerisci = new QPushButton("Simili", widget);
            suggerisci->setStyleSheet("QPushButton{"
                                      "background-color: green;"
                                      "}"
                                      "QPushButton::hover{"
                                      "background-color:blue;"
                                      "}"
                                      "QPushButton::pressed{"
                                      "background-color:red;"
                                      "}");
            QPushButton* azione = new QPushButton(widget);
            azione->setStyleSheet("QPushButton{"
                                  "background-color: green;"
                                  "}"
                                  "QPushButton::hover{"
                                  "background-color:blue;"
                                  "}"
                                  "QPushButton::pressed{"
                                  "background-color:red;"
                                  "}");
            gestisciAzione(*cit, azione);

            buttonLayout->addWidget(prenota);
            buttonLayout->addWidget(restituisci);
            buttonLayout->addWidget(suggerisci);
            buttonLayout->addWidget(azione);

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
            gestisciConnect(*cit, prenota, restituisci, suggerisci);
            delete visitor;
        }
    }
}

void UserArea::gestisciAzione(Biblioteca* biblio, QPushButton* azione){
    if (dynamic_cast<Cartaceo*>(biblio)) {
        if (static_cast<Cartaceo*>(biblio)->getLetto())
            azione->setText("Non leggere l'oggetto");
        else
            azione->setText("Leggi l'oggetto");
    }
    if (dynamic_cast<Film*>(biblio)) {
        if (static_cast<Film*>(biblio)->getVisto())
            azione->setText("Non guardare l'oggetto");
        else
            azione->setText("Guarda l'oggetto");
    }
    if (dynamic_cast<Cd*>(biblio)) {
        if (static_cast<Cd*>(biblio)->getAscoltato())
            azione->setText("Non ascoltare l'oggetto");
        else
            azione->setText("Ascolta l'oggetto");
    }
    connect(azione, &QPushButton::clicked, this, [this, biblio](){
        if(dynamic_cast<Cartaceo*>(biblio)){
            Cartaceo* carta = static_cast<Cartaceo*>(biblio);
            carta->segnaLetto();
            if(carta->getLetto())
                QMessageBox::information(this, "Letto",
                                         "Hai appena letto l'oggetto");
            else
                QMessageBox::information(this, "Letto",
                                         "Hai tolto lo stato\n"
                                         "di letto all'oggetto");
            emit letto(carta->getId(), carta);
            handlePostAction(biblio);
        }
        if(dynamic_cast<Film*>(biblio)){
            Film* film = static_cast<Film*>(biblio);
            film->segnaVisto();
            if(film->getVisto())
                QMessageBox::information(this, "Visto",
                                         "Hai appena guardato l'oggetto");
            else
                QMessageBox::information(this, "Visto",
                                         "Hai tolto lo stato\n"
                                         "di visto all'oggetto");
            emit visto(film->getId(), film);
            handlePostAction(biblio);
        }
        if(dynamic_cast<Cd*>(biblio)){
            Cd* cd = static_cast<Cd*>(biblio);
            cd->segnaAscoltato();
            if(cd->getAscoltato())
                QMessageBox::information(this, "Ascoltato",
                                         "Hai appena ascoltato l'oggetto");
            else
                QMessageBox::information(this, "Ascoltato",
                                         "Hai tolto lo stato\n"
                                         "di ascoltato all'oggetto");
            emit ascoltato(cd->getId(), cd);
            handlePostAction(biblio);
        }
    });
}

void UserArea::gestisciConnect(Biblioteca* biblio, QPushButton* prenotazione,
                               QPushButton* restituzione, QPushButton* suggerisci)
{
    connect(prenotazione, &QPushButton::clicked, this, [this, biblio]() {
        if (biblio->prenota()) {
            if(biblio->getCopie()!=1)
                QMessageBox::information(this, "Successo", "La prenotazione è avvenuta con successo\n"
                                                           "Rimangono " + QString::number(biblio->getCopie()) + " copie disponibili");
            else
                QMessageBox::information(this, "Successo", "La prenotazione è avvenuta con successo\n"
                                                           "Rimane 1 copia disponibile");
            emit prenota(biblio->getId());
            handlePostAction(biblio);
                                                        }
        else {
            QMessageBox::warning(this, "Fallimento", "La prenotazione non ha avuto successo\n"
                                                     "Non ci sono copie disponibili");
        }
    });

    connect(restituzione, &QPushButton::clicked, this, [this, biblio]() {
        if (biblio->restituisci()) {
            QMessageBox::information(this, "Successo", "La restituzione è avvenuta con successo");
            emit restituisci(biblio->getId());
            handlePostAction(biblio);
        }
        else {
            QMessageBox::warning(this, "Fallimento", "La restituzione non ha avuto successo\n"
                                                     "Non ci sono prestiti in atto di quest'oggetto");
        }
        
    });

    connect(suggerisci, &QPushButton::clicked, this, [this, biblio]() {
        suggerisciSimili(biblio);
    });
}