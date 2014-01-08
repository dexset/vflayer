#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

#include <workspacewidget.h>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    WorkspaceWidget *ws;
    WSData *wsdata;
    Tool *tool;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
public slots:
    void loadImageFromFile();
    void saveToFile();
    void loadFromFile();
};

#endif // MAINWINDOW_H
