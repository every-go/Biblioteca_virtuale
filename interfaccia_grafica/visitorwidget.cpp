#include "visitorwidget.h"
#include <QMessageBox>
#include <QPixmap>
#include <QDebug>

// Ho incluso solo gli header necessari per evitare dipendenze ridondanti,
// poiché le classi come Cd, Film, Manga e Riviste includono già le dipendenze comuni
// come Multimedia, Biblioteca e Libri.

#include "../modello_logico/cd.h"
#include "../modello_logico/film.h"
#include "../modello_logico/manga.h"
#include "../modello_logico/riviste.h"

VisitorWidget::VisitorWidget() : widget(new QWidget()), artwork(new QLabel()), layout(new QGridLayout(widget)){
    widget->setLayout(layout);
}

QWidget* VisitorWidget::getWidget() const{
    return widget;
}

void VisitorWidget::visit(Biblioteca* biblio){
    // Visit "principale"
    // Immagine
    QLabel *image = new QLabel(widget);
    QString imagePath = QString::fromStdString(biblio->getImmagine());
    QPixmap pixmap(imagePath);

    // Ridimensiona l'immagine al massimo di 180x260, mantenendo le proporzioni
    pixmap = pixmap.scaled(180, 260, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Imposta il QPixmap all'interno del QLabel
    image->setPixmap(pixmap);

    // Imposta una dimensione fissa per il QLabel (opzionale)
    image->setFixedSize(180, 260);
    image->setAlignment(Qt::AlignCenter); // Allinea l'immagine al centro

    // Aggiungi il QLabel al layout
    layout->addWidget(image, 0, 0, 4, 1);

    // Titolo
    QLabel *titolo = new QLabel(widget);
    titolo->setText(
        QString("<div style='text-align: center;'><b>Titolo</b><br>%1</div>")
            .arg(QString::fromStdString(biblio->getTitolo()))
        );
    titolo->setStyleSheet("QLabel{"
                          "font-size: 20px;"
                          "}");
    layout->addWidget(titolo, 0, 1, 1, 2, Qt::AlignCenter); // Riga 0, colonna 1, si estende su 2 colonne

    // Genere
    QLabel *genere = new QLabel(widget);
    genere->setText(
        QString("<div style='text-align: center;'><b>Genere</b><br>%1</div>")
            .arg(QString::fromStdString(biblio->getGenere()))
        );
    layout->addWidget(genere, 1, 1, 1, 1, Qt::AlignCenter); // Riga 1, colonna 1

    // Anno
    QLabel *anno = new QLabel(widget);
    anno->setText(
        QString("<div style='text-align: center;'><b>Anno</b><br>%1</div>")
            .arg(QString::number(biblio->getAnno()))
        );
    layout->addWidget(anno, 1, 2, 1, 1, Qt::AlignCenter); // Riga 1, colonna 2

    // Prezzo
    QLabel *costo = new QLabel(widget);
    costo->setText(
        QString("<div style='text-align: center;'><b>Costo</b><br>%1</div>")
            .arg(QString::number(biblio->getCosto()))
        );
    layout->addWidget(costo, 2, 1, 1, 1, Qt::AlignCenter); // Riga 2, colonna 1

    // Disponibilità
    QLabel *disponibile = new QLabel(widget);
    disponibile->setText(
        QString("<div style='text-align: center;'><b>%1</b></div>")
            .arg(biblio->getDisponibilita()? "Disponibile" : "Non disponibile")
        );
    layout->addWidget(disponibile, 2, 2, 1, 1, Qt::AlignCenter); // Riga 2, colonna 2
}


void VisitorWidget::visit(Cartaceo* carta){
    // Prima chiama il visit di Biblioteca
    visit(static_cast<Biblioteca*>(carta));
    //autore
    QLabel *autore = new QLabel(widget);
    autore->setText(
        QString("<div style='text-align: center;'><b>Autore</b><br>%1</div>")
            .arg(QString::fromStdString(carta->getAutore()))
        );
    layout->addWidget(autore, 3, 1, 1, 1, Qt::AlignCenter);
    //editore
    QLabel *editore = new QLabel(widget);
    editore->setText(
        QString("<div style='text-align: center;'><b>Editore</b><br>%1</div>")
            .arg(QString::fromStdString(carta->getEditore()))
        );
    layout->addWidget(editore, 3, 2, 1, 1, Qt::AlignCenter);
    QLabel *letto = new QLabel(widget);
    letto->setText(
        carta->getLetto()? "L'oggetto è stato letto" : "L'oggetto non è stato letto");
    layout->addWidget(letto, 4, 0, 1, 1, Qt::AlignCenter);
}

void VisitorWidget::visit(Multimedia* multi){
    // Prima chiama il visit di Biblioteca
    visit(static_cast<Biblioteca*>(multi));
    QLabel *durata = new QLabel(widget);
    durata->setText(
        QString("<div style='text-align: center;'><b>Durata</b><br>%1</div>")
            .arg(QString::number(multi->getDurata()))
        );
    layout->addWidget(durata, 3, 1, 1, 1, Qt::AlignCenter);
    QLabel *studio = new QLabel(widget);
    studio->setText(
        QString("<div style='text-align: center;'><b>Studio</b><br>%1</div>")
            .arg(QString::fromStdString(multi->getStudio()))
        );
    layout->addWidget(studio, 3, 2, 1, 1, Qt::AlignCenter);
}

void VisitorWidget::visit(Libri* libro) {
    // Prima chiama il visit di Cartaceo
    visit(static_cast<Cartaceo*>(libro));
    QLabel* language = new QLabel(widget);
    language->setText(
        QString("<div style='text-align: center;'><b>Lingua originale</b><br>%1</div>")
            .arg(QString::fromStdString(libro->getLanguage()))
        );
    layout->addWidget(language, 4, 1, 1, 1, Qt::AlignCenter);
    QLabel* nvolumi = new QLabel(widget);
    nvolumi->setText(
        QString("<div style='text-align: center;'><b>Numero volumi</b><br>%1</div>")
            .arg(QString::number(libro->getNvolumi()))
        );
    layout->addWidget(nvolumi, 4, 2, 1, 1, Qt::AlignCenter);
}

void VisitorWidget::visit(Manga* manga){
    // Prima chiama il visit di Libri
    visit(static_cast<Libri*>(manga));
    QLabel* concluso = new QLabel(widget);
    concluso->setText(
        QString(manga->getConcluso()? "Il manga è conluso" : "Il manga non è concluso")
        );
    layout->addWidget(concluso, 5, 0, 1, 3, Qt::AlignCenter);
}

void VisitorWidget::visit(Riviste* rivista){
    // Prima chiama il visit di Cartaceo
    visit(static_cast<Cartaceo*>(rivista));
    QLabel* diffusione = new QLabel(widget);
    diffusione->setText(
        QString("<div style='text-align: center;'><b>Diffusione della rivista</b><br>%1</div>")
            .arg(QString::fromStdString(rivista->diffusionToString()))
        );
    layout->addWidget(diffusione, 4, 1, 1, 2, Qt::AlignCenter);
}

void VisitorWidget::visit(Film* film){
    // Prima chiama il visit di Multimedia
    visit(static_cast<Multimedia*>(film));
    QLabel* regista = new QLabel(widget);
    regista->setText(
        QString("<div style='text-align: center;'><b>Regista</b><br>%1</div>")
            .arg(QString::fromStdString(film->getRegista()))
        );
    layout->addWidget(regista, 4, 1, 1, 1, Qt::AlignCenter);
    QLabel* attore = new QLabel(widget);
    attore->setText(
        QString("<div style='text-align: center;'><b>Attore protagonista</b><br>%1</div>")
            .arg(QString::fromStdString(film->getAttore()))
        );
    layout->addWidget(attore, 4, 2, 1, 1, Qt::AlignCenter);
    QLabel* visto = new QLabel(widget);
    visto->setText(
        QString(film->getVisto()? "Il film è già stato guardato" : "Il film non è stato guardato")
        );
    layout->addWidget(visto, 4, 0, 2, 1, Qt::AlignCenter);
}

void VisitorWidget::visit(Cd* cd) {
    // Prima chiama il visit di Multimedia
    visit(static_cast<Multimedia*>(cd));
    QLabel* artista = new QLabel(widget);
    artista->setText(
        QString("<div style='text-align: center;'><b>Artista del disco</b><br>%1</div>")
            .arg(QString::fromStdString(cd->getArtista()))
        );
    layout->addWidget(artista, 4, 1, 1, 1, Qt::AlignCenter);
    QLabel* ntracce = new QLabel(widget);
    ntracce->setText(
        QString("<div style='text-align: center;'><b>Numero di tracce</b><br>%1</div>")
            .arg(QString::number(cd->getTracce()))
        );
    layout->addWidget(ntracce, 4, 2, 1, 1, Qt::AlignCenter);
    QLabel* ascoltato = new QLabel(widget);
    ascoltato->setText(
        QString(cd->getAscoltato()? "Il cd è stato ascoltato" : "Il cd non è stato ascoltato")
        );
    layout->addWidget(ascoltato, 4, 0, 2, 1, Qt::AlignCenter);
}
