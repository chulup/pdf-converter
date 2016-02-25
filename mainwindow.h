#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_file_triggered();

    void on_actionSave_as_pdf_triggered();

private:
    Ui::MainWindow *ui;
    QImage _image;
};

#endif // MAINWINDOW_H
