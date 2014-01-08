#include <QPainter>
#include "workspacewidget.h"
#include <cmath>

WorkspaceWidget::WorkspaceWidget(QWidget *parent, Program *p) :
    QWidget(parent), prog(p)
{
    offset.x = 0;
    offset.y = 0;
    size.x = 320;
    size.y = 240;
    setMouseTracking(1);
}

void WorkspaceWidget::loadImage(QString name)
{
    img.load(name);
}

vec2 WorkspaceWidget::mapCursorToTool(float x, float y)
{
    WSData *wsdata = prog->getWSData();

    float wk = size.x / (float)(wsdata->width());
    float hk = size.y / (float)(wsdata->height());

    return vec2( (x - offset.x) / wk, (y - offset.y) / hk );
}

vec2 WorkspaceWidget::mapToolToCursor(float x, float y)
{
    WSData *wsdata = prog->getWSData();

    float wk = size.x / (float)(wsdata->width());
    float hk = size.y / (float)(wsdata->height());

    return vec2( x * wk + offset.x, y * hk + offset.y );
}

void WorkspaceWidget::mousePressEvent(QMouseEvent *me)
{
    mode = me->button() == Qt::MidButton ? 2 : 1;

    oldpos = mapCursorToTool( me->x(), me->y() );
    m_oldpos = vec2( me->x(), me->y() );
}

void WorkspaceWidget::mouseReleaseEvent(QMouseEvent *) { mode = 0; }

void WorkspaceWidget::wheelEvent(QWheelEvent *me)
{
    float scale = 0.3;
    float a = size.x / size.y;
    float sx = size.x + size.x * scale * (me->delta() > 0 ? 1 : -1);
    float sy = size.y + size.y * scale * (me->delta() > 0 ? 1 : -1);

    float maxXSize = 3200;
    float minXSize = 32;

    if( sx > maxXSize )
    {
        size.x = maxXSize;
        size.y = size.x / a;
        update();
        return;
    }
    if( sx < minXSize )
    {
        size.x = minXSize;
        size.y = size.x / a;
        update();
        return;
    }

    vec2 p1 = mapCursorToTool( m_oldpos );
    size.x = sx;
    size.y = sy;
    vec2 p2 = mapToolToCursor( p1 );
    offset.x += m_oldpos.x - p2.x;
    offset.y += m_oldpos.y - p2.y;

    update();
}

void WorkspaceWidget::mouseMoveEvent(QMouseEvent *me)
{
    vec2 npos = mapCursorToTool( me->x(), me->y() );
    if( mode == 1 )
    {
        Tool *tool = prog->getTool();
        if( tool ) tool->action( npos, oldpos );
    }
    else if( mode == 2 )
    {
        offset.x += me->x() - m_oldpos.x;
        offset.y += me->y() - m_oldpos.y;
    }
    oldpos = npos;
    m_oldpos = vec2( me->x(), me->y() );
    update();
}

void WorkspaceWidget::paintEvent(QPaintEvent *)
{
    QPainter paint(this);

    paint.drawImage( offset.x, offset.y, img.scaled( size.x, size.y, Qt::IgnoreAspectRatio ) );
    paint.setPen(QPen(QColor(255,255,255)));

    //TODO: проверки prog, wsdata, tool

    WSData *wsdata = prog->getWSData();

    float wk = size.x / (float)(wsdata->width());
    float hk = size.y / (float)(wsdata->height());

    for( int j=0; j < wsdata->height(); j+=vf_step/hk )
    {
        for( int i=0; i < wsdata->width(); i+=vf_step/wk )
        {
            vec2 v = wsdata->getVector(i,j);
            int x, y, vx, vy;
            x = offset.x + i * wk;
            y = offset.y + j * hk;
            vx = x + v.x * vf_step * vf_scale;
            vy = y + v.y * vf_step * vf_scale;
            paint.drawRect( QRect( x-1, y-1, 2, 2 ) );
            paint.drawLine( x, y, vx, vy );
        }
    }
    prog->getTool()->draw(paint, m_oldpos);
}
