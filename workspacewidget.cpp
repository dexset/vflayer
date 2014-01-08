#include <QPainter>
#include "workspacewidget.h"
#include <cmath>

WorkspaceWidget::WorkspaceWidget(QWidget *parent, WSData *ws) :
    QWidget(parent), wsdata(ws)
{
    w = wsdata->width();
    h = wsdata->height();
}

void WorkspaceWidget::loadImage(QString name) { img.load(name); }

void WorkspaceWidget::setTool( Tool *t ) { tool = t; }

void WorkspaceWidget::mousePressEvent(QMouseEvent *me)
{
    oldpos.x = me->x();
    oldpos.y = me->y();
}

void WorkspaceWidget::mouseMoveEvent(QMouseEvent *me)
{
    if( tool ) tool->action( vec2(me->x(), me->y()), oldpos );
    oldpos.x = me->x();
    oldpos.y = me->y();
    update();
}

void WorkspaceWidget::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    paint.drawImage( 0, 0, img.scaled( w, h, Qt::IgnoreAspectRatio ) );
    paint.setPen(QPen(QColor(255,255,255)));

    float step = 10;
    float wk = w / (float)(wsdata->width());
    float hk = h / (float)(wsdata->height());
    for( int j=0; j < wsdata->height(); j+=step )
    {
        for( int i=0; i < wsdata->width(); i+=step )
        {
            vec2 v = wsdata->getVector(i,j);
            int x, y, vx, vy;
            x = i * wk;
            y = j * hk;
            vx = x + v.x * step;
            vy = y + v.y * step;
            paint.drawLine( x, y, vx, vy );
        }
    }
}
