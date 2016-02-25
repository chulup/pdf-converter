#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QImageReader>
#include <QString>
#include <QErrorMessage>
#include <QPrinter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_file_triggered()
{
    auto inputFormatsList = QImageReader::supportedImageFormats();
    auto filters = QString("*.") + QString(inputFormatsList[0]);
    filters = std::accumulate(
                std::begin(inputFormatsList)+1,
                std::end(inputFormatsList),
                filters,
                [](const QString& a, auto b) {
                    return a + " *." + QString(b);
    });
    auto filename = QFileDialog::getOpenFileName(this,
                                                 "Open file",
                                                 QString(),
                                                 "File (" + filters + ")"
                                                 );
    if(filename.isEmpty()) {
        return;
    }
    QImageReader reader(filename);
    _image = reader.read();
    if(_image.isNull()) {
        QErrorMessage::qtHandler()->showMessage("Can't read image " + filename + ":\n" + reader.errorString());
        return;
    }
    auto scene = new QGraphicsScene(this);
    scene->addPixmap(QPixmap::fromImage(_image));
    scene->setSceneRect(_image.rect());
    ui->graphicsView->setScene(scene);
}

void MainWindow::on_actionSave_as_pdf_triggered()
{
    auto filename = QFileDialog::getSaveFileName(this,
                                                 "Export to PDF",
                                                 QString(),
                                                 "PDF (*.pdf)");
    if(filename.isEmpty())
        return;
    if(!filename.toLower().endsWith(".pdf"))
        filename += ".pdf";

    QPrinter printer(QPrinter::ScreenResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setColorMode(QPrinter::Color);
    printer.setOutputFileName(filename);
    printer.setPageSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Portrait);

    QPainter painter(&printer);
    painter.drawImage(QPoint(0, 0), _image);
    painter.end();
}
