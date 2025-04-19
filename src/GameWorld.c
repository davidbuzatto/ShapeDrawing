/**
 * @file GameWorld.h
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld implementation.
 * 
 * @copyright Copyright (c) 2025
 */
#include <stdio.h>
#include <stdlib.h>

#include "GameWorld.h"
#include "ResourceManager.h"
#include "shape_draw.h"

#include "raylib/raylib.h"
//#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
//#include "raylib/raygui.h"       // other compilation units must only include
//#undef RAYGUI_IMPLEMENTATION     // raygui.h

#define CONVEX_P_COUNT 5
#define CONCAVE_P_COUNT 10

Vector2 convexPoints[CONVEX_P_COUNT];
Vector2 concavePoints[CONCAVE_P_COUNT];
Vector2 *draggingV;

/**
 * @brief Creates a dinamically allocated GameWorld struct instance.
 */
GameWorld* createGameWorld( void ) {

    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );
    gw->dummy = 0;

    Vector2 cxp[] = {
        { 250, 30 },
        { 50, 50 },
        { 70, 200 },
        { 200, 250 },
        { 300, 150 }
    };

    Vector2 cep[] = {
        { 550, 100 },
        { 530, 180 },
        { 450, 180 },
        { 510, 230 },
        { 490, 310 },
        { 550, 260 },
        { 610, 310 },
        { 590, 230 },
        { 650, 180 },
        { 570, 180 }
    };

    for ( int i = 0; i < CONVEX_P_COUNT; i++ ) {
        convexPoints[i] = cxp[i];
    }

    for ( int i = 0; i < CONCAVE_P_COUNT; i++ ) {
        concavePoints[i] = cep[i];
    }

    draggingV = NULL;

    return gw;

}

/**
 * @brief Destroys a GameWindow object and its dependecies.
 */
void destroyGameWorld( GameWorld *gw ) {
    free( gw );
}

/**
 * @brief Reads user input and updates the state of the game.
 */
void updateGameWorld( GameWorld *gw, float delta ) {

    if ( IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {
        for ( int i = 0; i < CONVEX_P_COUNT; i++ ) {
            if ( CheckCollisionPointCircle( GetMousePosition(), convexPoints[i], 5 ) ) {
                draggingV = &convexPoints[i];
                break;
            }
        }
        for ( int i = 0; i < CONCAVE_P_COUNT; i++ ) {
            if ( CheckCollisionPointCircle( GetMousePosition(), concavePoints[i], 5 ) ) {
                draggingV = &concavePoints[i];
                break;
            }
        }
    }

    if ( IsMouseButtonReleased( MOUSE_BUTTON_LEFT ) ) {
        draggingV = NULL;
    }

    if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) && draggingV != NULL ) {
        draggingV->x = GetMouseX();
        draggingV->y = GetMouseY();
    }

}

/**
 * @brief Draws the state of the game.
 */
void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );
    
    DrawShape( convexPoints, CONVEX_P_COUNT, SKYBLUE );
    DrawShapeLines( convexPoints, CONVEX_P_COUNT, DARKBLUE );

    DrawConcaveShape( concavePoints, CONCAVE_P_COUNT, GREEN );
    DrawShapeLines( concavePoints, CONCAVE_P_COUNT, DARKGREEN );

    for ( int i = 0; i < CONVEX_P_COUNT; i++ ) {
        DrawCircleV( convexPoints[i], 5, SKYBLUE );
        DrawCircleLinesV( convexPoints[i], 5, DARKBLUE );
        DrawText( TextFormat( "%.1f %.1f", convexPoints[i].x, convexPoints[i].y ), convexPoints[i].x, convexPoints[i].y, 20, BLUE );
    }

    for ( int i = 0; i < CONCAVE_P_COUNT; i++ ) {
        DrawCircleV( concavePoints[i], 5, GREEN );
        DrawCircleLinesV( concavePoints[i], 5, DARKGREEN );
        DrawText( TextFormat( "%.1f %.1f", concavePoints[i].x, concavePoints[i].y ), concavePoints[i].x, concavePoints[i].y, 20, LIME );
    }

    EndDrawing();

}