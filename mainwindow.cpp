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
}

MainWindow::~MainWindow()
{
    delete ui;
    delete wsdata;
    delete ws;
    delete tool;
}
