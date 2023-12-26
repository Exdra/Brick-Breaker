
#include "../include\raylib.h"
#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>

// defining the resloution of the window
typedef enum screen {
    WINDOWWIDTH = 900,
    WINDOWHEIGHT = 500
}screen;

typedef struct Brick {
    Rectangle body;
    Color color;
}Brick;

void initGrid(Brick** grid,int* gridSize,int x,int y){
    for (int i = 0; i < *gridSize; i++) {

        (*grid)[i].body.x = x;
        (*grid)[i].body.y = y;
        (*grid)[i].body.width = 45;
        (*grid)[i].body.height = 25;

        if (x >= (WINDOWWIDTH - 90)) {
            y = y+36;
            x = 35;
        }
        else {
            x += 56;
        }
        
    }
    for (int i = 0; i < *gridSize; i++) {
        switch (GetRandomValue(1,6)) {
            case 1:
                (*grid)[i].color = RED;
                break;
            case 2:
                (*grid)[i].color = ORANGE;
                break;
            case 3:
                (*grid)[i].color = YELLOW;
                break;
            case 4:
                (*grid)[i].color = GREEN;
                break;
            case 5:
                (*grid)[i].color = BLUE;
                break;
            case 6:
                (*grid)[i].color = VIOLET;
                break;
        }
    }
};

int searchBrick(Brick** grid,int* size,Rectangle element){
    for (int i = 0; i < *size; i++) {
        if (element.x == (*grid)[i].body.x && element.y == (*grid)[i].body.y) {
            return i;
        }
    }
    return -1;
};

void removeBrick(Brick** grid,Rectangle element,int* size){
    int elementLocation = searchBrick(grid,size,element);
    for (int i = elementLocation; i < *size - 1; i++) {
        (*grid)[i]= (*grid)[i+1];
        (*grid)[i] = (*grid)[i+1];
    }
    *size -= 1;
};

int main() {

    int fps;
    int mouseX;
    int mouseY;
    float x = 35;
    float y = 35;
    bool hasGameEnded = false;
    int health = 3;
    Sound ballhit = LoadSound("assets/collide.wav");
    Rectangle player = {WINDOWWIDTH/2-32,400-4,64,8};
    Vector2 ball = {WINDOWWIDTH/2,WINDOWHEIGHT/2};
    Vector2 ballVel = {128,128};
    Brick* grid = (Brick*) malloc (90 * sizeof (Brick));
    int gridSize = 90;

    initGrid(&grid,&gridSize,x,y);
    InitAudioDevice();
    InitWindow(WINDOWWIDTH, WINDOWHEIGHT, "Brick Breaker");
    HideCursor();
    SetTargetFPS(60);
    // main game loop
    while (!WindowShouldClose()) {
        fps = GetFPS();
        if (hasGameEnded == 0) {
            mouseX = GetMouseX();
            player.x = mouseX;
            ball.x += ballVel.x * GetFrameTime();
            ball.y += ballVel.y * GetFrameTime();
        }

        if (CheckCollisionCircleRec(ball,8,player)) {
            if (ball.x > player.x+player.width/2 / 2 && ballVel.x < 0){
                ballVel.x *= -1;
            }
            if (ball.x < player.x+player.width/2 && ballVel.x > 0){
                ballVel.x *= -1;
            }
            PlaySound(ballhit);
            ballVel.y *= -1;
        } 
        if (ball.x+8 >= WINDOWWIDTH){
            ballVel.x *= -1;
            PlaySound(ballhit);
        } 
        if (ball.x-8 <= 0){
            ballVel.x *= -1;
            PlaySound(ballhit);
        } 
        if (ball.y-8 <= 0){
            ballVel.y *= -1;
            PlaySound(ballhit);
        } 
        if (ball.y+8 >= WINDOWHEIGHT){ 
            ball.x = WINDOWWIDTH / 2;
            ball.y = WINDOWHEIGHT / 2;
            health -= 1;
        }

        BeginDrawing();

        ClearBackground(BLACK);
        DrawText(TextFormat("FPS: %d", fps),15, 15, 20,LIME);
        DrawText(TextFormat("Health: %d", health),WINDOWWIDTH - 115, 15, 20,RED);
        DrawRectangleRec(player,WHITE);
        DrawCircleV(ball,8,BLUE);
        for (int i = 0; i < gridSize; i++) {
            DrawRectangleRec(grid[i].body,grid[i].color);
            if (CheckCollisionCircleRec(ball,8,grid[i].body)){
                ballVel.y *= -1;
                removeBrick(&grid,grid[i].body,&gridSize);
                PlaySound(ballhit);
            }
        }

        if (gridSize <= 0) {
            hasGameEnded = true;
            DrawText("YOU WON",WINDOWHEIGHT/2,WINDOWWIDTH/2,20,GREEN);
        }
        if (health == 0) {
            hasGameEnded = true;
            DrawText("YOU LOST",WINDOWHEIGHT/2,WINDOWWIDTH/2,20,RED);
        }
        if (IsKeyPressed(KEY_R)){
            health = 3;
            gridSize = 90;
            x = 35;
            y = 35;
            ball.x = WINDOWWIDTH/2;
            ball.y = WINDOWHEIGHT/2;
            initGrid(&grid,&gridSize,x,y);
            hasGameEnded = false;
        }
        
        EndDrawing();
    }
}
