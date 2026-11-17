#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "calculations.h"
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <cmath>
#include <stdexcept>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), first(0), operation(0), newNumber(true) {
    ui->setupUi(this);
    for (int i = 0; i < 10; i++) {
        QPushButton *button = findChild<QPushButton *>("digit" + QString::number(i));
        connect(button, &QPushButton::clicked, this, &MainWindow::digitClicked);
    }
    QPushButton *buttons[] = {ui->add, ui->subtract, ui->multiply, ui->divide, ui->root};
    for (QPushButton *button : buttons)
        connect(button, &QPushButton::clicked, this, &MainWindow::operationClicked);

    connect(ui->equals, &QPushButton::clicked, this, &MainWindow::equalsClicked);
    connect(ui->sign, &QPushButton::clicked, this, &MainWindow::signClicked);
    connect(ui->tanButton, &QPushButton::clicked, this, &MainWindow::tangentClicked);
    connect(ui->dbButton, &QPushButton::clicked, this, &MainWindow::decibelsClicked);
    connect(ui->dot, &QPushButton::clicked, this, &MainWindow::dotClicked);
    connect(ui->back, &QPushButton::clicked, this, &MainWindow::backClicked);
    connect(ui->clear, &QPushButton::clicked, this, &MainWindow::clearClicked);
    connect(ui->display, &QLineEdit::returnPressed, this, &MainWindow::equalsClicked);
    connect(ui->display, &QLineEdit::textEdited, this, [this]() { newNumber = false; });
}

MainWindow::~MainWindow() {
    delete ui;
}

double MainWindow::readNumber() {
    bool ok;
    double x = ui->display->text().trimmed().replace(',', '.').toDouble(&ok);
    if (!ok || !std::isfinite(x)) throw std::runtime_error("Введите число.");
    return x;
}

void MainWindow::showNumber(double value) {
    if (!std::isfinite(value)) throw std::runtime_error("Слишком большое число.");
    if (value == 0) value = 0;
    ui->display->setText(QString::number(value, 'g', 15));
    newNumber = true;
}

void MainWindow::showError(const char *message) {
    clearClicked();
    QMessageBox::warning(this, "Ошибка", QString::fromUtf8(message));
}

void MainWindow::digitClicked() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (newNumber) ui->display->clear();
    newNumber = false;
    if (ui->display->text() == "0") ui->display->clear();
    ui->display->insert(button->text());
}

void MainWindow::dotClicked() {
    if (newNumber) ui->display->setText("0");
    newNumber = false;
    QString text = ui->display->text();
    if (!text.contains('.') && !text.contains(',')) {
        if (text.isEmpty() || text == "-") ui->display->insert("0");
        ui->display->insert(".");
    }
}

void MainWindow::operationClicked() {
    try {
        QPushButton *button = qobject_cast<QPushButton *>(sender());
        double x = readNumber();
        if (operation && !newNumber) x = calculate(first, x, operation);
        first = x;
        QString name = button->objectName();
        if (name == "add") operation = '+';
        if (name == "subtract") operation = '-';
        if (name == "multiply") operation = '*';
        if (name == "divide") operation = '/';
        if (name == "root") operation = 'r';
        showNumber(first);
        if (operation == 'r')
            ui->status->setText("Введите степень корня n, затем нажмите =");
        else
            ui->status->setText(QString::number(first, 'g', 15) + " " + button->text());
    } catch (const std::exception &e) {
        showError(e.what());
    }
}

void MainWindow::equalsClicked() {
    if (!operation) return;
    if (newNumber) {
        ui->status->setText("Введите второе число, затем нажмите =");
        return;
    }
    try {
        double result = calculate(first, readNumber(), operation);
        showNumber(result);
        operation = 0;
        ui->status->setText("Результат");
    } catch (const std::exception &e) {
        showError(e.what());
    }
}

void MainWindow::signClicked() {
    try {
        if (operation && newNumber) {
            ui->display->setText("-");
            newNumber = false;
        } else {
            bool wasNew = newNumber;
            showNumber(-readNumber());
            newNumber = wasNew;
        }
    } catch (const std::exception &e) {
        showError(e.what());
    }
}

void MainWindow::tangentClicked() {
    try {
        bool secondNumber = operation != 0;
        showNumber(tangent(readNumber()));
        if (secondNumber) newNumber = false;
        else ui->status->setText("Тангенс, угол в градусах");
    } catch (const std::exception &e) {
        showError(e.what());
    }
}

void MainWindow::decibelsClicked() {
    try {
        bool secondNumber = operation != 0;
        showNumber(decibels(readNumber()));
        if (secondNumber) newNumber = false;
        else ui->status->setText("дБ = 20 · log10(x), отношение амплитуд");
    } catch (const std::exception &e) {
        showError(e.what());
    }
}

void MainWindow::backClicked() {
    if (operation && newNumber) return;
    newNumber = false;
    ui->display->backspace();
    if (ui->display->text().isEmpty()) ui->display->setText("0");
}

void MainWindow::clearClicked() {
    first = 0;
    operation = 0;
    newNumber = true;
    ui->display->setText("0");
    ui->status->setText("Готово");
}
