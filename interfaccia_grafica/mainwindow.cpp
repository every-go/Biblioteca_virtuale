#include "mainwindow.h"
#include <QSplitter>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QDialog>
#include <QToolBar>
#include <QIcon>

MainWindow::MainWindow(QStackedWidget* stackWidget, QWidget* parent) : QMainWindow(parent), stack(stackWidget) {
    setWindowTitle("Biblioteca");
    resize(800,600);
    QWidget *paginaIniziale= new QWidget(this);
    setCentralWidget(paginaIniziale);
    // Crea la barra degli strumenti
    QToolBar *toolBar = new QToolBar("Main Toolbar", this);
    addToolBar(Qt::TopToolBarArea, toolBar);
    QPushButton* close = new QPushButton("Chiudi", paginaIniziale);
    toolBar->addWidget(close);
    this->setStyleSheet("QMainWindow{"
                        "background-image: url(:/images/biblioteca.png);"
                        "background-position: center;"
                        "}");

    //creo il layout principale con un layout verticale
    QVBoxLayout *mainLayout = new QVBoxLayout(paginaIniziale);
    //il QSplitter è un widget per separare due aree (admin e utente)
    QSplitter *splitter = new QSplitter(Qt::Horizontal, paginaIniziale);
    splitter->setStyleSheet("QSplitter {"
                            "border: 1px solid #888; "
                            "}");
    mainLayout->addWidget(splitter);
    //l'area admin e l'area user "vivono" sullo splitter
    QWidget *adminArea = new QWidget(splitter);
    QWidget *userArea = new QWidget(splitter);
    //creo i layout per le due aree
    QVBoxLayout *adminLayout = new QVBoxLayout(adminArea);
    QVBoxLayout *userLayout = new QVBoxLayout(userArea);
    // Aggiungi i pulsanti per le due aree
    QLabel *adminText = new QLabel("Accedi alla parte admin della biblioteca", adminArea);
    QLabel *userText = new QLabel("Accedi alla parte utente della biblioteca", userArea);
    QPushButton *adminButton = new QPushButton("Accesso Admin", adminArea);
    QPushButton *userButton = new QPushButton("Accesso Utente", userArea);
    adminButton->setStyleSheet("QPushButton{"
                               "background-color: #4CAF40;"
                               "}");
    userButton->setStyleSheet("QPushButton{"
                               "background-color: #4CAF40;"
                               "}");
    QIcon admin("./IMG/admin.svg");
    adminButton->setIcon(admin);
    QIcon user("./IMG/user.svg");
    userButton->setIcon(user);
    adminText->setAlignment(Qt::AlignCenter);
    userText->setAlignment(Qt::AlignCenter);
    adminText->setStyleSheet("QLabel{"
                             "color: red"
                             "}");
    userText->setStyleSheet("QLabel{"
                             "color: red"
                             "}");

    adminLayout->setAlignment(Qt::AlignCenter);
    userLayout->setAlignment(Qt::AlignCenter);

    adminLayout->addWidget(adminText);
    adminLayout->addWidget(adminButton);
    userLayout->addWidget(userText);
    userLayout->addWidget(userButton);
    //connessione coi slot
    connect(userButton, &QPushButton::clicked, this, &MainWindow::showUser);
    connect(adminButton, &QPushButton::clicked, this, &MainWindow::admin);
    connect(close, &QPushButton::clicked, this, &MainWindow::chiudi);
}

void MainWindow::showUser(){
    QMessageBox::information(this, "Parte utente", "Stai accedendo alla parte utente");
    stack->setCurrentIndex(1);
}

void MainWindow::admin() {
    /*QDialog *loginDialog = new QDialog(this);
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
    loginDialog->exec();*/
    showAdmin();
}

void MainWindow::showAdmin(){
    stack->setCurrentIndex(2);
    //QMessageBox::information(this, "Parte admin", "Stai accedendo alla parte admin");
}

void MainWindow::chiudi(){
    stack->close();
}
