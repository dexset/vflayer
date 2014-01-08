#ifndef TOOL_H
#define TOOL_H

#include "wsdata.h"
#include <cmath>
#include <cstdio>

#include <QString>
#include <QPainter>

#include <vector>

struct Setting
{
    QString name;
    float min, max, val;

    Setting( QString N="default", float Min=0, float Max=1, float Val=0 ):
        name(N), min(Min), max(Max), val(Val)
    {}

    Setting( const Setting& v ):
        name(v.name), min(v.min), max(v.max), val(v.val)
    {}

    void operator =( const Setting& v)
    {
        name = v.name;
        min = v.min;
        max = v.max;
        val = v.val;
    }
};

typedef std::vector<Setting> SettingList;

class Tool
{
protected:
    WSData *wsdata;

public:
    Tool();

    void setData( WSData * wsd ) { wsdata = wsd; }

    virtual SettingList getSettings() = 0;
    virtual void setSetting( const Setting& s ) = 0;

    virtual void action( const vec2& cur, const vec2& old ) = 0;

    virtual void draw( QPainter& p, const vec2& v, const vec2& s ) = 0;
};

class Brush: public Tool
{
protected:
    virtual void brush_action( const vec2& p, const vec2& d, int i, int j ) = 0;

public:
    Setting rad;

    Brush( float R=50 ):
        rad(QString("radius"),0,200,R)
    {}

    void setSetting(const Setting &s)
    { if( s.name == QString("radius") ) rad.val = s.val; }

    SettingList getSettings()
    {
        SettingList sm;
        sm.push_back( rad );
        return sm;
    }

    void action( const vec2& cur, const vec2& old )
    {
        vec2 dist( cur.x - old.x, cur.y - old.y );

        float r = rad.val;

        if( r < 0.01 ) return;

        for( int i = -r+1; i < r; ++i )
            for( int j = -r+1; j < r; ++j )
            {
                int tx = cur.x+i;
                int ty = cur.y+j;

                if( tx >= 0 && tx < wsdata->width() && ty >= 0 && ty < wsdata->height() )
                    brush_action( cur, dist, i, j );
            }
    }

    void draw( QPainter &p, const vec2& v, const vec2& s )
    {
        float rx = rad.val * s.x;
        float ry = rad.val * s.y;
        p.drawEllipse( v.x - rx, v.y - ry, rx * 2, ry * 2 );
    }
};

class ForceBrush : public Brush
{
public:
    Setting force;

    ForceBrush( float F, float R ): Brush(R), force( QString("force"), 0, 50, F ) { }

    void setSetting(const Setting &s)
    {
        if( s.name == QString("force") ) force.val = s.val;
        else this->Brush::setSetting(s);
    }

    SettingList getSettings()
    {
        SettingList sm = this->Brush::getSettings();
        sm.push_back( force );
        return sm;
    }

};

class Comb: public ForceBrush
{
protected:
    void brush_action(const vec2 &p, const vec2 &d, int i, int j)
    {
        int tx = p.x+i;
        int ty = p.y+j;
        float r = rad.val;
        float f = force.val;

        float v = ( r - sqrt(i*i+j*j+1) ) / r;
        float kk = f * 0.01 * ( v > 0 ? v : 0 ) ;

        vec2 vv = wsdata->getVector(tx, ty);

        vv.x += d.x * kk;
        vv.y += d.y * kk;

        wsdata->setVector(tx,ty,vv);
    }

public:

    Comb( float F=5, float R=50 ): ForceBrush(F, R) { }
};

class Scaler: public ForceBrush
{
protected:
    void brush_action(const vec2 &p, const vec2 &d, int i, int j)
    {
        int tx = p.x+i;
        int ty = p.y+j;
        float r = rad.val;
        float f = force.val;

        float v = ( r - sqrt(i*i+j*j) ) / r;
        float kk = f * 0.01 * ( v > 0 ? v : 0 );

        vec2 vv = wsdata->getVector(tx, ty);

        float vl = sqrt(vv.x * vv.x + vv.y * vv.y + 1);

        vv.x += vv.x / vl * kk;
        vv.y += vv.y / vl * kk;

        wsdata->setVector(tx,ty,vv);
    }

public:

    Scaler( float F=0.5, float R=50 ): ForceBrush(F, R)
    {
        force.min = -25;
        force.max =  25;
    }
};

class CombNS: public ForceBrush
{
protected:
    void brush_action(const vec2 &p, const vec2 &d, int i, int j)
    {
        int tx = p.x+i;
        int ty = p.y+j;

        float r = rad.val;
        float f = force.val;

        float v = ( r - sqrt(i*i+j*j+1) ) / r;
        float kk = f * 0.01 * ( v > 0 ? v : 0 );

        vec2 vv = wsdata->getVector(tx, ty);

        float l = sqrt( vv.x * vv.x + vv.y * vv.y );
        if( l == 0 ) return;

        vv.x /= l;
        vv.y /= l;

        vv.x += d.x * kk;
        vv.y += d.y * kk;

        float l2 = sqrt( vv.x * vv.x + vv.y * vv.y );
        if( l2 == 0 ) return;

        vv.x = vv.x / l2 * l;
        vv.y = vv.y / l2 * l;

        wsdata->setVector(p.x+i,p.y+j,vv);
    }

public:

    CombNS( float F=5, float R=50 ): ForceBrush(F, R) { }
};

#endif // TOOL_H
