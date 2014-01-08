#ifndef WORKSPACEWIDGET_H
#define WORKSPACEWIDGET_H

#include <QWidget>
#include <QMouseEvent>

#include "wsdata.h"
#include "tool.h"

class WorkspaceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WorkspaceWidget(QWidget *parent, WSData *w);

    void loadImage( QString name );
    void setTool( Tool *t );

protected:

    WSData *wsdata;
    Tool *tool;
    QImage img;


    vec2 oldpos;
    int w, h;

    void paintEvent(QPaintEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);

signals:

public slots:

};

#endif // WORKSPACEWIDGET_H
