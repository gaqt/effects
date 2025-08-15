#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <map>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <vector>

#include "sources.cpp"
using namespace std;

#define WIDTH 600
#define HEIGHT 600
#define GRAVITY 0.000002
#define MAX_DOTS 100

struct Attractor {
   Vector2 pos;
   float radius;
};

uint64_t dotCount = 0;

struct Dot {
   Vector2 pos;
   Vector2 velocity;
   size_t id;
   Color color;

   static Dot random(Attractor &attractor) {
      float angle = (float)rand() / (RAND_MAX / (2.0f * M_PI));
      float dist = 200.0f + (float)rand() / (RAND_MAX / 100.0f);

      Vector2 pos = attractor.pos + Vector2Rotate({dist, 0.0f}, angle);
      Vector2 velocity = {(float)rand() / (RAND_MAX * 4.0f) - 0.125f,
                          (float)rand() / (RAND_MAX * 4.0f) - 0.125f};
      velocity += Vector2Rotate(
          Vector2Scale(Vector2Normalize(attractor.pos - pos), 0.6f),
          M_PI / 2.0f);

      Color color = {(unsigned char)(rand() % 255),
                     (unsigned char)(rand() % 255),
                     (unsigned char)(rand() % 255), 255};

      if (dotCount == ULLONG_MAX)
         dotCount = 0;

      return {pos, velocity, dotCount++, color};
   }
};

struct Path {
   vector<pair<Vector2, Color>> points;
   size_t start;
   bool decay;
};

int main() {
   srand(time(NULL));

   SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
   InitWindow(WIDTH, HEIGHT, "Effects");
   SetTargetFPS(60);

   Attractor attractor = {{WIDTH / 2.0f, HEIGHT / 2.0f}, 50.0f};
   vector<Dot> dots;
   map<size_t, Path> paths;
   Texture2D dotTexture = LoadTexture("resources/dot.png");
   // Texture2D trailTexture = LoadTexture("resources/trail.png");

   while (!WindowShouldClose()) {
      if (dots.size() < MAX_DOTS) {
         Dot dot = Dot::random(attractor);
         paths[dot.id] = {{{dot.pos, {}}}, 0, false};
         dots.push_back(dot);
      }

      for (auto &dot : dots) {
         Vector2 diff = attractor.pos - dot.pos;
         float dist = Vector2Distance(attractor.pos, dot.pos);
         diff = Vector2Scale(diff, dist * GRAVITY);
         dot.velocity += diff;
         dot.pos += dot.velocity;
         if (dist < attractor.radius) {
            paths[dot.id].decay = true;
            dot = Dot::random(attractor);
            paths[dot.id] = {{}, 0, false};
         }

         dot.color.a =
             max(0.0f,
                 -0.06f * powf((dist - attractor.radius - 70.0f), 2.0f) + 250);

         paths[dot.id].points.push_back({dot.pos, dot.color});
      }

      vector<size_t> toDelete;
      for (auto &[idx, path] : paths) {
         if (!path.decay)
            continue;
         if (path.start >= path.points.size())
            toDelete.push_back(idx);
         else
            path.start += 3;
      }
      for (auto idx : toDelete)
         paths.erase(idx);

      BeginDrawing();
      ClearBackground(BLACK);
      for (auto &[idx, path] : paths) {
         if (path.start >= path.points.size())
            continue;
         auto prev = path.points[path.start];
         for (size_t i = path.start + 1; i < path.points.size(); i++) {
            auto crr = path.points[i];
            DrawLineV(prev.first, crr.first, prev.second);
            prev = crr;
         }
      }
      for (auto &dot : dots) {
         DrawTexture(dotTexture, dot.pos.x - dotTexture.width / 2.0f,
                     dot.pos.y - dotTexture.height / 2.0f, dot.color);
      }
      EndDrawing();
   }

   CloseWindow();

   return 0;
}
