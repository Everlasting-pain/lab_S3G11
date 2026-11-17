#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void digitClicked();
    void operationClicked();
    void equalsClicked();
    void signClicked();
    void tangentClicked();
    void decibelsClicked();
    void dotClicked();
    void backClicked();
    void clearClicked();

private:
    Ui::MainWindow *ui;
    double first;
    char operation;
    bool newNumber;

    double readNumber();
    void showNumber(double value);
    void showError(const char *message);
};

#endif
