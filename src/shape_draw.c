#include "shape_draw.h"
#include "raylib/raylib.h"
#include "raylib/rlgl.h"

#include <stdlib.h>

typedef struct {
    Vector2 a;
    Vector2 b;
    Vector2 c;
} Triangle;

bool IsConvex( Vector2 prev, Vector2 curr, Vector2 next ) {
    return (curr.x - prev.x) * (next.y - curr.y) - (curr.y - prev.y) * (next.x - curr.x) < 0;
}

bool IsTriangleCCW( Vector2 a, Vector2 b, Vector2 c ) {
    return ( (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x) ) < 0;
}

// Ear Clipping
int TriangulatePolygon( const Vector2 *points, int pointCount, Triangle *triangles, int maxTriangles ) {

    if ( pointCount < 3 ) {
        return 0;
    }

    int *indices = (int*) malloc( sizeof(int) * pointCount );
    for ( int i = 0; i < pointCount; i++ ) {
        indices[i] = i;
    }

    int n = pointCount;
    int triCount = 0;

    while ( n > 3 && triCount < maxTriangles ) {

        bool earFound = false;

        for ( int i = 0; i < n; i++ ) {

            int i0 = indices[(i + n - 1) % n];
            int i1 = indices[i];
            int i2 = indices[(i + 1) % n];

            Vector2 prev = points[i0];
            Vector2 curr = points[i1];
            Vector2 next = points[i2];

            if ( !IsConvex( prev, curr, next ) ) {
                continue;
            }

            bool containsPoint = false;
            for ( int j = 0; j < n; j++ ) {
                int idx = indices[j];
                if ( idx == i0 || idx == i1 || idx == i2 ) {
                    continue;
                }
                if ( CheckCollisionPointTriangle( points[idx], prev, curr, next ) ) {
                    containsPoint = true;
                    break;
                }
            }

            if ( !containsPoint ) {
                triangles[triCount++] = (Triangle){ prev, curr, next };
                for ( int j = i; j < n - 1; j++ ) {
                    indices[j] = indices[j + 1];
                }
                n--;
                earFound = true;
                break;
            }

        }

        if ( !earFound ) {
            break;
        }

    }

    if ( n == 3 && triCount < maxTriangles ) {
        triangles[triCount++] = (Triangle){
            points[indices[0]],
            points[indices[1]],
            points[indices[2]]
        };
    }

    free( indices );

    return triCount;

}

void DrawConcaveShape( const Vector2 *points, int pointCount, Color color ) {

    Triangle triangles[128];
    int triCount = TriangulatePolygon( points, pointCount, triangles, 128 );

    rlBegin( RL_TRIANGLES );
    rlColor4ub( color.r, color.g, color.b, color.a );

    for ( int i = 0; i < triCount; i++ ) {

        Vector2 a = triangles[i].a;
        Vector2 b = triangles[i].b;
        Vector2 c = triangles[i].c;

        if ( !IsTriangleCCW( a, b, c ) ) {
            Vector2 tmp = b;
            b = c;
            c = tmp;
        }

        rlVertex2f( a.x, a.y );
        rlVertex2f( b.x, b.y );
        rlVertex2f( c.x, c.y );

    }

    rlEnd();

}

void DrawShape( const Vector2 *points, int pointCount, Color color ) {

    if ( pointCount < 3 ) {
        return;
    }

    Vector2 center = { 0 };
    for ( int i = 0; i < pointCount; i++ ) {
        center.x += points[i].x;
        center.y += points[i].y;
    }
    center.x /= pointCount;
    center.y /= pointCount;

    rlBegin( RL_TRIANGLES );
    rlColor4ub( color.r, color.g, color.b, color.a );

    for ( int i = 0; i < pointCount; i++ ) {
        Vector2 p1 = points[i];
        Vector2 p2 = points[(i + 1) % pointCount];
        rlVertex2f( center.x, center.y );
        rlVertex2f( p1.x, p1.y );
        rlVertex2f( p2.x, p2.y );
    }

    rlEnd();

}

void DrawShapeLines( const Vector2 *points, int pointCount, Color color ) {

    if ( pointCount < 3 ) {
        return;
    }

    rlBegin( RL_LINES );
    rlColor4ub( color.r, color.g, color.b, color.a );

    for ( int i = 0; i < pointCount; i++ ) {
        Vector2 p1 = points[i];
        Vector2 p2 = points[(i + 1) % pointCount];
        rlVertex2f( p1.x, p1.y );
        rlVertex2f( p2.x, p2.y );
    }

    rlEnd();

}
