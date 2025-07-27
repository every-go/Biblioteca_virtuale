#include "librarymanager.h"
#include <QComboBox>
#include <QToolBar>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QFileDialog>
#include <QFileInfo>
#include <QSpacerItem>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <typeindex>
#include <climits>
#include <cfloat>

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
    scrollContent->setLayout(layout);
    setCentralWidget(scrollArea);
    modifyLayout = new QVBoxLayout();
    connect(indietro, &QPushButton::clicked, this, &LibraryManager::tornaIndietro);
    connect(close, &QPushButton::clicked, this, &LibraryManager::close);
}

void LibraryManager::close(){
    stack->close();
}

void LibraryManager::tornaIndietro(){
    stack->setCurrentIndex(2);
}

void LibraryManager::clearLayout(QLayout* layout){
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr)
        if (item->widget())
            //deleteLater serve per non rischiare problemi di accesso alla memoria
            item->widget()->deleteLater();
}

void LibraryManager::createObject(){
    clearLayout(layout);
    Biblioteca* biblio = nullptr;
    QPushButton* crea = new QPushButton("Crea", scrollContent);
    crea->setStyleSheet("QPushButton{"
                        "background-color: white;"
                        "}");
    QStringList classNames{"Libri","Manga","Riviste","Cd","Film"};
    QComboBox* selezione = new QComboBox(scrollContent);
    selezione->setEditable(true);
    selezione->lineEdit()->setAlignment(Qt::AlignCenter);
    selezione->addItem("Seleziona un'opzione");
    selezione->addItems(classNames);
    selezione->setCurrentText("Seleziona un'opzione");
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
    layout->addLayout(modifyLayout);
    layout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout->addLayout(fixedLayout);
    connect(selezione, &QComboBox::currentTextChanged, this, [this, selezione](const QString& text){
        if(text == "Seleziona un'opzione")
            clearLayout(modifyLayout);
        if(text == "Riviste")
            menuRiviste();
        if(text == "Manga")
            menuManga();
        if(text == "Libri")
            menuLibri();
        if(text == "Cd")
            menuCd();
        if(text == "Film")
            menuFilm();
    });

    // Chiamata iniziale per simulare l'azione del segnale
    emit selezione->currentTextChanged(selezione->currentText());

    connect(crea, &QPushButton::clicked, this, [this, &biblio, selezione](){
        if(selezione->currentText() == "Riviste")
            biblio = createRiviste();
        else if(selezione->currentText() == "Libri")
            biblio = createLibri();
        else if(selezione->currentText() == "Manga")
            biblio = createManga();
        else if(selezione->currentText() == "Cd")
            biblio = createCd();
        else if(selezione->currentText() == "Film")
            biblio = createFilm();
        else
            return;
        if(biblio){
            emit newObject(biblio);
            stack->setCurrentIndex(2);
            emit handle();
        }
    });
}

void LibraryManager::modifyObject(Biblioteca* biblio){
    clearLayout(layout);
    QPushButton* salva = new QPushButton("Salva", scrollContent);
    salva->setStyleSheet("QPushButton{"
                        "background-color: white;"
                        "}"
                         "QPushButton::pressed{"
                         "background-color: red;"
                         "}");
    QStringList classNames{"Libri","Manga","Riviste","Cd","Film"};
    QComboBox* selezione = new QComboBox(scrollContent);
    selezione->setEditable(true);
    selezione->lineEdit()->setAlignment(Qt::AlignCenter);
    selezione->addItems(classNames);
    selezione->setEnabled(false);
    selezione->setStyleSheet("QComboBox{"
                             "background-color:white;"
                             "color: black;"
                             "font-weight: bold;"
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
    fixedLayout->addWidget(salva);
    layout->addLayout(dynamicLayout);
    layout->addLayout(modifyLayout);
    layout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout->addLayout(fixedLayout);

    QMap<std::type_index, std::function<void(Biblioteca*)>> menuMap = {
        {typeid(Manga), [this, selezione](Biblioteca* biblio) {
             selezione->setCurrentText("Manga");
             menuManga(static_cast<Manga*>(biblio));
         }},
        {typeid(Libri), [this, selezione](Biblioteca* biblio) {
             selezione->setCurrentText("Libri");
             menuLibri(static_cast<Libri*>(biblio));
         }},
        {typeid(Riviste), [this, selezione](Biblioteca* biblio) {
             selezione->setCurrentText("Riviste");
             menuRiviste(static_cast<Riviste*>(biblio));
         }},
        {typeid(Film), [this, selezione](Biblioteca* biblio) {
             selezione->setCurrentText("Film");
             menuFilm(static_cast<Film*>(biblio));
         }},
        {typeid(Cd), [this, selezione](Biblioteca* biblio) {
             selezione->setCurrentText("Cd");
             menuCd(static_cast<Cd*>(biblio));
         }}
    };
    std::type_index tipo(typeid(*biblio));
    if (menuMap.contains(tipo))
        menuMap[tipo](biblio);

    connect(salva, &QPushButton::clicked, this, [this, biblio, selezione](){
        if(selezione->currentText() == "Riviste"){
            save(static_cast<Riviste*>(biblio));
        }
        if(selezione->currentText() == "Libri"){
            save(static_cast<Libri*>(biblio));
        }
        if(selezione->currentText() == "Manga"){
            save(static_cast<Manga*>(biblio));
        }
        if(selezione->currentText() == "Cd"){
            save(static_cast<Cd*>(biblio));
        }
        if(selezione->currentText() == "Film"){
            save(static_cast<Film*>(biblio));
        }
        if(biblio){
            emit update(biblio->getId(), biblio);
            stack->setCurrentIndex(2);
            emit handle();
        }
    });
}

void LibraryManager::menuBiblio(Biblioteca* biblio){
    image = "";
    clearLayout(modifyLayout);
    QLabel* titolo = new QLabel(scrollContent);
    titolo->setText("<div style = 'text-align: center;'>"
                    "<b>Titolo</b></div>");
    titolo->setStyleSheet("QLabel{"
                          "background-color: lightblue;"
                          "}");
    QLineEdit* menuTitolo = new QLineEdit(scrollContent);
    menuTitolo->setAlignment(Qt::AlignCenter);
    menuTitolo->setStyleSheet("QLineEdit{"
                              "background-color: lightgray;"
                              "}");
    menuTitolo->setText(biblio?
                        QString::fromStdString(biblio->getTitolo()) : "");
    menuWidgets["Titolo"] = menuTitolo;
    modifyLayout->addWidget(titolo);
    modifyLayout->addWidget(menuTitolo);

    QLabel* genere = new QLabel(scrollContent);
    genere->setText("<div style = 'text-align: center;'>"
                    "<b>Genere</b></div>");
    genere->setStyleSheet("QLabel{"
                          "background-color: lightblue;"
                          "}");
    QLineEdit* menuGenere = new QLineEdit(scrollContent);
    menuGenere->setAlignment(Qt::AlignCenter);
    menuGenere->setStyleSheet("QLineEdit{"
                              "background-color: lightgray;"
                              "}");
    menuGenere->setText(biblio?
                        QString::fromStdString(biblio->getGenere()) : "");
    menuWidgets["Genere"] = menuGenere;
    modifyLayout->addWidget(genere);
    modifyLayout->addWidget(menuGenere);

    QLabel* anno = new QLabel(scrollContent);
    anno->setText("<div style = 'text-align: center;'>"
                    "<b>Anno</b></div>");
    anno->setStyleSheet("QLabel{"
                          "background-color: lightblue;"
                          "}");
    QSpinBox* menuAnno = new QSpinBox(scrollContent);
    menuAnno->setMaximum(INT_MAX);
    menuAnno->setMinimum(INT_MIN);
    menuAnno->setAlignment(Qt::AlignCenter);
    menuAnno->setStyleSheet("QSpinBox{"
                             "background-color: lightgray;"
                             "}");
    menuAnno->setValue(biblio?
                       biblio->getAnno() : 0);
    menuWidgets["Anno"] = menuAnno;
    modifyLayout->addWidget(anno);
    modifyLayout->addWidget(menuAnno);

    QLabel* costo = new QLabel(scrollContent);
    costo->setText("<div style = 'text-align: center;'>"
                  "<b>Costo</b></div>");
    costo->setStyleSheet("QLabel{"
                          "background-color: lightblue;"
                          "}");
    QDoubleSpinBox* menuCosto = new QDoubleSpinBox(scrollContent);
    menuCosto->setMaximum(DBL_MAX);
    menuCosto->setMinimum(0.01);
    menuCosto->setAlignment(Qt::AlignCenter);
    menuCosto->setValue(biblio?
                        biblio->getCosto() : 4.99);
    menuCosto->setSingleStep(0.01);
    menuCosto->setStyleSheet("QDoubleSpinBox{"
                             "background-color: lightgray;"
                             "}");
    menuWidgets["Costo"] = menuCosto;
    modifyLayout->addWidget(costo);
    modifyLayout->addWidget(menuCosto);

    if(!biblio){
        // Creazione del QLabel e del QComboBox come prima
        QLabel* disponibile = new QLabel(scrollContent);
        disponibile->setText("<div style = 'text-align: center;'>"
                             "<b>Vuoi impostarlo disponibile?</b></div>");
        disponibile->setStyleSheet("QLabel{"
                                   "background-color: lightblue; "
                                   "}");
        QComboBox* menuDisponibile = new QComboBox(scrollContent);
        menuDisponibile->setEditable(true);
        menuDisponibile->lineEdit()->setAlignment(Qt::AlignCenter);
        menuDisponibile->addItems({"Disponibile","Non disponibile"});
        menuDisponibile->setStyleSheet("QComboBox{"
                                       "background-color: lightgray; "
                                       "}");
        menuDisponibile->setCurrentText("Disponibile");
        menuWidgets["Disponibile"] = menuDisponibile;
        modifyLayout->addWidget(disponibile);
        modifyLayout->addWidget(menuDisponibile);

        // Creazione del QLabel e del QSpinBox come prima
        QLabel* copie = new QLabel(scrollContent);
        copie->setText("<div style = 'text-align: center;'>"
                       "<b>Numero copie</b></div>");
        copie->setStyleSheet("QLabel{"
                             "background-color: lightblue; "
                             "}");
        QSpinBox* menuCopie = new QSpinBox(scrollContent);
        menuCopie->setStyleSheet("QSpinBox{"
                                 "background-color: lightgray; "
                                 "}");
        menuCopie->setAlignment(Qt::AlignCenter);
        connect(menuDisponibile, &QComboBox::currentTextChanged, this, [this, menuCopie](const QString& isDisponibile) {
            if (isDisponibile == "Disponibile") {
                menuCopie->setMinimum(1);
                menuCopie->setMaximum(INT_MAX);
            } else {
                menuCopie->setMinimum(0);
                menuCopie->setMaximum(0);
            }
        });

        menuCopie->setMinimum(1);
        menuCopie->setMaximum(INT_MAX);
        menuWidgets["Copie"] = menuCopie;
        modifyLayout->addWidget(copie);
        modifyLayout->addWidget(menuCopie);

        QLabel* prestiti = new QLabel(scrollContent);
        prestiti->setText("<div style = 'text-align: center;'>"
                          "<b>Numero prestiti</b></div>");
        prestiti->setStyleSheet("QLabel{"
                                "background-color: lightblue;"
                                "}");
        QSpinBox* menuPrestiti = new QSpinBox(scrollContent);
        menuPrestiti->setMinimum(1);
        menuPrestiti->setMaximum(INT_MAX);
        connect(menuCopie, &QSpinBox::valueChanged, this, [this, menuPrestiti](int numero){
            menuPrestiti->setMinimum(numero>=1 ? numero : 1);
        });
        menuPrestiti->setStyleSheet("QSpinBox{"
                                    "background-color: lightgray;"
                                    "}");
        menuPrestiti->setAlignment(Qt::AlignCenter);
        menuWidgets["Prestiti"] = menuPrestiti;
        modifyLayout->addWidget(prestiti);
        modifyLayout->addWidget(menuPrestiti);
    }
    else{
        QLabel* prestiti = new QLabel(scrollContent);
        prestiti->setText("<div style = 'text-align: center;'>"
                          "<b>Numero prestiti</b></div>");
        prestiti->setStyleSheet("QLabel{"
                                "background-color: lightblue;"
                                "}");
        QSpinBox* menuPrestiti = new QSpinBox(scrollContent);
        menuPrestiti->setMaximum(INT_MAX);
        menuPrestiti->setMinimum(biblio->getCopie());
        menuPrestiti->setStyleSheet("QSpinBox{"
                                    "background-color: lightgray;"
                                    "}");
        menuPrestiti->setAlignment(Qt::AlignCenter);
        menuWidgets["Prestiti"] = menuPrestiti;
        modifyLayout->addWidget(prestiti);
        modifyLayout->addWidget(menuPrestiti);
    }
    QLabel* immagine = new QLabel(scrollContent);
    immagine->setText("<div style = 'text-align: center;'>"
                      "<b>Inserisci l'immagine</b></div>");
    immagine->setStyleSheet("QLabel{"
                            "background-color: lightblue;"
                            "}");
    QPushButton* menuImmagine = new QPushButton(scrollContent);
    menuImmagine->setText(biblio? "Modifica immagine" : "Scegli immagine");
    menuImmagine->setStyleSheet("QPushButton{"
                                "background-color: lightgray;"
                                "}"
                                "QPushButton::hover{"
                                "background-color: green"
                                "}");
    //stringa della classe
    connect(menuImmagine, &QPushButton::clicked, this, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this, "Seleziona un'immagine", "", "Immagini (*.png *.jpg *.jpeg *.bmp)");
        QString fileName = QFileInfo(filePath).fileName();
        QString targetPath = QDir("IMG").filePath(fileName);
        QDir().mkpath("IMG");
        image = targetPath;
        QFile::copy(filePath, targetPath);
    });

    //non serve collegare con menuWidgets essendo una stringa
    modifyLayout->addWidget(immagine);
    modifyLayout->addWidget(menuImmagine);
}

void LibraryManager::menuCarta(Cartaceo* carta){
    menuBiblio(static_cast<Biblioteca*>(carta));
    QLabel* autore = new QLabel(scrollContent);
    autore->setText("<div style = 'text-align: center;'>"
                    "<b>Autore</b></div>");
    autore->setStyleSheet("QLabel{"
                          "background-color: lightblue; "
                          "}");
    QLineEdit* menuAutore = new QLineEdit(scrollContent);
    menuAutore->setAlignment(Qt::AlignCenter);
    menuAutore->setStyleSheet("QLineEdit{"
                              "background-color: lightgray; "
                              "}");
    menuAutore->setText(carta ?
                            QString::fromStdString(carta->getAutore()) : "");
    menuWidgets["Autore"] = menuAutore;
    modifyLayout->addWidget(autore);
    modifyLayout->addWidget(menuAutore);

    QLabel* editore = new QLabel(scrollContent);
    editore->setText("<div style = 'text-align: center;'>"
                     "<b>Editore</b></div>");
    editore->setStyleSheet("QLabel{"
                           "background-color: lightblue; "
                           "}");
    QLineEdit* menuEditore = new QLineEdit(scrollContent);
    menuEditore->setAlignment(Qt::AlignCenter);
    menuEditore->setStyleSheet("QLineEdit{"
                               "background-color: lightgray; "
                               "}");
    menuEditore->setText(carta ?
                             QString::fromStdString(carta->getEditore()) : "");
    menuWidgets["Editore"] = menuEditore;
    modifyLayout->addWidget(editore);
    modifyLayout->addWidget(menuEditore);
}

void LibraryManager::menuMulti(Multimedia* multi){
    menuBiblio(static_cast<Biblioteca*>(multi));
    QLabel* durata = new QLabel(scrollContent);
    durata->setText("<div style = 'text-align: center;'>"
                    "<b>Durata</b></div>");
    durata->setStyleSheet("QLabel{"
                          "background-color: lightblue; "
                          "}");
    QSpinBox* menuDurata = new QSpinBox(scrollContent);
    menuDurata->setMaximum(INT_MAX);
    menuDurata->setMinimum(1);
    menuDurata->setAlignment(Qt::AlignCenter);
    menuDurata->setStyleSheet("QSpinBox{"
                              "background-color: lightgray;"
                              "}");
    menuDurata->setValue(multi ? multi->getDurata() : 60);
    menuWidgets["Durata"] = menuDurata;
    modifyLayout->addWidget(durata);
    modifyLayout->addWidget(menuDurata);

    QLabel* studio = new QLabel(scrollContent);
    studio->setText("<div style = 'text-align: center;'>"
                    "<b>Studio</b></div>");
    studio->setStyleSheet("QLabel{"
                          "background-color: lightblue; "
                          "}");
    QLineEdit* menuStudio = new QLineEdit(scrollContent);
    menuStudio->setAlignment(Qt::AlignCenter);
    menuStudio->setStyleSheet("QLineEdit{"
                              "background-color: lightgray; "
                              "}");
    menuStudio->setText(multi ?
                            QString::fromStdString(multi->getStudio()) : "");
    menuWidgets["Studio"] = menuStudio;
    modifyLayout->addWidget(studio);
    modifyLayout->addWidget(menuStudio);
}

void LibraryManager::menuRiviste(Riviste* rivista){
    menuCarta(static_cast<Cartaceo*>(rivista));
    QLabel* diffusione = new QLabel(scrollContent);
    if(rivista)
        diffusione->setText("<div style = 'text-align: center;'>"
                      "<b>Scegli la diffusione</b></div>");
    else
        diffusione->setText("<div style = 'text-align: center;'>"
                            "<b>Modifica la diffusione</b></div>");
    diffusione->setStyleSheet("QLabel{"
                            "background-color: lightblue; "
                            "}");
    QComboBox* menuDiffusione = new QComboBox(scrollContent);
    menuDiffusione->setEditable(true);
    menuDiffusione->lineEdit()->setAlignment(Qt::AlignCenter);
    menuDiffusione->addItems({"Provinciale","Regionale","Nazionale","Internazionale"});
    menuDiffusione->setStyleSheet("QComboBox{"
                                "background-color: lightgray; "
                                "}");
    if(rivista)
        menuDiffusione->setCurrentText(
            QString::fromStdString(rivista->diffusionToString()));
    else
        menuDiffusione->setCurrentText("Provinciale");
    menuWidgets["Diffusione"] = menuDiffusione;
    modifyLayout->addWidget(diffusione);
    modifyLayout->addWidget(menuDiffusione);
}

void LibraryManager::menuLibri(Libri* libro){
    menuCarta(static_cast<Cartaceo*>(libro));
    QLabel* linguaOriginale = new QLabel(scrollContent);
    linguaOriginale->setText("<div style = 'text-align: center;'>"
                             "<b>Lingua originale</b></div>");
    linguaOriginale->setStyleSheet("QLabel{"
                                   "background-color: lightblue; "
                                   "}");
    QLineEdit* menuLinguaOriginale = new QLineEdit(scrollContent);
    menuLinguaOriginale->setAlignment(Qt::AlignCenter);
    menuLinguaOriginale->setStyleSheet("QLineEdit{"
                                       "background-color: lightgray; "
                                       "}");
    menuLinguaOriginale->setText(libro ?
                                     QString::fromStdString(libro->getLanguage()) : "");
    menuWidgets["Lingua originale"] = menuLinguaOriginale;
    modifyLayout->addWidget(linguaOriginale);
    modifyLayout->addWidget(menuLinguaOriginale);

    QLabel* nvolumi = new QLabel(scrollContent);
    nvolumi->setText("<div style = 'text-align: center;'>"
                     "<b>Numero volumi</b></div>");
    nvolumi->setStyleSheet("QLabel{"
                           "background-color: lightblue;"
                           "}");
    QSpinBox* menuNvolumi = new QSpinBox(scrollContent);
    menuNvolumi->setMaximum(INT_MAX);
    menuNvolumi->setMinimum(1);
    menuNvolumi->setAlignment(Qt::AlignCenter);
    menuNvolumi->setStyleSheet("QSpinBox{"
                               "background-color: lightgray;"
                               "}");
    menuNvolumi->setValue(libro ? libro->getNvolumi() : 1);
    menuWidgets["Nvolumi"] = menuNvolumi;
    modifyLayout->addWidget(nvolumi);
    modifyLayout->addWidget(menuNvolumi);

}

void LibraryManager::menuManga(Manga* manga){
    menuLibri(static_cast<Libri*>(manga));
    QLabel* concluso = new QLabel(scrollContent);
    concluso->setText("<div style = 'text-align: center;'>"
                      "<b>Concluso?</b></div>");
    concluso->setStyleSheet("QLabel{"
                            "background-color: lightblue; "
                            "}");
    QComboBox* menuConcluso = new QComboBox(scrollContent);
    menuConcluso->setEditable(true);
    menuConcluso->lineEdit()->setAlignment(Qt::AlignCenter);
    menuConcluso->addItems({"Concluso", "Non concluso"});
    menuConcluso->setStyleSheet("QComboBox{"
                                "background-color: lightgray; "
                                "}");
    if(manga)
        menuConcluso->setCurrentText(manga->getConcluso() ? "Concluso" : "Non concluso");
    else
        menuConcluso->setCurrentText("Non concluso");
    menuWidgets["Concluso"] = menuConcluso;
    modifyLayout->addWidget(concluso);
    modifyLayout->addWidget(menuConcluso);
}

void LibraryManager::menuFilm(Film* film){
    menuMulti(static_cast<Multimedia*>(film));
    QLabel* regista = new QLabel(scrollContent);
    regista->setText("<div style = 'text-align: center;'>"
                     "<b>Regista</b></div>");
    regista->setStyleSheet("QLabel{"
                           "background-color: lightblue; "
                           "}");
    QLineEdit* menuRegista = new QLineEdit(scrollContent);
    menuRegista->setAlignment(Qt::AlignCenter);
    menuRegista->setStyleSheet("QLineEdit{"
                               "background-color: lightgray; "
                               "}");
    menuRegista->setText(film ?
                             QString::fromStdString(film->getRegista()) : "");
    menuWidgets["Regista"] = menuRegista;
    modifyLayout->addWidget(regista);
    modifyLayout->addWidget(menuRegista);

    QLabel* attoreProtagonista = new QLabel(scrollContent);
    attoreProtagonista->setText("<div style = 'text-align: center;'>"
                                "<b>Attore protagonista</b></div>");
    attoreProtagonista->setStyleSheet("QLabel{"
                                      "background-color: lightblue; "
                                      "}");
    QLineEdit* menuAttoreProtagonista = new QLineEdit(scrollContent);
    menuAttoreProtagonista->setAlignment(Qt::AlignCenter);
    menuAttoreProtagonista->setStyleSheet("QLineEdit{"
                                          "background-color: lightgray; "
                                          "}");
    menuAttoreProtagonista->setText(film ?
                                        QString::fromStdString(film->getAttore()) : "");
    menuWidgets["Attore protagonista"] = menuAttoreProtagonista;
    modifyLayout->addWidget(attoreProtagonista);
    modifyLayout->addWidget(menuAttoreProtagonista);
}

void LibraryManager::menuCd(Cd* cd){
    menuMulti(static_cast<Multimedia*>(cd));
    QLabel* artista = new QLabel(scrollContent);
    artista->setText("<div style = 'text-align: center;'>"
                     "<b>Artista</b></div>");
    artista->setStyleSheet("QLabel{"
                           "background-color: lightblue; "
                           "}");
    QLineEdit* menuArtista = new QLineEdit(scrollContent);
    menuArtista->setAlignment(Qt::AlignCenter);
    menuArtista->setStyleSheet("QLineEdit{"
                               "background-color: lightgray; "
                               "}");
    menuArtista->setText(cd ?
                             QString::fromStdString(cd->getArtista()) : "");
    menuWidgets["Artista"] = menuArtista;
    modifyLayout->addWidget(artista);
    modifyLayout->addWidget(menuArtista);

    QLabel* ntracce = new QLabel(scrollContent);
    ntracce->setText("<div style = 'text-align: center;'>"
                     "<b>Numero tracce</b></div>");
    ntracce->setStyleSheet("QLabel{"
                           "background-color: lightblue;"
                           "}");
    QSpinBox* menuNtracce = new QSpinBox(scrollContent);
    menuNtracce->setMaximum(INT_MAX);
    menuNtracce->setMinimum(1);
    menuNtracce->setAlignment(Qt::AlignCenter);
    menuNtracce->setStyleSheet("QSpinBox{"
                               "background-color: lightgray;"
                               "}");
    menuNtracce->setValue(cd ? cd->getTracce() : 10);
    menuWidgets["Ntracce"] = menuNtracce;
    modifyLayout->addWidget(ntracce);
    modifyLayout->addWidget(menuNtracce);
}

void LibraryManager::save(Biblioteca* biblio){
    QString titolo =
        (qobject_cast<QLineEdit*>(menuWidgets["Titolo"]))->text();
    biblio->setTitolo(titolo.toStdString());
    QString genere =
        (qobject_cast<QLineEdit*>(menuWidgets["Genere"]))->text();
    biblio->setGenere(genere.toStdString());
    int anno =
        (qobject_cast<QSpinBox*>(menuWidgets["Anno"]))->value();
    biblio->setAnno(anno);
    double costo =
        (qobject_cast<QDoubleSpinBox*>(menuWidgets["Costo"]))->value();
    biblio->setCosto(costo);
    int prestiti =
        (qobject_cast<QSpinBox*>(menuWidgets["Prestiti"]))->value();
    biblio->setNprestiti(prestiti);
    if(!image.isEmpty())
        biblio->setImmagine(image.toStdString());
}

void LibraryManager::save(Cartaceo* carta){
    save(static_cast<Biblioteca*>(carta));
    QString autore =
        (qobject_cast<QLineEdit*>(menuWidgets["Autore"]))->text();
    carta->setAutore(autore.toStdString());
    QString editore =
        (qobject_cast<QLineEdit*>(menuWidgets["Editore"]))->text();
    carta->setEditore(editore.toStdString());
}

void LibraryManager::save(Multimedia* multi){
    save(static_cast<Biblioteca*>(multi));
    int durata =
        (qobject_cast<QSpinBox*>(menuWidgets["Durata"]))->value();
    multi->setDurata(durata);
    QString studio =
        (qobject_cast<QLineEdit*>(menuWidgets["Studio"]))->text();
    multi->setStudio(studio.toStdString());
}

void LibraryManager::save(Riviste* rivista){
    save(static_cast<Cartaceo*>(rivista));
    QString diffusione =
        (qobject_cast<QComboBox*>(menuWidgets["Diffusione"]))->currentText();
    Riviste::Diffusione diff;
    if (diffusione == "Provinciale")
        diff = Riviste::Provinciale;
    else if (diffusione == "Regionale")
        diff = Riviste::Regionale;
    else if (diffusione == "Nazionale")
        diff = Riviste::Nazionale;
    else if (diffusione == "Internazionale")
        diff = Riviste::Internazionale;
    rivista->setDiffusione(diff);
}

void LibraryManager::save(Libri* libro){
    save(static_cast<Cartaceo*>(libro));
    QString linguaOriginale =
        (qobject_cast<QLineEdit*>(menuWidgets["Lingua originale"]))->text();
    libro->setLanguage(linguaOriginale.toStdString());
    int nvolumi =
        (qobject_cast<QSpinBox*>(menuWidgets["Nvolumi"]))->value();
    libro->setNvolumi(nvolumi);
}

void LibraryManager::save(Manga* manga){
    save(static_cast<Libri*>(manga));
    QString concluso =
        (qobject_cast<QComboBox*>(menuWidgets["Concluso"]))->currentText();
    bool conc = (concluso == "Concluso");
    manga->setConcluso(conc);
}

void LibraryManager::save(Film* film){
    save(static_cast<Multimedia*>(film));
    QString regista =
        (qobject_cast<QLineEdit*>(menuWidgets["Regista"]))->text();
    film->setRegista(regista.toStdString());
    QString attoreProtagonista =
        (qobject_cast<QLineEdit*>(menuWidgets["Attore protagonista"]))->text();
    film->setAttore(attoreProtagonista.toStdString());
}

void LibraryManager::save(Cd* cd){
    save(static_cast<Multimedia*>(cd));
    QString artista =
        (qobject_cast<QLineEdit*>(menuWidgets["Artista"]))->text();
    cd->setArtista(artista.toStdString());
    int ntracce =
        (qobject_cast<QSpinBox*>(menuWidgets["Ntracce"]))->value();
    cd->setTracce(ntracce);
}

LibraryManager::BibliotecaInfo LibraryManager::createBiblio(){
    LibraryManager::BibliotecaInfo biblio;
    biblio.titolo = (qobject_cast<QLineEdit*>(menuWidgets["Titolo"]))->text();
    biblio.genere = (qobject_cast<QLineEdit*>(menuWidgets["Genere"]))->text();
    biblio.anno = (qobject_cast<QSpinBox*>(menuWidgets["Anno"]))->value();
    biblio.costo = (qobject_cast<QDoubleSpinBox*>(menuWidgets["Costo"]))->value();
    QString disp = (qobject_cast<QComboBox*>(menuWidgets["Disponibile"]))->currentText();
    biblio.disponibile = (disp=="Disponibile"? true : false);
    biblio.copie = (qobject_cast<QSpinBox*>(menuWidgets["Anno"]))->value();
    biblio.nprestiti = (qobject_cast<QSpinBox*>(menuWidgets["Anno"]))->value();
    return biblio;
}

LibraryManager::CartaceoInfo LibraryManager::createCartaceo(){}

LibraryManager::MultimediaInfo LibraryManager::createMultimedia(){}

Riviste* LibraryManager::createRiviste(){}

Libri* LibraryManager::createLibri(){}

Manga* LibraryManager::createManga(){}

Cd* LibraryManager::createCd(){}

Film* LibraryManager::createFilm(){}