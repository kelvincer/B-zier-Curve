#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include "raymath.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define NUM_POINTS 4
#define FONT_SIZE 25
#define TITLE "CUBIC BÉZIER"

static bool onPause = false;
static float t = 0.0f;
Vector2 *points = NULL;
Vector2 curve;
Vector2 line_1;
Vector2 line_2;
Vector2 line_3;
Vector2 line_4;
Vector2 line_5;
int currentGesture;
Vector2 touchPosition;

void UnloadGame();
void DrawGame();
void InitGame();
void UpdateGame();

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
    InitGame();
    SetTraceLogLevel(LOG_DEBUG);
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
        ClearBackground(DARKBROWN);

        DrawSplineBezierCubic(points, NUM_POINTS, 5, PINK);

        DrawLineEx(*(points), *(points + 1), 2, GRAY);
        // DrawLineBezier(*(points), *(points + 1), 2, GRAY);
        DrawLineEx(*(points + 1), *(points + 2), 2, GRAY);
        DrawLineEx(*(points + 2), *(points + 3), 2, GRAY);

        DrawCircle((*points).x, (*points).y, 5, RED);
        DrawCircle((*(points + 1)).x, (*(points + 1)).y, 5, RED);
        DrawCircle((*(points + 2)).x, (*(points + 2)).y, 5, RED);
        DrawCircle((*(points + 3)).x, (*(points + 3)).y, 5, RED);

        DrawLineEx((Vector2){line_1.x, line_1.y}, (Vector2){line_2.x, line_2.y}, 5, WHITE);
        DrawLineEx((Vector2){line_2.x, line_2.y}, (Vector2){line_3.x, line_3.y}, 5, WHITE);
        DrawLineEx((Vector2){line_4.x, line_4.y}, (Vector2){line_5.x, line_5.y}, 5, GREEN);

        DrawCircle(line_1.x, line_1.y, 5, BLUE);
        DrawCircle(line_2.x, line_2.y, 5, BLUE);
        DrawCircle(line_3.x, line_3.y, 5, BLUE);
        DrawCircle(line_4.x, line_4.y, 5, ORANGE);
        DrawCircle(line_5.x, line_5.y, 5, ORANGE);

        DrawCircleGradient(curve.x, curve.y, 10, GRAY, BLUE);

        DrawText(TITLE, SCREEN_WIDTH - MeasureText(TITLE, FONT_SIZE) - 10, SCREEN_HEIGHT - FONT_SIZE, FONT_SIZE, ORANGE);
    }
    EndDrawing();
}

void InitGame()
{
    points = calloc(NUM_POINTS, sizeof(Vector2));
    points[0] = (Vector2){10, 400};
    points[1] = (Vector2){100, 40};
    points[2] = (Vector2){450, 230};
    points[3] = (Vector2){750, 30};
}

void UpdateGame()
{
    currentGesture = GetGestureDetected();
    touchPosition = GetTouchPosition(0);

    for (size_t i = 0; i < NUM_POINTS; i++)
    {
        if (CheckCollisionPointCircle(touchPosition, points[i], 50))
        {
            if (currentGesture == GESTURE_DRAG)
            {
                *(points + i) = touchPosition;
                break;
            }
        }
    }

    if (IsKeyPressed(KEY_P))
    {
        onPause = !onPause;
    }

    if (IsKeyPressed(KEY_R))
    {
        t = 0;
    }

    // P = (1-t)**3 * P0 + t*P1*(3*(1-t)**2) + P2*(3*(1-t)*t**2) + P3*t**3
    curve.x = pow(1 - t, 3) * (*points).x + t * (*(points + 1)).x * 3 * pow(1 - t, 2) + (*(points + 2)).x * 3 * (1 - t) * pow(t, 2) + (*(points + 3)).x * pow(t, 3);
    curve.y = pow(1 - t, 3) * (*points).y + t * (*(points + 1)).y * 3 * pow(1 - t, 2) + (*(points + 2)).y * 3 * (1 - t) * pow(t, 2) + (*(points + 3)).y * pow(t, 3);

    // P = (1-t)*P0 + t*P1
    line_1.x = (1 - t) * (*(points)).x + t * (*(points + 1)).x;
    line_1.y = (1 - t) * (*(points)).y + t * (*(points + 1)).y;

    line_2.x = (1 - t) * (*(points + 1)).x + t * (*(points + 2)).x;
    line_2.y = (1 - t) * (*(points + 1)).y + t * (*(points + 2)).y;

    line_3.x = (1 - t) * (*(points + 2)).x + t * (*(points + 3)).x;
    line_3.y = (1 - t) * (*(points + 2)).y + t * (*(points + 3)).y;

    line_4.x = (1 - t) * line_1.x + t * line_2.x;
    line_4.y = (1 - t) * line_1.y + t * line_2.y;

    line_5.x = (1 - t) * line_2.x + t * line_3.x;
    line_5.y = (1 - t) * line_2.y + t * line_3.y;

    if (!onPause)
    {
        if (t < 1)
        {
            t += 0.1 / 60;
            if (t > 1)
            {
                t = 1;
            }
            TraceLog(LOG_DEBUG, "t: %f", t);
        }
    }
}