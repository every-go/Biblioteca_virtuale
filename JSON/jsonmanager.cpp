#include "jsonmanager.h"
#include "../interfaccia_grafica/adminarea.h"
#include "../interfaccia_grafica/userarea.h"
#include <QStringList>
#include <QMap>
#include <QInputDialog>
#include <QFile>
#include <QPushButton>
#include <QJsonArray>
#include <QByteArray>
#include <QList>
#include <QJsonDocument>
#include <QDebug>
#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>

// Ho incluso solo gli header necessari per evitare dipendenze ridondanti,
// poiché le classi come Cd, Film, Manga e Riviste includono già le dipendenze comuni
// come Multimedia, Biblioteca e Libri.

#include "../modello_logico/manga.h"
#include "../modello_logico/riviste.h"
#include "../modello_logico/cd.h"
#include "../modello_logico/film.h"

JsonManager::JsonManager(QWidget *parent) : QWidget(parent) {}

void JsonManager::setOggetti(const QList<Biblioteca*>& newBiblioteca){
    biblioteca = newBiblioteca;
}

void JsonManager::addObserver(JsonObserver* observer){
    observers.append(observer);
}

void JsonManager::notifyObservers(const QList<Biblioteca *> &newBiblioteca){
    //dato che so di avere solo due observer admin e user posso
    //chiamare la funzione senza fare i dynamic_cast
    for(JsonObserver* observer : observers){
        observer->onBibliotecaUpdated(newBiblioteca);
    }
}

QList<Biblioteca*> JsonManager::loadBibliotecaListFromJson(const QString& filename) {
    QList<Biblioteca*> listaBiblioteche;
    // Carica il file JSON
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)){
        qWarning() << "File non aperto";
        return listaBiblioteche;
    }
    // Leggi il contenuto del file
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    // Verifica se il documento è valido
    if (doc.isNull() || !doc.isArray()){
        qWarning() << "File non valido";
        return listaBiblioteche;
    }
    // Ottieni l'array di oggetti JSON
    QJsonArray array = doc.array();
    // Per ogni oggetto JSON nell'array, creiamo un oggetto Biblioteca
    for (const QJsonValue& value : array) {
        if (value.isObject()) {
            QJsonObject obj = value.toObject();
            QString classe = obj["Classe"].toString();
            Biblioteca* biblio = nullptr;
            if(classe== "Riviste")
                biblio = JsonManager::loadRiviste(obj);
            else if(classe=="Libri")
                biblio = JsonManager::loadLibri(obj);
            else if(classe=="Cd")
                biblio = JsonManager::loadCd(obj);
            else if(classe=="Film")
                biblio = JsonManager::loadFilm(obj);
            else if(classe=="Manga")
                biblio = JsonManager::loadManga(obj);
            if(biblio)
                listaBiblioteche.append(biblio);
            else
                qWarning()<<"Classe sconosciuta"<<classe;
        }
    }
    setOggetti(listaBiblioteche);
    return listaBiblioteche;
}

JsonManager::BibliotecaBase JsonManager::loadBiblioteca(const QJsonObject& obj) {
    JsonManager::BibliotecaBase base;
    // Caricamento dei dati dal QJsonObject
    base.titolo = obj["Titolo"].toString();
    base.genere = obj["Genere"].toString();
    base.anno = obj["Anno"].toInt();
    base.costo = obj["Costo"].toDouble();
    base.disponibile = obj["Disponibile"].toBool();
    base.copie = obj["Copie disponibili"].toInt();
    base.nprestiti = obj["Nprestiti"].toInt();
    base.immagine = obj["Immagine"].toString();
    return base;
}

JsonManager::CartaceoBase JsonManager::loadCartaceo(const QJsonObject& obj){
    JsonManager::CartaceoBase carta;
    carta.autore = obj["Autore"].toString();
    carta.editore = obj["Editore"].toString();
    carta.letto = obj["Letto"].toBool();
    return carta;
}

JsonManager::MultimediaBase JsonManager::loadMultimedia(const QJsonObject& obj){
    JsonManager::MultimediaBase multi;
    multi.durata = obj["Durata"].toInt();
    multi.studio = obj["Studio"].toString();
    return multi;
}

Biblioteca* JsonManager::loadRiviste(const QJsonObject& obj){
    JsonManager::BibliotecaBase base = loadBiblioteca(obj);
    JsonManager::CartaceoBase cartbase = loadCartaceo(obj);
    QString diffusione = obj["Diffusione"].toString();
    Riviste::Diffusione diff;
    if (diffusione == "Provinciale")
        diff = Riviste::Provinciale;
    else if (diffusione == "Regionale")
        diff = Riviste::Regionale;
    else if (diffusione == "Nazionale")
        diff = Riviste::Nazionale;
    else if (diffusione == "Internazionale")
        diff = Riviste::Internazionale;
    else{
        //valore default
        diff= Riviste::Provinciale;
        qWarning() << "Nessun tipo di diffusione presente";
    }
    return new Riviste(base.titolo.toStdString(), base.genere.toStdString(), base.anno, base.costo, base.disponibile,
                       base.copie, base.nprestiti, base.immagine.toStdString(),
                       cartbase.autore.toStdString(), cartbase.editore.toStdString(), cartbase.letto, diff);
}

Biblioteca* JsonManager::loadLibri(const QJsonObject& obj){
    JsonManager::BibliotecaBase base = loadBiblioteca(obj);
    JsonManager::CartaceoBase cartbase = loadCartaceo(obj);
    QString language = obj["Lingua originale"].toString();
    int number = obj["Nvolumi"].toInt();
    return new Libri(base.titolo.toStdString(), base.genere.toStdString(), base.anno, base.costo, base.disponibile,
                     base.copie, base.nprestiti, base.immagine.toStdString(),
                     cartbase.autore.toStdString(), cartbase.editore.toStdString(), cartbase.letto, language.toStdString(), number);
}

Biblioteca* JsonManager::loadManga(const QJsonObject& obj){
    JsonManager::BibliotecaBase base = loadBiblioteca(obj);
    JsonManager::CartaceoBase cartbase = loadCartaceo(obj);
    QString language = obj["Lingua originale"].toString();
    int number = obj["Nvolumi"].toInt();
    bool concluso = obj["Concluso"].toBool();
    return new Manga(base.titolo.toStdString(), base.genere.toStdString(), base.anno, base.costo, base.disponibile,
                     base.copie, base.nprestiti, base.immagine.toStdString(),
                     cartbase.autore.toStdString(), cartbase.editore.toStdString(), cartbase.letto, language.toStdString(), number, concluso);
}

Biblioteca* JsonManager::loadCd(const QJsonObject& obj){
    JsonManager::BibliotecaBase base = loadBiblioteca(obj);
    JsonManager::MultimediaBase multbase = loadMultimedia(obj);
    QString artista = obj["Artista"].toString();
    int ntracce = obj["Ntracce"].toInt();
    bool ascoltato = obj["Ascoltato"].toBool();
    return new Cd(base.titolo.toStdString(), base.genere.toStdString(), base.anno, base.costo, base.disponibile,
                  base.copie, base.nprestiti, base.immagine.toStdString(),
                  multbase.durata, multbase.studio.toStdString(), artista.toStdString(), ntracce, ascoltato);
}

Biblioteca* JsonManager::loadFilm(const QJsonObject& obj){
    JsonManager::BibliotecaBase base = loadBiblioteca(obj);
    JsonManager::MultimediaBase multbase = loadMultimedia(obj);
    QString director = obj["Regista"].toString();
    QString actor = obj["Attore protagonista"].toString();
    bool listened = obj["Visto"].toBool();
    return new Film(base.titolo.toStdString(), base.genere.toStdString(), base.anno, base.costo, base.disponibile,
                    base.copie, base.nprestiti, base.immagine.toStdString(),
                    multbase.durata, multbase.studio.toStdString(), director.toStdString(), actor.toStdString(), listened);
}

//necessario per centrare i vari dialog durante la gestione degli oggetti
QWidget* JsonManager::getAdminArea(){
    QWidget* parent = nullptr;
    for (JsonObserver* observer : observers) {
        AdminArea* adminArea = dynamic_cast<AdminArea*>(observer);
        if (adminArea) {
            parent = adminArea;
            break;
        }
    }
    return parent;
}

void JsonManager::handleObjectCreation(QString selectedClass) {
    QWidget* parent = getAdminArea();
    Biblioteca* newObject = nullptr;
    if (selectedClass == "Riviste")
        newObject = createRiviste();
    else if (selectedClass == "Libri")
        newObject = createLibri();
    else if (selectedClass == "Manga")
        newObject = createManga();
    else if (selectedClass == "Cd")
        newObject = createCd();
    else if (selectedClass == "Film")
        newObject = createFilm();
    //serve perché se l'utente ha annullato l'operazione
    //restituisco nullptr
    if (newObject) {
        biblioteca.append(newObject);
        notifyObservers(biblioteca);
        savenewObject(newObject);
    }
    else
        QMessageBox::warning(parent, "Annullamento", "Hai annullato l'operazione");
}

std::pair<JsonManager::BibliotecaBase, bool> JsonManager::createBiblioteca() {
    QWidget* parent = getAdminArea();
    JsonManager::BibliotecaBase biblio;
    std::pair<QString, bool> title = getValidTextInput(parent, "Titolo", "Inserisci il titolo");
    if(title.second==false)
        return{{}, false};
    std::pair<QString, bool> genre = getValidTextInput(parent, "Genere", "Inserisci il genere");
    if(genre.second == false)
        return{{}, false};
    std::pair<int, bool> year = getValidIntInput(parent, "Anno", "Inserisci l'anno di creazione");
    if(year.second == false)
        return{{}, false};
    std::pair<double, bool> price = getValidDoubleInput(parent, "Costo", "Inserisci il costo", 4.99, 0.01);
    if(price.second == false)
        return{{}, false};
    std::pair<bool, bool> disponibile = getBooleanInputFromList(parent, "Disponibile", "Inserisci la disponibilità");
    if(disponibile.second == false)
        return{{}, false};
    std::pair<int, bool> copies;
    if(disponibile.first == true)
        copies = getValidIntInput(parent, "Copie", "Inserisci le copie disponibili", 1, 1);
    else
        copies = {0, true};
    if(copies.second == false)
        return{{}, false};
    std::pair<int, bool> nprest = getValidIntInput(parent, "Numero prestiti", "Inserisci il numero di prestiti possibili", copies.first>0 ? copies.first : 1, copies.first>0 ? copies.first : 1);
    if(nprest.second == false)
        return{{}, false};
    std::pair<QString, bool> image = getImageInput(parent, "Immagine", "Inserisci l'immagine");
    if(image.second == false)
        return{{}, false};
    biblio.titolo = title.first;
    biblio.genere = genre.first;
    biblio.anno = year.first;
    biblio.costo = price.first;
    biblio.disponibile = disponibile.first;
    biblio.copie = copies.first;
    biblio.nprestiti = nprest.first>=biblio.copie?
                           nprest.first : biblio.copie;
    biblio.immagine = image.first;
    return {biblio, true};
}

std::pair<JsonManager::MultimediaBase,bool> JsonManager::createMultimedia(){
    QWidget* parent = getAdminArea();
    JsonManager::MultimediaBase multi;
    std::pair<int, bool> time = getValidIntInput(parent, "Durata", "Inserisci la durata", 1, 1);
    if(time.second == false)
        return{{}, false};
    std::pair<QString, bool> studios = getValidTextInput(parent, "Studio", "Inserisci lo studio di produzione");
    if(studios.second == false)
        return{{}, false};
    multi.durata = time.first;
    multi.studio = studios.first;
    return {multi, true};
}

std::pair<JsonManager::CartaceoBase, bool> JsonManager::createCartaceo(){
    QWidget* parent = getAdminArea();
    JsonManager::CartaceoBase carta;
    std::pair<QString, bool> author = getValidTextInput(parent, "Autore", "Inserisci l'autore");
    if(author.second == false)
        return{{}, false};
    std::pair<QString, bool> editor = getValidTextInput(parent, "Editore", "Inserisci l'editore");
    if (editor.second==false || editor.second==false)
        return {{}, false};
    carta.autore = author.first;
    carta.editore = editor.first;
    return {carta, true};
}

Biblioteca* JsonManager::createRiviste(){
    QWidget* parent = getAdminArea();
    std::pair<JsonManager::BibliotecaBase, bool> biblio = createBiblioteca();
    if(biblio.second == false)
        return nullptr;
    std::pair<JsonManager::CartaceoBase, bool> carta = createCartaceo();
    if(carta.second == false)
        return nullptr;
    QStringList choices{"Provinciale", "Regionale", "Nazionale", "Internazionale"};
    QString choice;
    bool ok;
    do {
        choice = QInputDialog::getItem(parent, "Diffusione", "Inserisci la diffusione della rivista", choices, 0, false, &ok, Qt::Dialog);
        if(!ok)
            return nullptr;
        if (choice.isEmpty()) {
            QMessageBox::warning(parent, "Input non valido", "Devi selezionare una delle opzioni");
        }
    }
    while (choice.isEmpty());
    Riviste::Diffusione diff;
    if (choice == "Provinciale")
        diff = Riviste::Provinciale;
    else if (choice == "Regionale")
        diff = Riviste::Regionale;
    else if (choice == "Nazionale")
        diff = Riviste::Nazionale;
    else if (choice == "Internazionale")
        diff = Riviste::Internazionale;
    else
        diff = Riviste::Provinciale;
    return new Riviste(biblio.first.titolo.toStdString(), biblio.first.genere.toStdString(),
                       biblio.first.anno, biblio.first.costo, biblio.first.disponibile, biblio.first.copie,
                       biblio.first.nprestiti, biblio.first.immagine.toStdString(), carta.first.autore.toStdString(),
                       carta.first.editore.toStdString(), false, diff);
}

Biblioteca* JsonManager::createLibri(){
    QWidget* parent = getAdminArea();
    std::pair<JsonManager::BibliotecaBase, bool> biblio = createBiblioteca();
    if(biblio.second == false)
        return nullptr;
    std::pair<JsonManager::CartaceoBase, bool> carta = createCartaceo();
    if(carta.second == false)
        return nullptr;
    std::pair<QString, bool> language = getValidTextInput(parent, "Lingua originale", "Inserisci la lingua originale");
    if(language.second == false)
        return nullptr;
    std::pair<int, bool> number = getValidIntInput(parent, "Numero volumi", "Inserisci il numero di volumi", 1, 1);
    if(number.second == false)
        return nullptr;
    return new Libri(biblio.first.titolo.toStdString(), biblio.first.genere.toStdString(),
                     biblio.first.anno, biblio.first.costo, biblio.first.disponibile, biblio.first.copie,
                     biblio.first.nprestiti, biblio.first.immagine.toStdString(), carta.first.autore.toStdString(),
                     carta.first.editore.toStdString(), false, language.first.toStdString(), number.first);
}

Biblioteca* JsonManager::createManga(){
    QWidget* parent = getAdminArea();
    std::pair<JsonManager::BibliotecaBase, bool> biblio = createBiblioteca();
    if(biblio.second == false)
        return nullptr;
    std::pair<JsonManager::CartaceoBase, bool> carta = createCartaceo();
    if(carta.second == false)
        return nullptr;
    std::pair<QString, bool> language = getValidTextInput(parent, "Lingua originale", "Inserisci la lingua originale");
    if(language.second == false)
        return nullptr;
    std::pair<int, bool> number = getValidIntInput(parent, "Numero volumi", "Inserisci il numero di volumi", 1, 1);
    if(number.second == false)
        return nullptr;
    std::pair<bool, bool> finish = getBooleanInputFromList(parent, "Concluso", "Il manga è concluso?");
    if(finish.second == false)
        return nullptr;
    return new Manga(biblio.first.titolo.toStdString(), biblio.first.genere.toStdString(),
                     biblio.first.anno, biblio.first.costo, biblio.first.disponibile, biblio.first.copie,
                     biblio.first.nprestiti, biblio.first.immagine.toStdString(), carta.first.autore.toStdString(),
                     carta.first.editore.toStdString(), false, language.first.toStdString(), number.first, finish.first);
}

Biblioteca* JsonManager::createCd(){
    QWidget* parent = getAdminArea();
    std::pair<JsonManager::BibliotecaBase, bool> biblio = createBiblioteca();
    if(biblio.second == false)
        return nullptr;
    std::pair<JsonManager::MultimediaBase, bool> multi = createMultimedia();
    if(multi.second == false)
        return nullptr;
    std::pair<QString, bool> artist = getValidTextInput(parent, "Artista", "Inserisci l'artista");
    if(artist.second == false)
        return nullptr;
    std::pair<int, bool> tracks = getValidIntInput(parent, "Numero tracce", "Inserisci il numero di tracce", 1, 1);
    if(tracks.second == false)
        return nullptr;
    return new Cd(biblio.first.titolo.toStdString(), biblio.first.genere.toStdString(),
                  biblio.first.anno, biblio.first.costo, biblio.first.disponibile, biblio.first.copie,
                  biblio.first.nprestiti, biblio.first.immagine.toStdString(), multi.first.durata,
                  multi.first.studio.toStdString(), artist.first.toStdString(), tracks.first, false);
}

Biblioteca* JsonManager::createFilm(){
    QWidget* parent = getAdminArea();
    std::pair<JsonManager::BibliotecaBase, bool> biblio = createBiblioteca();
    if(biblio.second == false)
        return nullptr;
    std::pair<JsonManager::MultimediaBase, bool> multi = createMultimedia();
    if(multi.second == false)
        return nullptr;
    std::pair<QString, bool> director = getValidTextInput(parent, "Regista", "Inserisci il regista");
    if(director.second == false)
        return nullptr;
    std::pair<QString, bool> actor = getValidTextInput(parent, "Attore", "Inserisci l'attore protagonista");
    if(actor.second == false)
        return nullptr;
    return new Film(biblio.first.titolo.toStdString(), biblio.first.genere.toStdString(),
                    biblio.first.anno, biblio.first.costo, biblio.first.disponibile, biblio.first.copie,
                    biblio.first.nprestiti, biblio.first.immagine.toStdString(), multi.first.durata,
                    multi.first.studio.toStdString(), director.first.toStdString(), actor.first.toStdString(), false);
}

std::pair<QString, bool> JsonManager::getValidTextInput(QWidget* parent, const QString& title, const QString& label, const QString& defaultText) {
    QString input;
    bool ok;
    do {
        input = QInputDialog::getText(parent, title, label, QLineEdit::Normal, defaultText, &ok, Qt::Dialog);
        if (!ok)
            return {"", false};
        if (input.isEmpty())
            QMessageBox::warning(parent, "Input non valido", "Devi inserire un valore valido.");
    }
    while (input.isEmpty());
    return {input, true};
}

std::pair<int, bool> JsonManager::getValidIntInput(QWidget* parent, const QString& title, const QString& label, int defaultValue, int min, int max, int step) {
    int input;
    bool ok;
    do {
        input = QInputDialog::getInt(parent, title, label, defaultValue, min, max, step, &ok, Qt::Dialog);
        if(!ok)
            return {0, false};
        if (input<min || input>max)
            QMessageBox::warning(parent, "Input non valido", "Devi inserire un valore valido");
    } while (input<min || input>max);
    return {input, true};
}

std::pair<double, bool> JsonManager::getValidDoubleInput(QWidget* parent, const QString& title, const QString& label, double defaultValue, double min, double max, double decimals) {
    double input;
    bool ok;
    do {
        input = QInputDialog::getDouble(parent, title, label, defaultValue, min, max, decimals, &ok, Qt::Dialog);
        if (!ok)
            return{0.0, false};
        if(input<min || input>max)
            QMessageBox::warning(parent, "Input non valido", "Devi inserire un valore valido");
    } while (input<min || input>max);
    return {input, true};
}

std::pair<bool, bool> JsonManager::getBooleanInputFromList(QWidget* parent, const QString& title, const QString& label) {
    QStringList options{"Sì","No"};
    QString choice;
    bool ok;
    do {
        choice = QInputDialog::getItem(parent, title, label, options, 0, false, &ok, Qt::Dialog);
        if(!ok)
            return {false, false};
        if (choice.isEmpty())
            QMessageBox::warning(parent, "Input non valido", "Devi selezionare una delle opzioni");
    }
    while (choice.isEmpty());

    return {(choice == "Sì"), true};
}

std::pair<QString, bool> JsonManager::getImageInput(QWidget* parent, const QString& title, const QString& label) {
    QString filePath = QFileDialog::getOpenFileName(parent, title, label, "Immagini (*.png *.jpg *.jpeg *.bmp *.gif)");
    if (filePath.isEmpty())
        return {"", false};
    QDir imgDir("IMG");
    if (!imgDir.exists())
        imgDir.mkpath(".");
    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();
    QString newFilePath = "IMG/" + fileName;
    QFile::copy(filePath, newFilePath);
    return {newFilePath, true};
}

void JsonManager::deleteObject(Biblioteca* biblio){
    int occurenceToSkip = 0;
    for(int i = 0; i < biblioteca.size(); i++){
        if(biblioteca[i] == biblio){
            biblioteca.removeAt(i);
            notifyObservers(biblioteca);
            break;
        }
        if(biblioteca[i]->getTitolo() == biblio->getTitolo())
            occurenceToSkip++;
    }
    QFile file("dati.json");
    if (!file.open(QIODevice::ReadOnly)){
        qWarning() << "File non aperto";
    }
    // Leggi il contenuto del file
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    // Verifica se il documento è valido
    if (doc.isNull() || !doc.isArray()){
        qWarning() << "File non valido";
        return;
    }
    QJsonArray array = doc.array();
    QJsonArray updatedArray;
    int occurences = 0;
    for (int i = 0; i < array.size(); i++) {
        QJsonObject obj = array[i].toObject();
        if (obj["Titolo"].toString() == QString::fromStdString(biblio->getTitolo())) {
            if (occurences == occurenceToSkip) {
                // Salta l'occorrenza che deve essere eliminata
                occurences++;
                continue;
            }
            occurences++;
        }
        // Aggiungi gli altri oggetti
        updatedArray.append(obj);
    }
    file.close();
    if(file.open(QIODevice::WriteOnly)){
        QJsonDocument updatedDoc(updatedArray);
        file.write(updatedDoc.toJson());
        file.close();
    }
    else
        qWarning() << "File non apribile in scrittura";
    delete biblio;
}

void JsonManager::modifyObject(Biblioteca* biblio){
    QWidget* parent = getAdminArea();
    QDialog* dialog = new QDialog(parent);
    dialog->setWindowTitle(QString::fromStdString(biblio->getTitolo()));
    dialog->setWindowFlags(dialog->windowFlags() | Qt::WindowStaysOnTopHint);
    dialog->resize(300, 300);
    QGridLayout* dialogLayout = new QGridLayout(dialog);
    QPushButton* annulla = new QPushButton("Annulla", dialog);
    dialogLayout->addWidget(annulla);
    connect(annulla, &QPushButton::clicked, this, [dialog](){
        dialog->reject();
    });
    if(typeid(*biblio)==typeid(Manga))
        mostraMenu(static_cast<Manga*>(biblio), dialog, dialogLayout, parent);
    if(typeid(*biblio)==typeid(Libri))
        mostraMenu(static_cast<Libri*>(biblio), dialog, dialogLayout, parent);
    if(typeid(*biblio)==typeid(Riviste))
        mostraMenu(static_cast<Riviste*>(biblio), dialog, dialogLayout, parent);
    if(typeid(*biblio)==typeid(Film))
        mostraMenu(static_cast<Film*>(biblio), dialog, dialogLayout, parent);
    if(typeid(*biblio)==typeid(Cd))
        mostraMenu(static_cast<Cd*>(biblio), dialog, dialogLayout, parent);
    dialog->exec();
}

void JsonManager::mostraMenu(Biblioteca* biblio, QDialog* dialog, QGridLayout* dialogLayout, QWidget* parent){
    QPushButton* titolo = new QPushButton("Modifica titolo", dialog);
    dialogLayout->addWidget(titolo, 0, 0, 1, 1);
    QPushButton* genere = new QPushButton("Modifica genere", dialog);
    dialogLayout->addWidget(genere, 0, 1, 1, 1);
    QPushButton* anno = new QPushButton("Modifica anno", dialog);
    dialogLayout->addWidget(anno, 0, 2, 1, 1);
    QPushButton* costo = new QPushButton("Modifica costo", dialog);
    dialogLayout->addWidget(costo, 1, 0, 1, 1);
    QPushButton* nprestiti = new QPushButton("Modifica numero\nprestiti", dialog);
    dialogLayout->addWidget(nprestiti, 1, 1, 1, 1);
    QPushButton* image = new QPushButton("Modifica immagine", dialog);
    dialogLayout->addWidget(image, 1, 2, 1, 1);
    connect(titolo, &QPushButton::clicked, this, [this, parent, biblio, dialog](){
        std::pair<QString, bool> newtitle = getValidTextInput(parent,
                                                              "Nuovo titolo", "Inserisci il nuovo titolo");
        if(newtitle.second == false){
            QMessageBox::warning(parent, "Annullamento", "Hai annullato l'operazione");
            return;
        }
        biblio->setTitolo(newtitle.first.toStdString());
        updateObject(biblio);
        dialog->reject();
    });
    connect(genere, &QPushButton::clicked, this, [this, parent, biblio, dialog](){
        std::pair<QString, bool> newgenre = getValidTextInput(parent, "Nuovo genere",
                                                              "Inserisci il nuovo genere");
        if(newgenre.second == false){
            QMessageBox::warning(parent, "Annullamento", "Hai annullato l'operazione");
            return;
        }
        biblio->setGenere(newgenre.first.toStdString());
        updateObject(biblio);
        dialog->reject();
    });
    connect(anno, &QPushButton::clicked, this, [this, parent, biblio, dialog](){
        std::pair<int, bool> newyear = getValidIntInput(parent, "Nuovo anno",
                                                            "Inserisci il nuovo anno");
        if(newyear.second == false){
            QMessageBox::warning(parent, "Annullamento", "Hai annullato l'operazione");
            return;
        }
        biblio->setAnno(newyear.first);
        updateObject(biblio);
        dialog->reject();

    });
    connect(costo, &QPushButton::clicked, this, [this, parent, biblio, dialog](){
        std::pair<double, bool> newcost = getValidDoubleInput(parent, "Nuovo costo",
                                                               "Inserisci il nuovo costo", 0.01, 0.01);
        if(newcost.second == false){
            QMessageBox::warning(parent, "Annullamento", "Hai annullato l'operazione");
            return;
        }
        biblio->setCosto(newcost.first);
        updateObject(biblio);
        dialog->reject();
    });
    connect(nprestiti, &QPushButton::clicked, this, [this, parent, biblio, dialog](){
        std::pair<int, bool> newnprest = getValidIntInput(parent, "Nuovo numero prestiti",
                                                              "Inserisci il nuovo numero di prestiti effettuabili", biblio->getCopie(), biblio->getCopie());
        if(newnprest.second == false){
            QMessageBox::warning(parent, "Annullamento", "Hai annullato l'operazione");
            return;
        }
        biblio->setNprestiti(newnprest.first);
        updateObject(biblio);
        dialog->reject();
    });
    connect(image, &QPushButton::clicked, this, [this, parent, biblio, dialog](){
        std::pair<QString, bool> newimage = getImageInput(parent, "Immagine",
                                                          "Inserisci la nuova immagine");
        if(newimage.second == false){
            QMessageBox::warning(parent, "Annullamento", "Hai annullato l'operazione");
            return;
        }
        biblio->setImmagine(newimage.first.toStdString());
        updateObject(biblio);
        dialog->reject();
    });
}

void JsonManager::mostraMenu(Cartaceo* carta, QDialog* dialog, QGridLayout* dialogLayout, QWidget* parent){
    mostraMenu(static_cast<Biblioteca*>(carta), dialog, dialogLayout, parent);
    QPushButton* autore = new QPushButton("Modifica autore", dialog);
    dialogLayout->addWidget(autore, 2, 0, 1, 1);
    QPushButton* editore = new QPushButton("Modifica editore", dialog);
    dialogLayout->addWidget(editore, 2, 1, 1, 1);
    connect(autore, &QPushButton::clicked, this, [this, parent, carta, dialog](){
        std::pair<QString, bool> newautore = getValidTextInput(parent, "Nuovo autore",
                                                               "Inserisci il nuovo autore");
        if(newautore.second == false){
            QMessageBox::warning(parent, "Annullamento", "Hai annullato l'operazione");
            return;
        }
        carta->setAutore(newautore.first.toStdString());
        updateObject(carta);
        dialog->reject();
    });
    connect(editore, &QPushButton::clicked, this, [this, parent, carta, dialog](){
        std::pair<QString, bool> neweditore = getValidTextInput(parent, "Nuovo editore",
                                                                "Inserisci il nuovo editore");
        if(neweditore.second == false){
            QMessageBox::warning(parent, "Annullamento", "Hai annullato l'operazione");
            return;
        }
        carta->setEditore(neweditore.first.toStdString());
        updateObject(carta);
        dialog->reject();
    });
}

void JsonManager::mostraMenu(Multimedia* multi, QDialog* dialog, QGridLayout* dialogLayout, QWidget* parent){
    mostraMenu(static_cast<Biblioteca*>(multi), dialog, dialogLayout, parent);
    QPushButton* durata = new QPushButton("Modifica la durata", dialog);
    dialogLayout->addWidget(durata, 2, 0, 1, 1);
    QPushButton* studio = new QPushButton("Modifica studio", dialog);
    dialogLayout->addWidget(studio, 2, 1, 1, 1);
    connect(durata, &QPushButton::clicked, this, [this, parent, multi, dialog](){
        std::pair<int, bool> newdurata = getValidIntInput(parent, "Durata",
                                                              "Inserisci la nuova durata", 1, 1);
        if(newdurata.second == false){
            QMessageBox::warning(parent, "Annullamento", "Hai annullato l'operazione");
            return;
        }
        multi->setDurata(newdurata.first);
        updateObject(multi);
        dialog->reject();
    });
    connect(studio, &QPushButton::clicked, this, [this, parent, multi, dialog](){
        std::pair<QString, bool> newstudio = getValidTextInput(parent, "Nuovo studio",
                                                                "Inserisci il nuovo studio");
        if(newstudio.second == false){
            QMessageBox::warning(parent, "Annullamento", "Hai annullato l'operazione");
            return;
        }
        multi->setStudio(newstudio.first.toStdString());
        updateObject(multi);
        dialog->reject();
    });
}

void JsonManager::mostraMenu(Riviste* rivista, QDialog* dialog, QGridLayout* dialogLayout, QWidget* parent){
    mostraMenu(static_cast<Cartaceo*>(rivista), dialog, dialogLayout, parent);
    QPushButton* diffusione = new QPushButton("Modifica diffusione", dialog);
    dialogLayout->addWidget(diffusione, 2, 2, 1, 1);
    connect(diffusione, &QPushButton::clicked, this, [this, parent, rivista, dialog](){
        QStringList choices{"Provinciale", "Regionale", "Nazionale", "Internazionale"};
        QString choice;
        bool ok;
        do {
            choice = QInputDialog::getItem(parent, "Diffusione", "Inserisci la diffusione della rivista", choices, 0, false, &ok, Qt::Dialog);
            if(!ok){
                QMessageBox::warning(parent, "Annullamento", "Hai annullato l'operazione");
                return;
            }
            if (choice.isEmpty()) {
                QMessageBox::warning(parent, "Input non valido", "Devi selezionare una delle opzioni");
            }
        }
        while (choice.isEmpty());
        Riviste::Diffusione diff;
        if (choice == "Provinciale")
            diff = Riviste::Provinciale;
        else if (choice == "Regionale")
            diff = Riviste::Regionale;
        else if (choice == "Nazionale")
            diff = Riviste::Nazionale;
        else if (choice == "Internazionale")
            diff = Riviste::Internazionale;
        else
            diff = Riviste::Provinciale;
        rivista->setDiffusione(diff);
        updateObject(rivista);
        dialog->reject();
    });
}

void JsonManager::mostraMenu(Libri* libro, QDialog* dialog, QGridLayout* dialogLayout, QWidget* parent){
    mostraMenu(static_cast<Cartaceo*>(libro), dialog, dialogLayout, parent);
    QPushButton* lingua = new QPushButton("Modifica lingua\noriginale", dialog);
    dialogLayout->addWidget(lingua, 2, 2, 1, 1);
    QPushButton* nvolumi = new QPushButton("Modifica numero\nvolumi", dialog);
    dialogLayout->addWidget(nvolumi, 3, 0, 1, 1);
    connect(lingua, &QPushButton::clicked, this, [this, parent, libro, dialog](){
        std::pair<QString, bool> newlanguage = getValidTextInput(parent, "Nuovo studio",
                                                               "Inserisci il nuovo studio");
        if(newlanguage.second == false){
            QMessageBox::warning(parent, "Annullamento", "Hai annullato l'operazione");
            return;
        }
        libro->setLanguage(newlanguage.first.toStdString());
        updateObject(libro);
        dialog->reject();
    });
    connect(nvolumi, &QPushButton::clicked, this, [this, parent, libro, dialog](){
        std::pair<int, bool> newnvolumi = getValidIntInput(parent, "Durata",
                                                          "Inserisci la nuova durata", 1, 1);
        if(newnvolumi.second == false){
            QMessageBox::warning(parent, "Annullamento", "Hai annullato l'operazione");
            return;
        }
        libro->setNvolumi(newnvolumi.first);
        updateObject(libro);
        dialog->reject();
    });
}

void JsonManager::mostraMenu(Manga* manga, QDialog* dialog, QGridLayout* dialogLayout, QWidget* parent){
    mostraMenu(static_cast<Libri*>(manga), dialog, dialogLayout, parent);
    QPushButton* concluso = new QPushButton("È concluso?");
    dialogLayout->addWidget(concluso, 3, 1, 1, 1);
    connect(concluso, &QPushButton::clicked, this, [this, parent, manga, dialog](){
        std::pair<bool, bool> newconcluso = getBooleanInputFromList(parent, "Concluso", "È concluso?");
        if(newconcluso.second == false){
            QMessageBox::warning(parent, "Annullamento", "Hai annullato l'operazione");
            return;
        }
        manga->setConcluso(newconcluso.first);
        updateObject(manga);
        dialog->reject();
    });
}

void JsonManager::mostraMenu(Film* film, QDialog* dialog, QGridLayout* dialogLayout, QWidget* parent){
    mostraMenu(static_cast<Multimedia*>(film), dialog, dialogLayout, parent);
    QPushButton* regista = new QPushButton("Modifica regista", dialog);
    dialogLayout->addWidget(regista, 2, 2, 1, 1);
    QPushButton* attore = new QPushButton("Modifica attore\nprotagonista", dialog);
    dialogLayout->addWidget(attore, 3, 0, 1, 1);
    connect(regista, &QPushButton::clicked, this, [this, film, dialog, parent](){
        std::pair<QString, bool> newregista = getValidTextInput(parent, "Regista",
                                                                "Inserisci il nuovo regista");
        if(newregista.second == false){
            QMessageBox::warning(parent, "Annullamento", "Hai annullato l'operazione");
            return;
        }
        film->setRegista(newregista.first.toStdString());
        updateObject(film);
        dialog->reject();
    });
    connect(attore, &QPushButton::clicked, this, [this, film, dialog, parent](){
        std::pair<QString, bool> newattore = getValidTextInput(parent, "Attore",
                                                               "Inserisci il nuovo attore");
        if(newattore.second == false){
            QMessageBox::warning(parent, "Annullamento", "Hai annullato l'operazione");
            return;
        }
        film->setAttore(newattore.first.toStdString());
        updateObject(film);
        dialog->reject();
    });
}

void JsonManager::mostraMenu(Cd* cd, QDialog* dialog, QGridLayout* dialogLayout, QWidget* parent){
    mostraMenu(static_cast<Multimedia*>(cd), dialog, dialogLayout, parent);
    QPushButton* artista = new QPushButton("Modifica artista", dialog);
    dialogLayout->addWidget(artista, 2, 2, 1, 1);
    QPushButton* tracks = new QPushButton("Modifica numero\ntracce", dialog);
    dialogLayout->addWidget(tracks, 3, 0, 1, 1);
    connect(artista, &QPushButton::clicked, this, [this, cd, dialog, parent](){
        std::pair<QString, bool> newartista = getValidTextInput(parent, "Artista",
                                                                "Inserisci il nuovo artista");
        if(newartista.second == false){
            QMessageBox::warning(parent, "Annullamento", "Hai annullato l'operazione");
            return;
        }
        cd->setArtista(newartista.first.toStdString());
        updateObject(cd);
        dialog->reject();
    });
    connect(tracks, &QPushButton::clicked, this, [this, cd, dialog, parent](){
        std::pair<int, bool> newtracks = getValidIntInput(parent, "Numero tracce",
                                                          "Inserisci il nuovo\nnumero di tracce", 1, 1);
        if(newtracks.second == false){
            QMessageBox::warning(parent, "Annullamento", "Hai annullato l'operazione");
            return;
        }
        cd->setTracce(newtracks.first);
        updateObject(cd);
        dialog->reject();
    });
}

//riscrive nel file Json
void JsonManager::updateObject(Biblioteca *biblio){
    notifyObservers(biblioteca);
    bool found = false;
    int index = 0;
    for(int i=0; i<biblioteca.size(); i++){
        if(biblioteca[i] == biblio){
            index = i;
            found = true;
            break;
        }
    }
    if (!found) {
        qWarning() << "Oggetto non trovato in biblioteca";
        return;
    }
    QFile file("dati.json");
    if (!file.open(QIODevice::ReadOnly)){
        qWarning() << "File non aperto";
        return;
    }
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isArray()){
        qWarning() << "File non valido";
        return;
    }
    QJsonArray array = doc.array();
    QMap<QString, std::function<QJsonObject(Biblioteca*)>>
        saveFunctions = {
                         {"Film", [this](Biblioteca* b) { return saveFilm(static_cast<Film*>(b)); }},
                         {"Cd", [this](Biblioteca* b) { return saveCd(static_cast<Cd*>(b)); }},
                         {"Riviste", [this](Biblioteca* b) { return saveRiviste(static_cast<Riviste*>(b)); }},
                         {"Libri", [this](Biblioteca* b) { return saveLibri(static_cast<Libri*>(b)); }},
                         {"Manga", [this](Biblioteca* b) { return saveManga(static_cast<Manga*>(b)); }},
                         };
    for (int i = 0; i < array.size(); ++i) {
        QJsonObject obj = array[i].toObject();
        if(i==index){
            QString className = obj["Classe"].toString();
            if (saveFunctions.contains(className))
                obj = saveFunctions[className](biblio);
            else{
                qWarning() << "Classe non esistente";
                break;
            }
            array[i] = obj;
            break;
        }
    }
    file.close();
    if(file.open(QIODevice::WriteOnly)){
        QJsonDocument updatedDoc(array);
        file.write(updatedDoc.toJson());
        file.close();
    }
    else{
        qWarning() << "File non apribile in scrittura";
        return;
    }
}

void JsonManager::savePrenota(Biblioteca *biblio){
    int occurenceToSkip = 0;
    for(int i = 0; i<biblioteca.size(); i++){
        if(biblioteca[i] == biblio)
            break;
        if(biblioteca[i]->getTitolo() == biblio->getTitolo())
            occurenceToSkip++;
    }
    QFile file("dati.json");
    if (!file.open(QIODevice::ReadOnly)){
        qWarning() << "File non aperto";
        return;
    }
    // Leggi il contenuto del file
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    // Verifica se il documento è valido
    if (doc.isNull() || !doc.isArray()){
        qWarning() << "File non valido";
        file.close();
        return;
    }
    bool updated = false;
    QJsonArray array = doc.array();
    QJsonArray update;
    int occurences = 0;
    for (int i = 0; i < array.size(); ++i) {
        QJsonObject obj = array[i].toObject();
        if (obj["Titolo"].toString() == QString::fromStdString(biblio->getTitolo())) {
            if (occurences == occurenceToSkip) {
                obj["Disponibile"] = biblio->getDisponibilita();
                obj["Copie disponibili"] = biblio->getCopie();
                updated = true;
            }
            occurences++;
        }
        update.append(obj);
    }
    if(updated){
        file.close();
        if(file.open(QIODevice::WriteOnly)){
            QJsonDocument updatedDoc(update);
            file.write(updatedDoc.toJson());
            file.close();
        }
        else
            qWarning() << "File non apribile in scrittura";
    }
    else
        qWarning() << "Titolo non trovato";
}

void JsonManager::saveLetto(Cartaceo *carta){
    int occurenceToSkip = 0;
    for(int i = 0; i<biblioteca.size(); i++){
        if(biblioteca[i] == carta)
            break;
        if(carta->getTitolo() == biblioteca[i]->getTitolo())
            occurenceToSkip++;
    }
    QFile file("dati.json");
    if (!file.open(QIODevice::ReadOnly)){
        qWarning() << "File non aperto";
        return;
    }
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    // Verifica se il documento è valido
    if (doc.isNull() || !doc.isArray()){
        qWarning() << "File non valido";
        file.close();
        return;
    }
    bool updated = false;
    QJsonArray array = doc.array();
    QJsonArray update;
    int occurences = 0;
    for (int i = 0; i < array.size(); i++) {
        QJsonObject obj = array[i].toObject();
        if (obj["Titolo"].toString() == QString::fromStdString(carta->getTitolo())) {
            if (occurences == occurenceToSkip) {
                obj["Letto"] = carta->getLetto();
                updated = true;
            }
            occurences++;
        }
        update.append(obj);
    }
    if(updated){
        file.close();
        if(file.open(QIODevice::WriteOnly)){
            QJsonDocument updatedDoc(update);
            file.write(updatedDoc.toJson());
            file.close();
        }
        else{
            qWarning() << "File non apribile in scrittura";
            return;
        }
    }
    else
        qWarning() << "Titolo non trovato";
}

void JsonManager::saveVisto(Film *film){
    int occurenceToSkip = 0;
    for(int i = 0; i<biblioteca.size(); i++){
        if(biblioteca[i] == film)
            break;
        if(film->getTitolo() == biblioteca[i]->getTitolo())
            occurenceToSkip++;
    }
    QFile file("dati.json");
    if (!file.open(QIODevice::ReadOnly)){
        qWarning() << "File non aperto";
        return;
    }
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    // Verifica se il documento è valido
    if (doc.isNull() || !doc.isArray()){
        qWarning() << "File non valido";
        return;
    }
    bool updated = false;
    QJsonArray array = doc.array();
    QJsonArray update;
    int occurences = 0;
    for (int i = 0; i < array.size(); ++i) {
        QJsonObject obj = array[i].toObject();
        if (obj["Titolo"].toString() == QString::fromStdString(film->getTitolo())) {
            if (occurences == occurenceToSkip) {
                obj["Visto"] = film->getVisto();
                updated = true;
            }
            occurences++;
        }
        update.append(obj);
    }
    if(updated){
        file.close();
        if(file.open(QIODevice::WriteOnly)){
            QJsonDocument updatedDoc(update);
            file.write(updatedDoc.toJson());
            file.close();
        }
        else{
            qWarning() << "File non apribile in scrittura";
            return;
        }
    }
    else
        qWarning() << "Titolo non trovato";
}

void JsonManager::saveAscoltato(Cd *cd){
    int occurenceToSkip = 0;
    for(int i = 0; i<biblioteca.size(); i++){
        if(biblioteca[i] == cd)
            break;
        if(cd->getTitolo() == biblioteca[i]->getTitolo())
            occurenceToSkip++;
    }
    QFile file("dati.json");
    if (!file.open(QIODevice::ReadOnly)){
        qWarning() << "File non aperto";
        return;
    }
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isArray()){
        qWarning() << "File non valido";
        return;
    }
    bool updated = false;
    QJsonArray array = doc.array();
    QJsonArray update;
    int occurences = 0;
    for (int i = 0; i < array.size(); ++i) {
        QJsonObject obj = array[i].toObject();
        if (obj["Titolo"].toString() == QString::fromStdString(cd->getTitolo())) {
            if (occurences == occurenceToSkip) {
                obj["Ascoltato"] = cd->getAscoltato();
                updated = true;
            }
            occurences++;
        }
        update.append(obj);
    }
    if(updated){
        file.close();
        if(file.open(QIODevice::WriteOnly)){
            QJsonDocument updatedDoc(update);
            file.write(updatedDoc.toJson());
            file.close();
        }
        else{
            qWarning() << "File non apribile in scrittura";
            return;
        }
    }
    else
        qWarning() << "Titolo non trovato";
}

void JsonManager::savenewObject(Biblioteca* biblio) {
    QJsonObject newObject;
    if (typeid(*biblio)==typeid(Libri))
        newObject = saveLibri(static_cast<Libri*>(biblio));
    else if (typeid(*biblio)==typeid(Manga))
        newObject = saveManga(static_cast<Manga*>(biblio));
    else if (typeid(*biblio)==typeid(Cd))
        newObject = saveCd(static_cast<Cd*>(biblio));
    else if (typeid(*biblio)==typeid(Film))
        newObject = saveFilm(static_cast<Film*>(biblio));
    else if (typeid(*biblio)==typeid(Riviste))
        newObject = saveRiviste(static_cast<Riviste*>(biblio));
    QFile file("dati.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Non è possibile aprire il file dati.json per la lettura!";
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (!doc.isArray()) {
        qWarning() << "Il file dati.json non contiene un array!";
        return;
    }
    QJsonArray jsonArray = doc.array();
    jsonArray.append(newObject);
    QJsonDocument updatedDoc(jsonArray);
    // Apri il file in modalità scrittura (sovrascrive tutto)
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Non è possibile aprire il file dati.json per la scrittura!";
        return;
    }
    file.write(updatedDoc.toJson());
    file.close();
}

void JsonManager::saveBiblioteca(Biblioteca* biblio, QJsonObject& obj){
    obj["Titolo"] = QString::fromStdString(biblio->getTitolo());
    obj["Genere"] = QString::fromStdString(biblio->getGenere());
    obj["Anno"] = biblio->getAnno();
    obj["Costo"] = biblio->getCosto();
    obj["Disponibile"] = biblio->getDisponibilita();
    obj["Copie disponibili"] = biblio->getCopie();
    obj["Nprestiti"] = biblio->getNprestiti();
    obj["Immagine"] = QString::fromStdString(biblio->getImmagine());
}

void JsonManager::saveMultimedia(Multimedia *multi, QJsonObject& obj){
    obj["Durata"] = multi->getDurata();
    obj["Studio"] = QString::fromStdString(multi->getStudio());
}

void JsonManager::saveCartaceo(Cartaceo *carta, QJsonObject& obj){
    obj["Autore"] = QString::fromStdString(carta->getAutore());
    obj["Editore"] = QString::fromStdString(carta->getEditore());
}

QJsonObject JsonManager::saveLibri(Libri* libro){
    QJsonObject obj;
    obj["Classe"]="Libri";
    saveBiblioteca(libro, obj);
    saveCartaceo(libro, obj);
    obj["Lingua originale"] = QString::fromStdString(libro->getLanguage());
    obj["Nvolumi"] = libro->getNvolumi();
    obj["Letto"] = libro->getLetto();
    return obj;
}

QJsonObject JsonManager::saveManga(Manga* manga){
    QJsonObject obj;
    obj["Classe"]="Manga";
    saveBiblioteca(manga, obj);
    saveCartaceo(manga, obj);
    obj["Lingua originale"] = QString::fromStdString(manga->getLanguage());
    obj["Nvolumi"] = manga->getNvolumi();
    obj["Letto"] = manga->getLetto();
    obj["Concluso"] = manga->getConcluso();
    return obj;
}

QJsonObject JsonManager::saveCd(Cd* cd){
    QJsonObject obj;
    obj["Classe"]="Cd";
    saveBiblioteca(cd, obj);
    saveMultimedia(cd, obj);
    obj["Artista"] = QString::fromStdString(cd->getArtista());
    obj["Ntracce"] = cd->getTracce();
    obj["Ascoltato"] = cd->getAscoltato();
    return obj;
}

QJsonObject JsonManager::saveFilm(Film* film){
    QJsonObject obj;
    obj["Classe"]="Film";
    saveBiblioteca(film, obj);
    saveMultimedia(film, obj);
    obj["Regista"] = QString::fromStdString(film->getRegista());
    obj["Attore protagonista"] = QString::fromStdString(film->getAttore());
    return obj;
}

QJsonObject JsonManager::saveRiviste(Riviste* rivista){
    QJsonObject obj;
    obj["Classe"]="Riviste";
    saveBiblioteca(rivista, obj);
    saveCartaceo(rivista, obj);
    obj["Diffusione"] = QString::fromStdString(rivista->diffusionToString());
    return obj;
}
