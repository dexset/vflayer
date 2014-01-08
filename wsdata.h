#ifndef WSDATA_H
#define WSDATA_H

#include <cstdio>

struct vec2
{
    float x, y;
    vec2( float X=0, float Y=0 ): x(X), y(Y) {}
};

class WSData
{
protected:
    vec2 *data;
    int w, h;

public:
    WSData( int W, int H ): w(W), h(H)
    {
        data = new vec2[w*h];
        fprintf( stderr, "create data [ %d %d ]", w, h );
    }

    int width() const { return w; }
    int height() const { return h; }

    const vec2& getVector( int x, int y ) const { return data[y*w+x]; }

    void setVector( int x, int y, const vec2& v ) { data[y*w+x] = v; }

    ~WSData()
    {
        delete data;
    }
};

#endif // WSDATA_H
