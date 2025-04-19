#pragma once

#include "raylib/raylib.h"

// concave and convex shapes. points must be CCW.
void DrawConcaveShape( Vector2 *points, int pointCount, Color color );

// convex shapes (faster). points must be CCW.
void DrawShape( const Vector2 *points, int pointCount, Color color );

// concave and convex. points must be ordered.
void DrawShapeLines( const Vector2 *points, int pointCount, Color color );
