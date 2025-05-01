#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Sprites
const SDL_FRect BALL_SRC     = {800, 548, 87, 77};
const SDL_FRect PADDLE_SRC   = {392, 9, 161, 55};

const SDL_FRect BRICK_BLUE       = {21, 17, 171, 59};
const SDL_FRect BRICK_BLUE_DMG   = {209, 16, 171, 60};
const SDL_FRect BRICK_PURPLE     = {20, 169, 168, 57};
const SDL_FRect BRICK_PURPLE_DMG = {208, 168, 170, 58};
const SDL_FRect BRICK_YELLOW     = {20, 469, 169, 59};
const SDL_FRect BRICK_YELLOW_DMG = {210, 470, 166, 63};
const SDL_FRect BRICK_ORANGE     = {17, 319, 175, 57};
const SDL_FRect BRICK_ORANGE_DMG = {206, 318, 175, 58};

const SDL_FRect LASER_SPRITE     = {837, 643, 11, 22};

enum BrickState { NORMAL, DAMAGED, DESTROYED };

struct Brick {
    SDL_FRect dest;
    const SDL_FRect* spriteNormal;
    const SDL_FRect* spriteBroken;
    BrickState state = NORMAL;
    int hitTimer = 0;
    bool animate = false;
};

struct LaserBeam {
    SDL_FRect dest;
    float speedY = -5.0f;
    bool active = true;
};

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_CreateWindowAndRenderer("Breakout POC", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    SDL_Texture* sheet = IMG_LoadTexture(renderer, "res/breakout.png");

    SDL_FRect ballDest   = {100.0f, 100.0f, 50.0f, 50.0f};
    SDL_FRect paddleDest = {300.0f, 550.0f, 128.0f, 32.0f};
    float ballVX = 2.5f, ballVY = -2.0f;
    float paddleDir = 1.0f;

    const int ROWS = 6;
    const int COLS = 5;
    const float spacing = 10.0f;
    const float brickW = 128.0f;
    const float brickH = 32.0f;

    std::vector<Brick> bricks;
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            Brick b;
            b.dest = {
                50.0f + col * (brickW + spacing),
                80.0f + row * (brickH + spacing),
                brickW,
                brickH
            };

            // הגדרת צבע לכל שורה
            if (row == 0) {
                b.spriteNormal = &BRICK_BLUE;
                b.spriteBroken = &BRICK_BLUE_DMG;
                b.animate = false;
            } else if (row == 1) {
                b.spriteNormal = &BRICK_PURPLE;
                b.spriteBroken = &BRICK_PURPLE_DMG;
                b.animate = false;
            } else if (row == 2) {
                b.spriteNormal = &BRICK_YELLOW;
                b.spriteBroken = &BRICK_YELLOW_DMG;
                b.animate = false;
            } else if (row == 3) {
                b.spriteNormal = &BRICK_ORANGE;
                b.spriteBroken = &BRICK_ORANGE_DMG;
                b.animate = true;
            } else {
                b.spriteNormal = &BRICK_PURPLE;
                b.spriteBroken = &BRICK_PURPLE_DMG;
                b.animate = true;
            }

            bricks.push_back(b);
        }
    }

    // לייזרים
    std::vector<LaserBeam> lasers;
    int laserFireTimer = 0;
    const int LASER_FIRE_DELAY = 60;

    for (int frame = 0; frame < 1200; ++frame) {
        ballDest.x += ballVX;
        ballDest.y += ballVY;
        if (ballDest.x < 0 || ballDest.x + ballDest.w > SCREEN_WIDTH) ballVX *= -1;
        if (ballDest.y < 0 || ballDest.y + ballDest.h > SCREEN_HEIGHT) ballVY *= -1;

        paddleDest.x += paddleDir * 3;
        if (paddleDest.x < 0 || paddleDest.x + paddleDest.w > SCREEN_WIDTH) paddleDir *= -1;

        // ירי לייזר
        laserFireTimer++;
        if (laserFireTimer >= LASER_FIRE_DELAY) {
            laserFireTimer = 0;
            float xLeft = paddleDest.x + 10;
            float xRight = paddleDest.x + paddleDest.w - 10 - LASER_SPRITE.w;
            float y = paddleDest.y;

            lasers.push_back({{xLeft, y, LASER_SPRITE.w, LASER_SPRITE.h}});
            lasers.push_back({{xRight, y, LASER_SPRITE.w, LASER_SPRITE.h}});
        }

        for (LaserBeam& l : lasers) {
            if (!l.active) continue;
            l.dest.y += l.speedY;
            if (l.dest.y + l.dest.h < 0)
                l.active = false;
        }

        SDL_RenderClear(renderer);

        // כדור ומחבט
        SDL_RenderTexture(renderer, sheet, &BALL_SRC, &ballDest);
        SDL_RenderTexture(renderer, sheet, &PADDLE_SRC, &paddleDest);

        // לבנים
        for (Brick& b : bricks) {
            if (b.animate && b.state != DESTROYED) {
                b.hitTimer++;
                if (b.hitTimer > 45 && b.state == NORMAL) {
                    b.state = DAMAGED;
                    b.hitTimer = 0;
                } else if (b.hitTimer > 46 && b.state == DAMAGED) {
                    b.state = DESTROYED;
                }
            }
            if (b.state == DESTROYED) continue;
            const SDL_FRect* sprite = (b.state == DAMAGED) ? b.spriteBroken : b.spriteNormal;
            SDL_RenderTexture(renderer, sheet, sprite, &b.dest);
        }

        // לייזרים
        for (LaserBeam& l : lasers) {
            if (l.active)
                SDL_RenderTexture(renderer, sheet, &LASER_SPRITE, &l.dest);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(sheet);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
