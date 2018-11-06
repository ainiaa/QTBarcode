#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QZXing/QZXing>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_encoderButton_clicked();

    void on_encoderButton1_clicked();

    void on_encoderButton2_clicked();

    void on_encoderButton3_clicked();

    void on_encoderButton4_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
