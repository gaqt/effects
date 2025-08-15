#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "sources.cpp"
using namespace std;

#define WIDTH 600
#define HEIGHT 600
#define GRAVITY 0.0000005

struct Dot {
   Vector2 pos;
   Vector2 velocity;
   Color color;

   static Dot random() {
      Vector2 pos = {(float)(rand() % (WIDTH - 300) + 150),
                     (float)(rand() % (HEIGHT - 300) + 150)};
      Vector2 velocity = {(float)rand() / (RAND_MAX / 2.0f) - 1.0f,
                          (float)rand() / (RAND_MAX / 2.0f) - 1.0f};
      Color color = {(unsigned char)(rand() % 255),
                     (unsigned char)(rand() % 255),
                     (unsigned char)(rand() % 255), 255};

      return {pos, velocity, color};
   }
};

struct Attractor {
   Vector2 pos;
   float radius;
};

int main() {
   InitWindow(WIDTH, HEIGHT, "Hello World");
   SetTargetFPS(60);

   srand(time(NULL));
   vector<Dot> dots;
   for (int i = 0; i < 200; i++) {
      dots.push_back(Dot::random());
   }

   Attractor attractor = {{WIDTH / 2.0f, HEIGHT / 2.0f}, 50.0f};

   Texture2D dotTexture = LoadTexture("resources/dot.png");

   while (!WindowShouldClose()) {
      for (auto &dot : dots) {
         Vector2 diff = attractor.pos - dot.pos;
         float dist = Vector2Distance(attractor.pos, dot.pos);
         diff = Vector2Scale(diff, dist * GRAVITY);
         dot.velocity += diff;
         dot.pos += dot.velocity;
         if (dist < attractor.radius) {
            dot = Dot::random();
         }

         dot.color.a =
             max(0.0f,
                 -0.06f * powf((dist - attractor.radius - 70.0f), 2.0f) + 250);
      }

      BeginDrawing();
      ClearBackground(BLACK);
      for (auto &dot : dots) {
         DrawTexture(dotTexture, dot.pos.x, dot.pos.y, dot.color);
      }
      EndDrawing();
   }

   CloseWindow();

   return 0;
}
