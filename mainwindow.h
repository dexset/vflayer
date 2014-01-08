#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

#include <workspacewidget.h>
#include <QFileDialog>
#include <QMessageBox>

#include "program.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public Program
{
    Q_OBJECT
protected:
    WorkspaceWidget *ws;
    WSData *wsdata;
    Comb *comb;
    Scaler *scaler;
    Tool *curtool;

    void setCurTool( Tool *nt );

public:
    explicit MainWindow(QWidget *parent = 0);
    WSData* getWSData();
    Tool* getTool();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
public slots:
    void loadImageFromFile();
    void saveToFile();
    void loadFromFile();
    void selectComb(bool);
    void selectScaler(bool);
    void updateTool( const Setting& s);
};

#endif // MAINWINDOW_H
