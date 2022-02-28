
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <SDL2/SDL.h>  
#include "config.h"
#include "Car.h"
#include "ColorDecoder.h"
#include "Test.h"

static uint32_t nextTime;
static uint32_t frame = 0;

SDL_Surface* image;
SDL_Window* myWindow{ nullptr }; 
SDL_Renderer* myRenderer{ nullptr }; 

SDL_Window* sensorWindow{ nullptr }; 
SDL_Renderer* sensorRenderer{ nullptr }; 

Car* carArray = new Car[NUMBER_OF_CARS];

uint32_t timeLeft(){
    uint32_t now = SDL_GetTicks();
    if (nextTime <= now){
        return 0;
    }
    else {
        return nextTime - now;
    }
}

void renderState(SDL_Renderer* render, SDL_Texture* background){

    //Main window
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderClear(render);
    SDL_RenderCopy(render, background, NULL, NULL);

    //Sensor Window
    SDL_SetRenderDrawColor(sensorRenderer, 0, 0, 0, 255);
    SDL_RenderClear(sensorRenderer);

    for(int i=0; i<NUMBER_OF_CARS; i++){

        //Rect
        SDL_SetRenderDrawColor(render, 255, 255* (1 - (double)abs(carArray[i].baseSpeed - carArray[i].speed)/(double)carArray[i].baseSpeed), 0, 255);
        SDL_Rect carRect = {(int)carArray[i].position.x - 5 ,(int)carArray[i].position.y - 5, 10, 10};
        SDL_RenderFillRect(render, &carRect);

        //Line
        Point target = carArray[i].getPointToward(TARGET_LINE_LENGTH);
        SDL_RenderDrawLine(render, (int)carArray[i].position.x ,(int)carArray[i].position.y, (int)target.x, (int)target.y);


        //Road Sensor
        for(int j=0; j<SIZE_OF_SENSOR; j++){


            for(int z=0; z<FRONT_SENSOR_DEPTH; z++){

                SDL_SetRenderDrawColor(sensorRenderer, carArray[i].frontSensor[z][j].r,carArray[i].frontSensor[z][j].g,carArray[i].frontSensor[z][j].b, 255);
                SDL_Rect frontColorRect = {j*SW_ZOOM,(FRONT_SENSOR_DEPTH-z)*SW_ZOOM, SW_ZOOM, SW_ZOOM};
                SDL_RenderFillRect(sensorRenderer, &frontColorRect);

            }

/*             SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
            SDL_RenderDrawPoint(render, tmpArray1[j][0], tmpArray1[j][1]);

            SDL_SetRenderDrawColor(render, 0, 150, 10, 255);
            SDL_RenderDrawPoint(render, tmpArray2[j][0], tmpArray2[j][1]);
 */
        }

    }

    SDL_RenderPresent(render);
    SDL_RenderPresent(sensorRenderer);
    frame++;
}

void updateState(){

    for(int i=0; i<NUMBER_OF_CARS; i++){
        
        //Color under Car
        SDL_Color Red = {255, 0, 0};
        SDL_Color Green = {0, 255, 0};
        SDL_Color Black = {0, 0, 0};
        SDL_Color Gray = {100, 100, 100};
        SDL_Color carRoadColor = decodeColor(image, carArray[i].position.x, carArray[i].position.y);

        //Start line
        if(compareColor(carRoadColor, Red)){
            carArray[i].drive(carArray);
        }

        //Finish line
        else if (compareColor(carRoadColor, Green)){
            printf("The car has reached the finish line\n");
            exit(0);
        }

        //Car crash
        else if(compareColor(carRoadColor, Gray)){
            printf("Car has crashed\n");
            exit(1);
        }

        //Off Road
        else if(compareColor(carRoadColor, Black)){
            printf("Car went Offroad\n");
            exit(1);
        }

        else{

            for(int j=0; j<FRONT_SENSOR_DEPTH; j++){
                carArray[i].scanSensor(image, carArray[i].frontSensor[j], FRONT_SENSOR_DISTANCE+j);
            }
            carArray[i].drive(carArray);
            
        }


    }

}

void init(){

    for(int i=0; i<NUMBER_OF_CARS; i++){
/*
        carArray[i].position.x = 5;
        carArray[i].position.y = 51+i*100;
        carArray[i].baseSpeed = 5;
        carArray[i].angle = 0;
 */

         carArray[i].baseSpeed = 30;
        carArray[i].speed = carArray[i].baseSpeed;
        carArray[i].angle = 0;
        carArray[i].position.x = 5 + (rand()%50);
        carArray[i].position.y = 30 + (rand()%50);
    }

}

int main(int argc, char* argv[]) 
{      

    //Testing everything
    testSequence();

   //Init every SDL Comp
    if (SDL_Init(SDL_INIT_VIDEO) < 0)  
    {         
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s\n", SDL_GetError());         
        return EXIT_FAILURE;     
    }       
    
    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);

    if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_SHOWN, &myWindow, &myRenderer)<0)     
    {         
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s\n", SDL_GetError());         
        SDL_Quit();         
        return EXIT_FAILURE;     
    }
    SDL_SetRenderDrawBlendMode(myRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetWindowTitle(myWindow, "Auto Driver");
    SDL_SetWindowPosition(myWindow, (screen.w-WIDTH)/2, (screen.h-HEIGHT)/2);
    
    if (SDL_CreateWindowAndRenderer(SIZE_OF_SENSOR*SW_ZOOM, FRONT_SENSOR_DEPTH*SW_ZOOM, SDL_WINDOW_SHOWN, &sensorWindow, &sensorRenderer)<0)
    {         
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s\n", SDL_GetError());         
        SDL_Quit();         
        return EXIT_FAILURE;
    }
    SDL_SetRenderDrawBlendMode(sensorRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetWindowTitle(sensorWindow, "Front Sensor");
    SDL_SetWindowPosition(sensorWindow, (screen.w + WIDTH)/2 + 20, (screen.h - HEIGHT)/2);


    //Loading Background
    image = SDL_LoadBMP("/home/tristan/Desktop/TS/Projet/Source/Road2.bmp");
    if (image == NULL) {
        SDL_GetError();
        printf("[DEBUG] > %s\n", SDL_GetError());
    }
    SDL_Texture *background = SDL_CreateTextureFromSurface(myRenderer, image);

    nextTime = SDL_GetTicks() + TICK_INTERVAL;
    SDL_Event events;
    bool isOpen{ true };
    bool isPaused = false;
    init();


    //LOOP
    while (isOpen)
    {
        
        //Inputs
        while (SDL_PollEvent(&events))
        {
            switch (events.type)
            {
            case SDL_QUIT:
                isOpen = false;
                break;
            case SDL_KEYDOWN:
                switch (events.key.keysym.sym)
                {
                case SDLK_q:
                    isOpen = false;
                    break;
                case SDLK_p:
                    if(!isPaused)isPaused = true;
                    else isPaused = false;
                    break;
                default:
                    break;
                }
            }
        }

        if (!isPaused){

            updateState();        
            renderState(myRenderer, background);

        }

        //Frame delay
        SDL_Delay(timeLeft());
        nextTime += TICK_INTERVAL;

    }

    //Destroy every SDL Comp   
    SDL_FreeSurface(image);
    SDL_DestroyTexture(background);
    SDL_DestroyRenderer(myRenderer);
    SDL_DestroyWindow(myWindow);     
    SDL_DestroyRenderer(sensorRenderer);
    SDL_DestroyWindow(sensorWindow);     
    SDL_Quit();     
    return EXIT_SUCCESS; 
}