#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPicture>
#include <QPainter>
#include <QImage>
QImage img;

#include <cstdio>

struct vec2 { float x=0, y=0; };

#define WIDTH 800
#define HEIGHT 600

vec2 vdata[WIDTH*HEIGHT];

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    img.load(QString("img.jpg"));
}

vec2 oldpos;

void MainWindow::mousePressEvent(QMouseEvent *me)
{
    oldpos.x = me->x();
    oldpos.y = me->y();
}

void MainWindow::mouseMoveEvent(QMouseEvent *me)
{
    float rad = 10;

    int px = me->x();
    int py = me->y();

    int dx = px - oldpos.x;
    int dy = py - oldpos.y;

    oldpos.x = px;
    oldpos.y = py;

    float k = 0.05;

    for( int i=-rad; i<rad; ++i)
        for( int j=-rad; j<rad; ++j )
        {
            int tx = px+i;
            int ty = py+j;
            if( tx >= 0 && tx < WIDTH && ty >= 0 && ty < HEIGHT )
            {
                vdata[ty*WIDTH+tx].x += dx * k;
                vdata[ty*WIDTH+tx].y += dy * k;
            }
        }

    update();
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter paint(this);
    paint.drawImage(0,0,img.scaled(800,600,Qt::IgnoreAspectRatio));
    paint.setPen(QPen(QColor(255,255,255)));

    float step = 10;
    for( int j=0; j < HEIGHT; j+=step )
    {
        for( int i=0; i < WIDTH; i+=step )
        {
            vec2 v = vdata[j*WIDTH+i];
            paint.drawLine( i, j, i + v.x * step, j + v.y * step );
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
