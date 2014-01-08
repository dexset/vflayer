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
}

void MainWindow::saveToFile()
{
    QString fname = QFileDialog::getSaveFileName();
    if(fname == QString(""))
        return;
    FILE* f;
    f = fopen( fname.toUtf8().constData(), "wb" );
    fwrite( wsdata->getData(), sizeof(vec2), wsdata->width() * wsdata->height(), f );
    fclose(f);
}

void MainWindow::loadFromFile()
{
    QString fname = QFileDialog::getOpenFileName();
    if(fname == QString(""))
        return;
    FILE* f;
    f = fopen( fname.toUtf8().constData(), "rb" );
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
