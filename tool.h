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
public:
    Setting rad, force;
    Brush( float F, float R=50 ):
        force(QString("force"),0,50,F),
        rad(QString("radius"),0,50,R)
    {}

    virtual void setSetting(const Setting &s)
    {
        if( s.name == QString("radius") )
            rad.val = s.val;
        else if( s.name == QString("force") )
            force.val = s.val;
    }

    SettingList getSettings()
    {
        SettingList sm;
        sm.push_back( rad );
        sm.push_back( force );
        return sm;
    }

    void draw( QPainter &p, const vec2& v, const vec2& s )
    {
        float rx = rad.val * s.x;
        float ry = rad.val * s.y;
        p.drawEllipse( v.x - rx, v.y - ry, rx * 2, ry * 2 );
    }
};

class Comb: public Brush
{
public:

    Comb( float F=5, float R=50 ): Brush(F, R)
    {
    }

    void action( const vec2& cur, const vec2& old )
    {
        float px = cur.x;
        float py = cur.y;

        float dx = px - old.x;
        float dy = py - old.y;

        float r = rad.val;
        float f = force.val;

        if( r < 0.01 ) return;

        for( int i=-r; i<r; ++i)
            for( int j=-r; j<r; ++j )
            {
                int tx = px+i;
                int ty = py+j;

                if( tx >= 0 && tx < wsdata->width() && ty >= 0 && ty < wsdata->height() )
                {

                    float v = ( r - sqrt(i*i+j*j+1) ) / r;
                    float kk = f * 0.01 * ( v > 0 ? v : 0 ) ;

                    vec2 vv = wsdata->getVector(tx, ty);

                    vv.x += dx * kk;
                    vv.y += dy * kk;

                    wsdata->setVector(tx,ty,vv);
                }
            }
    }

};

class Scaler: public Brush
{
public:

    Scaler( float F=0.5, float R=50 ): Brush(F, R)
    {
        force.min = -25;
        force.max =  25;
    }

    void action( const vec2& cur, const vec2& )
    {
        int px = cur.x;
        int py = cur.y;

        float f = force.val;
        float r = rad.val;

        for( int i=-r; i<r; ++i)
            for( int j=-r; j<r; ++j )
            {
                int tx = px+i;
                int ty = py+j;

                if( tx >= 0 && tx < wsdata->width() && ty >= 0 && ty < wsdata->height() )
                {
                    float v = ( r - sqrt(i*i+j*j) ) / r;
                    float kk = f * 0.01 * ( v > 0 ? v : 0 );

                    vec2 vv = wsdata->getVector(tx, ty);

                    float vl = sqrt(vv.x * vv.x + vv.y * vv.y + 1);

                    vv.x += vv.x / vl * kk;
                    vv.y += vv.y / vl * kk;

                    wsdata->setVector(tx,ty,vv);
                }
            }
    }
};

class CombNS: public Brush
{
public:

    CombNS( float F=5, float R=50 ): Brush(F, R) { }

    void action( const vec2& cur, const vec2& old )
    {
        float px = cur.x;
        float py = cur.y;

        float dx = px - old.x;
        float dy = py - old.y;

        float r = rad.val;
        float f = force.val;

        for( int i=-r; i<r; ++i)
            for( int j=-r; j<r; ++j )
            {
                int tx = px+i;
                int ty = py+j;

                if( tx >= 0 && tx < wsdata->width() && ty >= 0 && ty < wsdata->height() )
                {
                    float v = ( r - sqrt(i*i+j*j+1) ) / r;
                    float kk = f * 0.01 * ( v > 0 ? v : 0 );

                    vec2 vv = wsdata->getVector(tx, ty);

                    float l = sqrt( vv.x * vv.x + vv.y * vv.y );
                    if( l == 0 ) continue;

                    vv.x /= l;
                    vv.y /= l;

                    vv.x += dx * kk;
                    vv.y += dy * kk;

                    float l2 = sqrt( vv.x * vv.x + vv.y * vv.y );
                    if( l2 == 0 ) continue;

                    vv.x = vv.x / l2 * l;
                    vv.y = vv.y / l2 * l;

                    wsdata->setVector(tx,ty,vv);
                }
            }
    }

};

#endif // TOOL_H
