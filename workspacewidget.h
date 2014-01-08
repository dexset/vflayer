#ifndef WORKSPACEWIDGET_H
#define WORKSPACEWIDGET_H

#include <QWidget>
#include <QMouseEvent>

#include "program.h"

class WorkspaceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WorkspaceWidget(QWidget *parent, Program* p);

    void loadImage( QString name );
    void setTool( Tool *t );

protected:

    Program *prog;
    QImage img;

    vec2 oldpos;
    vec2 m_oldpos;
    vec2 size;
    vec2 offset;

    float vf_step = 20;
    float vf_scale = 0.5;
    int mode = 0;

    void paintEvent(QPaintEvent*);
    void wheelEvent(QWheelEvent *);
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);

    vec2 mapCursorToTool( float x, float y );
    vec2 mapCursorToTool( const vec2& p ) { return mapCursorToTool( p.x, p.y ); }

    vec2 mapToolToCursor( float x, float y );
    vec2 mapToolToCursor( const vec2& p ) { return mapToolToCursor( p.x, p.y ); }

signals:

public slots:

};

#endif // WORKSPACEWIDGET_H
