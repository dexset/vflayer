#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingslider.h"
#include <QSpacerItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    wsdata = new WSData( 800, 600 );
    ws = new WorkspaceWidget(this, this);

    comb = new Comb(5, 50);

    combns = new CombNS(5, 50);

    scaler = new Scaler(3, 60);

    setCurTool( comb );

    ws->loadImage(QString("img.jpg"));
    ui->mainLayout->addWidget(ws);
    this->connect(ui->actionSave,SIGNAL(activated()),SLOT(saveToFile()));
    this->connect(ui->actionOpen,SIGNAL(activated()),SLOT(loadFromFile()));
    this->connect(ui->actionOpenImage,SIGNAL(activated()),SLOT(loadImageFromFile()));
    this->connect(ui->actionExit,SIGNAL(activated()),SLOT(saveAndQuit()));
}

void MainWindow::saveAndQuit()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Save", "Do You want to save changes?",
                                QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        saveToFile();
    }
    close();
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
    int w, h;
    size_t s;
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
    int w, h;
    size_t s;
    fread( &w, sizeof(int), 1, f );
    fread( &h, sizeof(int), 1, f );
    fread( &s, sizeof(size_t), 1, f);
    if( wsdata )
        delete wsdata;
    wsdata = new WSData( w, h );
    fread( wsdata->getData(), sizeof(vec2), wsdata->width() * wsdata->height(), f );
    fclose(f);

    update();
}

void MainWindow::setCurTool( Tool *nt )
{
    curtool = nt;
    curtool->setData(wsdata);

    QLayoutItem *item;
    while( ( item = ui->settingGroup->layout()->takeAt(0) ) != NULL )
    {
        delete item->widget();
        delete item;
    }

    SettingList sm = curtool->getSettings();

    for( SettingList::iterator i = sm.begin(); i != sm.end(); ++i )
    {
        SettingSlider* ss = new SettingSlider(this);
        ss->setSetting( (*i) );
        ui->settingGroup->layout()->addWidget( ss );
        this->connect(ss, SIGNAL(updateSetting( const Setting& )), SLOT(updateTool( const Setting& )) );
    }
}

void MainWindow::updateTool(const Setting &s)
{
    curtool->setSetting(s);
}

void MainWindow::selectComb(bool v) { if( v ) setCurTool( comb ); }
void MainWindow::selectCombNS(bool v) { if( v ) setCurTool( combns ); }
void MainWindow::selectScaler(bool v) { if( v ) setCurTool( scaler ); }

WSData* MainWindow::getWSData() { return wsdata; }
Tool* MainWindow::getTool() { return curtool; }

MainWindow::~MainWindow()
{
    delete ui;
    delete wsdata;
    delete ws;
    delete comb;
    delete scaler;
}
