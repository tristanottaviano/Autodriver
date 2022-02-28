#include "Car.h"
#include <math.h>
#include<stdio.h>
#include "ColorDecoder.h"

double PIDLastError;
double PIDiError;

Car::Car(){
    this->position = Point();
    this->angle = 0;
    this->speed = 0;
    this->baseSpeed = 0;
}

Car::Car(double x, double y, int angle){
    this->position = Point(x,y);
    this->angle = angle;
    this->speed = 0;
    this->baseSpeed = 0;
}

void Car::drive(Car carArray[NUMBER_OF_CARS]){

    double turnValue =0;

    goForward();
    turnValue += calculateTurn();
    turnCar(turnValue);
    createDistanceFromCars(carArray, NUMBER_OF_CARS);

}

Point Car::getPointToward(double distance){

    return Point(position.x + (cos(angle*M_PI/180) * distance), position.y + (sin(angle*M_PI/180) * distance));

}

void Car::goForward(){

    Point p = getPointToward(speed*0.15);
    position.x = p.x;
    position.y = p.y;

}

double Car::PID(double input, double KP, double KI, double KD){

    double error = 0.0 - input;
    PIDiError += error * 0.1;
    double dError = (error - PIDLastError)/ 0.1;

    double turn = KP * error + KI * PIDiError + KD * dError;
    input = turn;
    PIDLastError = error;
    return turn;

}

double Car::calculateTurn(){
    
    double R=0,G=0,B=0;
    double grayLeft=0, obsDist=0, grayRight=0;
    SDL_Color magenta = {255, 0, 255};
    SDL_Color cyan = {0, 255, 255};
    SDL_Color blue = {0, 0, 255};
    SDL_Color gray = {100, 100, 100};

    //Front Sensor
    for(int i=0; i<FRONT_SENSOR_DEPTH; i++){
        for(int j=0; j<SIZE_OF_SENSOR; j++){
            
            if(compareColor(frontSensor[i][j], magenta) || compareColor(frontSensor[i][j], cyan) || compareColor(frontSensor[i][j], blue)){
                R += frontSensor[i][j].r * ((FRONT_SENSOR_DEPTH - i)/(double)FRONT_SENSOR_DEPTH);
                G += frontSensor[i][j].g * ((FRONT_SENSOR_DEPTH - i)/(double)FRONT_SENSOR_DEPTH);
                B += frontSensor[i][j].b * ((FRONT_SENSOR_DEPTH - i)/(double)FRONT_SENSOR_DEPTH);
            }
 
            if(compareColor(frontSensor[i][j], gray)){
                if(j<SIZE_OF_SENSOR*2/5){
                    grayLeft+= ((FRONT_SENSOR_DEPTH - i)/(double)FRONT_SENSOR_DEPTH) * abs(SIZE_OF_SENSOR/2 - abs(SIZE_OF_SENSOR/2-j));
                }
                else if(j>SIZE_OF_SENSOR*3/5){
                    grayRight+= ((FRONT_SENSOR_DEPTH - i)/(double)FRONT_SENSOR_DEPTH) * abs(SIZE_OF_SENSOR/2 - abs(SIZE_OF_SENSOR/2-j));
                }
            }

            //Obstacle dist
            if(j>SIZE_OF_SENSOR*4/10 && j<SIZE_OF_SENSOR*6/10 && i<FULL_STOP_DISTANCE && obsDist==0){
                
                //If the car is going toward the sidewalk
                if(compareColor(frontSensor[i][j], magenta) || compareColor(frontSensor[i][j], cyan)){

                    obsDist = i;

                    if (i<FULL_STOP_DISTANCE/10 && speed>-baseSpeed*0.1){
                        //speed = baseSpeed * (1 - (double)abs(FULL_STOP_DISTANCE - obsDist)/(double)FULL_STOP_DISTANCE);
                        speed--;
                    }

                }
                
                //If there is an obstacle we adapt the speed
                else if (compareColor(frontSensor[i][j], gray)){
                
                    obsDist = i;
                
                    if (i<FULL_STOP_DISTANCE){
                        speed = baseSpeed * (1 - (double)abs(FULL_STOP_DISTANCE - obsDist)/(double)FULL_STOP_DISTANCE);
                    }
                    
                }
            }
        }
    }

    //If there are no obstacles, we speed up
    if (obsDist==0 && speed < baseSpeed){
        speed++;
    }

    //Calculate the turn value
    double turn;
    if(B!=0){
        turn = ((FRONT_SENSOR_DEPTH*0.6)*(grayRight - grayLeft) + R - G)/B;
    }

    else {
        turn = 0;
    }

    return turn;

}

void Car::turnCar(double turn){

    turn = PID(turn, 20.0, 2.0, 0.5);
    angle += turn;
    //printf("Speed: %d\t Angle:%lf\t TurnDiff:%lf\n", speed, angle, turn);

}

void Car::scanSensor(SDL_Surface* surface, SDL_Color sensorBuffer[SIZE_OF_SENSOR], double distance){

    Point target = getPointToward(distance);
    Point p1 = Point(target.x +  cos((angle*M_PI/180)-M_PI/2)*SIZE_OF_SENSOR/2, target.y + sin((angle*M_PI/180)-M_PI/2)*SIZE_OF_SENSOR/2);
    Point p2 = Point(target.x +cos((angle*M_PI/180)+M_PI/2)*SIZE_OF_SENSOR/2, target.y + sin((angle*M_PI/180)+M_PI/2)*SIZE_OF_SENSOR/2);
    double x, y;
    double num, denom;
    double yOrigin, xOrigin;
    xOrigin = p1.x;
    yOrigin = p1.y;
    num = (p2.y-p1.y);
    denom = (p2.x-p1.x); 
    x = xOrigin;

    for(int i=0; i<SIZE_OF_SENSOR; i++){

        if (abs(denom)>0.5){
            y = (x-xOrigin)*num/denom + yOrigin;
            sensorBuffer[i] = decodeColor(surface, x, y);
            x += denom/SIZE_OF_SENSOR;
        }
        else{
            x = xOrigin;
            y = yOrigin+i;
            sensorBuffer[i] = decodeColor(surface, x, y);
        }   
        
    }

}

void Car::createDistanceFromCars(Car carArray[], int numberOfCar){

    double tmpDistance = 0;
    double distance = CAR_SAFETY_DISTANCE;
    Point target = getPointToward(speed*0.15);
    Car closestCar;
    double deltaX, deltaY;
    double closestDeltaX, closestDeltaY;
    double targetDeltaX = position.x - target.x;
    double targetDeltaY = position.y - target.y;


    for(int i=0; i<numberOfCar; i++){

        deltaX = (position.x - carArray[i].position.x); 
        deltaY = (position.y - carArray[i].position.y); 
        tmpDistance = sqrt(deltaX * deltaX + deltaY * deltaY);

        if(tmpDistance > 0.001 && tmpDistance<distance){
            distance = tmpDistance;
            closestCar = carArray[i];
        }

    }

    //If a car is too close, we adapt the speed
    if(distance<CAR_SAFETY_DISTANCE){

        //Re calculat
        closestDeltaX = (position.x - closestCar.position.x); 
        closestDeltaY = (position.y - closestCar.position.y);        

        //Calculate the angle between the vector (front/Car)
        double dotProduct = targetDeltaX * closestDeltaX + closestDeltaY * targetDeltaY;
        double determinant = targetDeltaX * closestDeltaY - targetDeltaY * closestDeltaX;
        double vectAngle = atan2(determinant, dotProduct);

        if(abs(vectAngle)<M_PI/6 && speed>0){
            speed--;
        }

        else if(abs(vectAngle)>5*M_PI/6 && speed<baseSpeed*1.25){
            speed++;
        }

        else if (vectAngle>0 && distance<CAR_SAFETY_DISTANCE/2){
            angle--;   
        }
        
        else if (vectAngle<0 && distance<CAR_SAFETY_DISTANCE/2){
            angle++;
        }

    }

    //If there is no car, re-adjust the speed
    else {
        if(speed<baseSpeed) speed++;
        else speed--;
    }

}
