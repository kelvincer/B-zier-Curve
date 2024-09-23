#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

static bool onPause = false;
static float t = 0.0f;
Vector2 *points = NULL;
Vector2 curve;
Vector2 line_1;
Vector2 line_2;

void UnloadGame();
void DrawGame();
void InitGame();
void UpdateGame();

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bezier-Curve");
    InitGame();
    SetTargetFPS(60);

    // game loop
    while (!WindowShouldClose())
    {
        UpdateGame(); // update game elements
        DrawGame();   // draw next animation frame
    }

    // cleanup
    UnloadGame();  // release game resources
    CloseWindow(); // close game window

    return 0;
}

void UnloadGame()
{
}

void DrawGame()
{
    BeginDrawing();
    if (!onPause || t == 0)
    {
        ClearBackground(BLACK);
        DrawSplineBezierQuadratic(points, 3, 5, GREEN);

        DrawLineEx(points[0], points[1], 2, GRAY);
        DrawLineEx(points[1], points[2], 2, GRAY);

        DrawCircle(points[0].x, points[0].y, 5, RED);
        DrawCircle(points[1].x, points[1].y, 5, RED);
        DrawCircle(points[2].x, points[2].y, 5, RED);

        DrawLineEx((Vector2){line_1.x, line_1.y}, (Vector2){line_2.x, line_2.y}, 5, DARKBROWN);

        DrawCircle(line_1.x, line_1.y, 5, BLUE);
        DrawCircle(line_2.x, line_2.y, 5, BLUE);

        DrawCircle(curve.x, curve.y, 5, PURPLE);
    }

    EndDrawing();
}

void InitGame()
{
    points = calloc(3, sizeof(Vector2));
    points[0] = (Vector2){20, 40};
    points[1] = (Vector2){300, 400};
    points[2] = (Vector2){700, 100};
}

void UpdateGame()
{
    if (IsKeyPressed(KEY_P))
    {
        onPause = !onPause;
    }

    if (IsKeyPressed(KEY_R))
    {
        t = 0;
    }

    // P = (1-t)**2 * P0 + 2*(1-t)*t * P1 + t**2 * P2
    curve.x = pow(1 - t, 2) * points[0].x + 2 * (1 - t) * t * points[1].x + pow(t, 2) * points[2].x;
    curve.y = pow(1 - t, 2) * points[0].y + 2 * (1 - t) * t * points[1].y + pow(t, 2) * points[2].y;

    // P = (1-t)*P0 + t*P1
    line_1.x = (1 - t) * points[0].x + t * points[1].x;
    line_1.y = (1 - t) * points[0].y + t * points[1].y;

    line_2.x = (1 - t) * points[1].x + t * points[2].x;
    line_2.y = (1 - t) * points[1].y + t * points[2].y;

    if (!onPause)
    {
        if (t < 1)
        {
            t += 0.2f / 60;
            if (t > 1)
            {
                t = 1;
            }
            printf("t = %f\n", t);
        }
    }
}
