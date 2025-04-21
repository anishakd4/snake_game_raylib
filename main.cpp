#include <iostream>
#include "raylib.h"
#include <deque>
#include <raymath.h>

using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 20;

double lastUpdateTime = 0;

bool eventTriggered(double interval){
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval){
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

bool elementinDeque(Vector2 element, deque<Vector2> deq){
    for(unsigned int i=0; i<deq.size(); i++){
        if(Vector2Equals(deq[i], element)){
            return true;
        }
    }
    return false;
}

class Snake{
    public: 
        deque<Vector2> body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };
        Vector2 direction = Vector2{1, 0};

        void Draw(){
            for(int i=0; i< body.size(); i++){
                int x = body[i].x;
                int y = body[i].y;
                Rectangle segment = Rectangle{ (float)x*cellSize, (float)y*cellSize, (float)cellSize, (float)cellSize };
                DrawRectangleRounded(segment, 0.5, 6, darkGreen);
            }
        }

        void update(){
            body.pop_back();
            body.push_front(Vector2Add(body[0] , direction));
        }
};

class Food {
    public:
        Vector2 position;
        Texture2D texture;

        Food(deque<Vector2> snakeBody){
            position = generateRandomPos(snakeBody);
            Image image = LoadImage("../Graphics/food.png");
            texture = LoadTextureFromImage(image);
            UnloadImage(image);
        }

        ~Food(){
            UnloadTexture(texture);
        }

        void Draw(){
            // DrawRectangle(position.x * cellSize, position.y * cellSize, cellSize, cellSize, darkGreen);
            DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
        }

        Vector2 generateRandomCell(){
            float x = GetRandomValue(0, cellCount - 1);
            float y = GetRandomValue(0, cellCount - 1);
            return Vector2{x, y};
        }

        Vector2 generateRandomPos(deque<Vector2> snakeBody){
            Vector2 position = generateRandomCell();
            while(elementinDeque(position, snakeBody)){
                position = generateRandomCell();
            }
            return position;
        }
};

class Game {
    public:
        
        Snake snake = Snake();
        Food food = Food(snake.body);

        void Draw(){

            food.Draw();
            snake.Draw();
        }

        void Update(){
            
            if(eventTriggered(0.2)){
                snake.update();
            }

            if(IsKeyPressed(KEY_UP) && snake.direction.y != 1){
                snake.direction = {0, -1};
            }else if(IsKeyPressed(KEY_DOWN) && snake.direction.y != -1){
                snake.direction = {0, 1};
            }else if(IsKeyPressed(KEY_LEFT) && snake.direction.x != 1){
                snake.direction = {-1, 0};
            }else if(IsKeyPressed(KEY_RIGHT) && snake.direction.x != -1){
                snake.direction = {1, 0};
            }

            CheckCollisionWithFood();
        }

        void CheckCollisionWithFood(){
            if(Vector2Equals(snake.body[0], food.position)){
                food.position = food.generateRandomPos(snake.body);
            }
        }
};

int main(){
    InitWindow(cellSize * cellCount, cellSize * cellCount, "Retro Snake");
    SetTargetFPS(60);

    Game game = Game();

    while(WindowShouldClose() == false){
        game.Update();

        BeginDrawing();

        ClearBackground(green);
        
        game.Draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}