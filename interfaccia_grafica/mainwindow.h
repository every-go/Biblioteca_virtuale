#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QStackedWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QStackedWidget* stack;
public:
    MainWindow(QStackedWidget* stackWidget, QWidget* parent =nullptr);
private slots:
    void showUser();
    void admin();
    void chiudi();
private:
    void showAdmin();
};

#endif // MAINWINDOW_H
