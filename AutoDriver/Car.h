#pragma once
#include "Point.h"
#include "config.h"
#include <SDL2/SDL.h>  

class Car{

    public:
    int speed, baseSpeed;
    double angle;
    Point position;
    SDL_Color frontSensor[FRONT_SENSOR_DEPTH][SIZE_OF_SENSOR];
    Car();
    Car(double x, double y, int angle);
    void drive(Car carArray[NUMBER_OF_CARS]);
    void goForward();
    void turnCar(double turn);
    double calculateTurn();
    double PID(double input, double KP, double KI, double KD);
    void scanSensor(SDL_Surface* surface, SDL_Color SensorBuffer[SIZE_OF_SENSOR], double distance);
    Point getPointToward(double distance);
    void createDistanceFromCars(Car carArray[], int numberOfCar);

};