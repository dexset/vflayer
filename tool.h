#ifndef TOOL_H
#define TOOL_H

#include "wsdata.h"
#include <cmath>
#include <cstdio>

#include <QString>

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
};

class Brush: public Tool
{
public:
    float rad;
    Brush( float R=50 ): rad(R) { }

    virtual void setSetting(const Setting &s)
    {
        if( s.name == QString("radius") )
            rad = s.val;
    }

    SettingList getSettings()
    {
        SettingList sm;
        sm.push_back( Setting( QString("radius"), 0, 200, rad ) );
        return sm;
    }
};

class Comb: public Brush
{
public:
    float force;

    Comb( float F=5, float R=50 ): Brush(R), force(F) { }

    void setSetting( const Setting &s )
    {
        if( s.name == QString("force") ) force = s.val;
        else this->Brush::setSetting(s);
    }

    SettingList getSettings()
    {
        SettingList sm = this->Brush::getSettings();
        sm.push_back( Setting( QString("force"), 0, 100, force ) );
        return sm;
    }

    void action( const vec2& cur, const vec2& old )
    {

        int px = cur.x;
        int py = cur.y;

        int dx = px - old.x;
        int dy = py - old.y;

        for( int i=-rad; i<rad; ++i)
            for( int j=-rad; j<rad; ++j )
            {
                int tx = px+i;
                int ty = py+j;


                if( tx >= 0 && tx < wsdata->width() && ty >= 0 && ty < wsdata->height() )
                {
                    float v = rad - sqrt(i*i+j*j);
                    float kk = force * 0.01 * ( v > 0 ? v : 0 ) / rad;

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
    float coef;

    Scaler( float C=0.5, float R=50 ): Brush(R), coef(C) {}

    void setSetting( const Setting &s )
    {
        if( s.name == QString("coef") ) coef = s.val;
        else this->Brush::setSetting(s);
    }

    SettingList getSettings()
    {
        SettingList sm = this->Brush::getSettings();
        sm.push_back( Setting( QString("coef"), -25, 25, coef ) );
        return sm;
    }

    void action( const vec2& cur, const vec2& )
    {
        int px = cur.x;
        int py = cur.y;

        for( int i=-rad; i<rad; ++i)
            for( int j=-rad; j<rad; ++j )
            {
                int tx = px+i;
                int ty = py+j;

                if( tx >= 0 && tx < wsdata->width() && ty >= 0 && ty < wsdata->height() )
                {
                    float v = rad - sqrt(i*i+j*j);
                    float kk = coef * 0.01 * ( v > 0 ? v : 0 ) / rad;

                    vec2 vv = wsdata->getVector(tx, ty);

                    float vl = sqrt(vv.x * vv.x + vv.y * vv.y + 1);

                    vv.x += vv.x / vl * kk;
                    vv.y += vv.y / vl * kk;

                    wsdata->setVector(tx,ty,vv);
                }
            }
    }
};

#endif // TOOL_H
