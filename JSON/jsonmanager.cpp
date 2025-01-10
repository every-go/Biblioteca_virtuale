#include "jsonmanager.h"
#include "../interfaccia_grafica/adminarea.h"
#include "../interfaccia_grafica/userarea.h"
#include <algorithm>
#include <QStringList>
#include <QMap>
#include <QFile>
#include <QJsonArray>
#include <QByteArray>
#include <QList>
#include <QJsonDocument>
#include <QFileInfo>

// Ho incluso solo gli header necessari per evitare dipendenze ridondanti,
// poiché le classi come Cd, Film, Manga e Riviste includono già le dipendenze comuni
// come Multimedia, Biblioteca e Libri.

#include "../modello_logico/manga.h"
#include "../modello_logico/riviste.h"
#include "../modello_logico/cd.h"
#include "../modello_logico/film.h"

JsonManager::JsonManager(const QString& fileName) : filePath(fileName) {}

void JsonManager::setOggetti(const QList<Biblioteca*>& newBiblioteca){
    biblioteca = newBiblioteca;
    std::sort(biblioteca.begin(), biblioteca.end(), [](Biblioteca* a, Biblioteca* b) {
        return a->getTitolo() < b->getTitolo();
    });
}

void JsonManager::addObserver(JsonObserver* observer){
    observers.append(observer);
}

void JsonManager::notifyObservers(QList<Biblioteca *> &newBiblioteca){
    //dato che so di avere solo due observer admin e user posso
    //chiamare la funzione senza fare i dynamic_cast
    std::sort(newBiblioteca.begin(), newBiblioteca.end(), [](Biblioteca* a, Biblioteca* b) {
        return a->getTitolo() < b->getTitolo();
    });
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
            if(classe == "Riviste")
                biblio = loadRiviste(obj);
            else if(classe =="Libri")
                biblio = loadLibri(obj);
            else if(classe =="Cd")
                biblio = loadCd(obj);
            else if(classe =="Film")
                biblio = loadFilm(obj);
            else if(classe =="Manga")
                biblio = loadManga(obj);
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
    QFile file(filePath);
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
    QFile file(filePath);
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
    QFile file(filePath);
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
    QFile file(filePath);
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
    qDebug() << "Fase 1";
    int occurenceToSkip = 0;
    for(int i = 0; i<biblioteca.size(); i++){
        if(biblioteca[i] == film)
            break;
        if(film->getTitolo() == biblioteca[i]->getTitolo())
            occurenceToSkip++;
    }
    QFile file(filePath);
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
    QFile file(filePath);
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
    biblioteca.append(biblio);
    notifyObservers(biblioteca);
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
    QFile file(filePath);
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
    obj["Letto"] = carta->getLetto();
}

QJsonObject JsonManager::saveLibri(Libri* libro){
    QJsonObject obj;
    obj["Classe"]="Libri";
    saveBiblioteca(libro, obj);
    saveCartaceo(libro, obj);
    obj["Lingua originale"] = QString::fromStdString(libro->getLanguage());
    obj["Nvolumi"] = libro->getNvolumi();
    return obj;
}

QJsonObject JsonManager::saveManga(Manga* manga){
    QJsonObject obj;
    obj["Classe"]="Manga";
    saveBiblioteca(manga, obj);
    saveCartaceo(manga, obj);
    obj["Lingua originale"] = QString::fromStdString(manga->getLanguage());
    obj["Nvolumi"] = manga->getNvolumi();
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
    obj["Visto"] = film->getVisto();
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
