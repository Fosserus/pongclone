#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

// used instead of constants because??? 
#define WIDTH 720
#define HEIGHT 720
#define FONT_SIZE 32

#define SIZE 16
#define BALL_SPEED 16
#define SPEED 9

#define PI 3.14159265358979323846

SDL_Renderer* renderer;
SDL_Window* window;
TTF_Font* font;
SDL_Color color;
bool isRunning;
int frameCount, timerFPS, lastFrame, fps;

SDL_Rect lPaddle, rPaddle, ball, scoreBoard;
float velX, velY;
std::string score;
int lScore, rScore;
bool turn;

void serve() {
       lPaddle.y = rPaddle.y = (HEIGHT / 2) - (lPaddle.h / 2);
        if (turn) { // if it's player 1's turn 
            ball.x = lPaddle.x + (lPaddle.w * 4); // reset ball
            velX = BALL_SPEED / 2; // reset ball's velocity

        } else {
            ball.x = rPaddle.x - (rPaddle.w * 4);
            velX = BALL_SPEED / 2;

        }
            velY = 0;
            ball.y = HEIGHT / 2 - (SIZE / 2);
            turn = !turn; // switch turn
    }

void write(std::string text, int x, int y) { // function basically renders text to screen
    
    SDL_Surface* surface;
    SDL_Texture* texture;
    
    if (font == NULL) {
        std::cout << "ERROR: font not found" << std::endl;
        }
        
    const char* t = text.c_str(); // char for score
    surface = TTF_RenderText_Solid(font, t, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    scoreBoard.w = surface->w;
    scoreBoard.h = surface->h;
    scoreBoard.x = x - scoreBoard.w;
    scoreBoard.y = y - scoreBoard.h;
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &scoreBoard);
    SDL_DestroyTexture(texture);
    }

void update() { 
      if (SDL_HasIntersection(&ball, &rPaddle)) { // collision detection
        double rel = (rPaddle.y + (rPaddle.h / 2)) - (ball.y + (SIZE / 2));
        double norm = rel / (rPaddle.h / 2);
        double bounce  = norm * (5 * PI / 12);
        velX = -BALL_SPEED * cos(bounce);
        velY = BALL_SPEED * -sin(bounce);
        }
        
    if (SDL_HasIntersection(&ball, &lPaddle)) {
        double rel = (lPaddle.y + (lPaddle.h / 2)) - (ball.y + (SIZE / 2));
        double norm = rel / (lPaddle.h / 2);
        double bounce  = norm * (5 * PI / 12);
        velX = BALL_SPEED * cos(bounce);
        velY = BALL_SPEED * -sin(bounce);
        }
        
   

    if (ball.y > rPaddle.y + (rPaddle.h / 2)) { // handling paddle tracking
         rPaddle.y += SPEED;
         }  

    if (ball.y < rPaddle.y + (rPaddle.h / 2)) {
         rPaddle.y -= SPEED;
         } 
         
     // ball physics
    if (ball.x <= 0) { // if it touches the left wall
        rScore++;
        serve();
        }
    if (ball.x + SIZE >= WIDTH) {
        lScore++;
        serve();
        }
    if (ball.y <=0 || ball.y + SIZE >= HEIGHT) {
        velY = -velY;
        }
        
        ball.x += velX;
        ball.y += velY;
    
    score = std::to_string(lScore) + "   " + std::to_string(rScore); // create score
    // creating boundaries for screen
    if (lPaddle.y < 0) {
        lPaddle.y = 0;
        }
    if (lPaddle.y + lPaddle.h > HEIGHT) {
        lPaddle.y = HEIGHT - lPaddle.h;
        } 
        
    if (rPaddle.y < 0) {
        rPaddle.y = 0;
        }
    if (rPaddle.y + rPaddle.h > HEIGHT) {
        rPaddle.y = HEIGHT - rPaddle.h;
        }    

    }
    
    
    
void input() {
    SDL_Event e; // keeps track of events in game
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    while (SDL_PollEvent (&e)) { // handling input
        if (e.type == SDL_QUIT) {
            isRunning = false;
            }
        }
        if (keystates[SDL_SCANCODE_ESCAPE]) {
        isRunning = false;
        }
        if (keystates[SDL_SCANCODE_UP]) {
        lPaddle.y -= SPEED;
        }
        if (keystates[SDL_SCANCODE_DOWN]) {
        lPaddle.y += SPEED;
        }
    }
    
        

void render() {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 255); // background to black
    SDL_RenderClear(renderer); // fill screen with color
    
    frameCount++; // frame handler
    timerFPS = SDL_GetTicks() - lastFrame;
    if (timerFPS < (1000 / 60)) {
        SDL_Delay((1000 / 60) - timerFPS);
        }
        
        
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255); // color to white (paddles)
    SDL_RenderFillRect(renderer, &lPaddle); // filling game objects
    SDL_RenderFillRect(renderer, &rPaddle);
    SDL_RenderFillRect(renderer, &ball);
    write(score, WIDTH / 2 + FONT_SIZE, FONT_SIZE * 2); // display score 
    
    SDL_RenderPresent(renderer); // render to screen
    
    }

int main() {
    // check for errors
   if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
       std::cout << "Failed at SDL_Init()" << std::endl;
       } 
   if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer) < 0) {
               std::cout << "Failed at SDL_CreateWindowAndRenderer()" << std::endl;
        }
        
    // initialize font
   TTF_Init();
   font = TTF_OpenFont("Peepo.ttf", FONT_SIZE);
   isRunning = true;
   
   static int lastTime = 0;
   color.r = color.g = color.b = 255;
   
   // initializing dimensions
   lScore = rScore = 0;
   lPaddle.x = 32;
   lPaddle.h = HEIGHT / 4;
   lPaddle.y = (HEIGHT / 2) - (lPaddle.h / 2);
   lPaddle.w = 12;
   rPaddle = lPaddle;
   rPaddle.x = WIDTH - rPaddle.w - 32;
   ball.w = ball.h = SIZE;
   
   serve();
   
   // main game loop
   while (isRunning) {
       lastFrame = SDL_GetTicks();
       if (lastFrame >= (lastTime + 1000)) {
           lastTime = lastFrame;
           fps = frameCount;
           frameCount = 0;
           }
       
       update();
       input();
       render();
       }
       TTF_CloseFont(font);
       SDL_DestroyRenderer(renderer);
       SDL_DestroyWindow(window);
       SDL_Quit();
    }