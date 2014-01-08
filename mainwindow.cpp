#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    wsdata = new WSData( 800, 600 );
    ws = new WorkspaceWidget(this, wsdata);
    tool = new Tool;
    tool->setData(wsdata);
    ws->setTool( tool );
    ws->loadImage(QString("img.jpg"));
    ui->horizontalLayout->addWidget(ws);
    this->connect(ui->actionSave,SIGNAL(activated()),SLOT(saveToFile()));
    this->connect(ui->actionOpen,SIGNAL(activated()),SLOT(loadFromFile()));
    this->connect(ui->actionOpenImage,SIGNAL(activated()),SLOT(loadImageFromFile()));
}

void MainWindow::loadImageFromFile()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Save", "Do You want to save changes?",
                                QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        saveToFile();
    }

    QString fname = QFileDialog::getOpenFileName();
    if(fname == QString(""))
        return;
    ws->loadImage(QString(fname));
    update();
}

void MainWindow::saveToFile()
{
    QString fname = QFileDialog::getSaveFileName();
    if(fname == QString(""))
        return;
    FILE* f;
    f = fopen( fname.toUtf8().constData(), "wb" );
    int w, h, s;
    w = wsdata->width();
    h = wsdata->height();
    s = sizeof(vec2);
    fwrite( &w, sizeof(int), 1, f );
    fwrite( &h, sizeof(int), 1, f );
    fwrite( &s, sizeof(size_t), 1, f);
    fwrite( wsdata->getData(), sizeof(vec2), wsdata->width() * wsdata->height(), f );
    fclose(f);
}

void MainWindow::loadFromFile()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Save", "Do You want to save changes?",
                                QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        saveToFile();
    }
    QString fname = QFileDialog::getOpenFileName();
    if(fname == QString(""))
        return;
    FILE* f;
    f = fopen( fname.toUtf8().constData(), "rb" );
    int w, h, s;
    fread( &w, sizeof(int), 1, f );
    fread( &h, sizeof(int), 1, f );
    if( wsdata )
        delete wsdata;
    wsdata = new WSData( 800, 600 );
    fread( &s, sizeof(size_t), 1, f);
    fread( wsdata->getData(), sizeof(vec2), wsdata->width() * wsdata->height(), f );
    fclose(f);
    update();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete wsdata;
    delete ws;
    delete tool;
}
