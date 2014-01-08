#ifndef TOOL_H
#define TOOL_H

#include "wsdata.h"
#include <cmath>
#include <cstdio>

class Tool
{
protected:
    WSData *wsdata;

public:
    Tool();

    void setData( WSData * wsd ) { wsdata = wsd; }

    void action( const vec2& cur, const vec2& old )
    {
        float rad = 50;

        int px = cur.x;
        int py = cur.y;

        int dx = px - old.x;
        int dy = py - old.y;

        float k = 0.05;

        for( int i=-rad; i<rad; ++i)
            for( int j=-rad; j<rad; ++j )
            {
                int tx = px+i;
                int ty = py+j;


                if( tx >= 0 && tx < wsdata->width() && ty >= 0 && ty < wsdata->height() )
                {
                    float v = rad - sqrt(i*i+j*j);
                    float kk = k * ( v > 0 ? v : 0 ) / rad;

                    vec2 vv = wsdata->getVector(tx, ty);

                    vv.x += dx * kk;
                    vv.y += dy * kk;

                    wsdata->setVector(tx,ty,vv);
                }
            }
    }
};

#endif // TOOL_H
