#include "Test.h"
SDL_Surface* testImage;


string carTestFunction(){
    Car test;
    stringstream log ;
    log <<"\n*************** Testing Car Object ***************\n";
    log << "Testing the creation of an empty car:\n";
    //testing angle value
    if (test.angle == 0){
        log << "\tAngle : Pass\n" ;
    }else{
        log << "\tAngle : Error angle equal : " << test.angle <<" not 0\n";
    }

    //testing speed value
    if (test.speed == 0){
        log << "\tSpeed : Pass\n";
    }else{
        log <<"\tSpeed : Error speed equal : " << test.speed << " not 0\n";
    }

    //testion position value
    if (test.position.x == 50 && test.position.y == 50){
        log << "\tPosition : Pass\n";
    }else{
        log << "\tPosition : Error position equal : [" << test.position.x << " ; " << test.position.y << " ] not [ 50 ; 50 ]\n";
    }

    //With a car with confugure option
    Car test2 = Car(120,111,90);

    log << "\nTesting the creation of an full car\n";
    //testing angle value
    if (test2.angle == 90){
        log << "\tAngle : Pass\n" ;
    }else{
        log << "\tAngle : Error angle equal : " << test2.angle <<" not 0\n";
    }

    //testing speed value
    if (test2.speed == 0){
        log << "\tSpeed : Pass,\n";
    }else{
        log <<"\tSpeed : Error speed equal : " << test2.speed << " not 0\n";
    }

    //testion position value
    if (test2.position.x == 120 && test2.position.y == 111){
        log << "\tPosition : Pass\n";
    }else{
        log << "\tPosition : Error position equal : [" << test2.position.x << " ; " << test2.position.y << " ] not [ 120 ; 111 ] \n";
    }

    log << "\nTesting Get_Point_Toward function\n";

    Point p = test.getPointToward(13);
    if (p.x == 63 && p.y == 50){
        log << "\tTest 1 : Pass\n";
    }else{
        log << "\tTest 1 : Error position equal : [" << p.x << " ; " << p.y << " ] not [ 63 ; 50 ] \n";
    }

    test.angle = 90;
    p = test.getPointToward(13);
    if (p.x == 50 && p.y == 63){
        log << "\tTest 2 : Pass\n";
    }else{
        log << "\tTest 2 : Error position equal : [" << p.x << " ; " << p.y << " ] not [ 50 ; 63 ] \n";
    }

    log << "\nTesting Go_Forward function\n";
    
    test2.speed = 100;
    test2.goForward();
    if (test2.position.x == 120 && test2.position.y == 126){
        log << "\tPass\n";
    }else{
        log << "\tError position equal : [" << test2.position.x << " ; " << test2.position.y << " ] not [ 135 ; 111 ]\n";
    }
    
    return log.str();

}

string pointTestFunction(){
    Point test;
    stringstream log ;
    log << "*************** Testing Point Object ***************\n";
    log << "Testing the creation of an empty point\n";

    if (test.x == 50 && test.y == 50){
        log << "\tPass\n" ;
    }else{
        log << "\tError point equal : [ " << test.x <<" ; " << test.y << " ] and not [ 50 ; 50 ]\n";
    }

    log << "Testing the creation of a preconfigure point\n";

    Point test2 = Point(120,111);
    if (test2.x == 120 && test2.y == 111){
        log << "\tPass\n" ;
    }else{
        log << "\tError point equal : [ " << test2.x <<" ; " << test2.y << " ] and not [ 120 ; 111 ]\n";
    }
    
    log << "\n\n";
    return log.str();
}

string turnTestFunction(){
    double tmpTurn=50;
    stringstream log ;
    SDL_Color magenta = {255, 0, 255};
    SDL_Color cyan = {0, 255, 255};
    SDL_Color blue = {0, 0, 255};
    SDL_Color gray = {100, 100, 100};

    log << "*************** Testing turn calculation ***************\n";
    //Init every Car & frontSensor
    Car car1 = Car(5, 5, 0);
    Car car2 = Car(5, 5, 0);
    Car car3 = Car(5, 5, 0);
    Car car4 = Car(5, 5, 0);
    Car car5 = Car(5, 5, 0);
    Car car6 = Car(5, 5, 0);
    Car car7 = Car(5, 5, 0);
    Car car8 = Car(5, 5, 0);
    Car car9 = Car(5, 5, 0);
    Car car10 = Car(5, 5, 0);
    Car car11 = Car(5, 5, 0);
   

    for (int i; i<FRONT_SENSOR_DEPTH; i++){
        for(int j=0; j<SIZE_OF_SENSOR; j++){

            //Car1: Empty sensor
            car1.frontSensor[i][j]=blue;

            //Car2: Straight Line
            if(j<5)car2.frontSensor[i][j]=cyan;
            else if(j>SIZE_OF_SENSOR-6) car2.frontSensor[i][j]=magenta;
            else car2.frontSensor[i][j]=blue;
            
            //Car3: Left sideWalk
            if(j<5)car3.frontSensor[i][j]=cyan;
            else car3.frontSensor[i][j]=blue;

            //Car4: Right Sidewalk
            if(j>SIZE_OF_SENSOR-6)car4.frontSensor[i][j]=magenta;
            else car4.frontSensor[i][j]=blue;

            //Car5: Walled road
            if(i>FRONT_SENSOR_DEPTH-20)car5.frontSensor[i][j]=gray;
            else car5.frontSensor[i][j]=blue;

            //Car6: Rock road
            if(i>FRONT_SENSOR_DEPTH-20 && j> SIZE_OF_SENSOR/2-5 && j> SIZE_OF_SENSOR/2+5 )car6.frontSensor[i][j]=gray;
            else car6.frontSensor[i][j]=blue;

            //Car7: Left Wall
            if(i>FRONT_SENSOR_DEPTH-20 && j<SIZE_OF_SENSOR/2-5)car7.frontSensor[i][j]=gray;
            else car7.frontSensor[i][j]=blue;

            //Car8: Right Wall
            if(i>FRONT_SENSOR_DEPTH-20 && j>SIZE_OF_SENSOR/2+5)car8.frontSensor[i][j]=gray;
            else car8.frontSensor[i][j]=blue;

            //Car9: Rock road w/ sidewalk
            if(i>FRONT_SENSOR_DEPTH-20 && j> SIZE_OF_SENSOR/2-5 && j> SIZE_OF_SENSOR/2+5 )car9.frontSensor[i][j]=gray;
            else if(j<5)car9.frontSensor[i][j]=cyan;
            else if(j>SIZE_OF_SENSOR-6) car9.frontSensor[i][j]=magenta;
            else car9.frontSensor[i][j]=blue;

            //Car10: Left Wall w/sidewalk
            if(i>FRONT_SENSOR_DEPTH-20 && j<SIZE_OF_SENSOR/2-5)car10.frontSensor[i][j]=gray;
            else if(j<5)car10.frontSensor[i][j]=cyan;
            else if(j>SIZE_OF_SENSOR-6) car10.frontSensor[i][j]=magenta;
            else car10.frontSensor[i][j]=blue;

            //Car11: Right Wall w/sidewalk
            if(i>FRONT_SENSOR_DEPTH-20 && j>SIZE_OF_SENSOR/2+5)car11.frontSensor[i][j]=gray;
            else if(j<5)car11.frontSensor[i][j]=cyan;
            else if(j>SIZE_OF_SENSOR-6) car11.frontSensor[i][j]=magenta;
            else car11.frontSensor[i][j]=blue;


        } 

    }

    //Testing the empty road
    log << "Testing empty road\n";
    tmpTurn = car1.calculateTurn();
    if (tmpTurn==0){
        log << "\tPass\n" ;
    }else{
        log << "\tError  turn is equal to [" << tmpTurn <<"] but should be [0]\n";
    }

    //Testing the road /w both sidewalks
    log << "Testing both sidewalk road\n";
    tmpTurn = car2.calculateTurn();
    if (tmpTurn==0){
        log << "\tPass\n" ;
    }else{
        log << "\tError  turn is equal to [" << tmpTurn <<"] but should be [0]\n";
    }

    //Testing the road /w left sidewalk
    log << "Testing left sidewalk road\n";
    tmpTurn = car3.calculateTurn();
    if (tmpTurn<0){
        log << "\tPass\n" ;
    }else{
        log << "\tError  turn is equal to [" << tmpTurn <<"] but should be [<0]\n";
    }

    //Testing the road /w right sidewalk
    log << "Testing right sidewalk road\n";
    tmpTurn = car4.calculateTurn();
    if (tmpTurn>0){
        log << "\tPass\n" ;
    }else{
        log << "\tError  turn is equal to [" << tmpTurn <<"] but should be [>0]\n";
    }

    //Testing the road /w Wall
    log << "Testing Blocked/Walled road\n";
    tmpTurn = car5.calculateTurn();
    if (tmpTurn<0){
        log << "\tTurn: Pass\n" ;
    }else{
        log << "\tError  turn is equal to [" << tmpTurn <<"] but should be [<0]\n";
    }

    //Testing the road /w Rock
    log << "Testing Rock on the road\n";
    tmpTurn = car6.calculateTurn();
    if (tmpTurn>0){
        log << "\tTurn: Pass\n" ;
    }else{
        log << "\tError  turn is equal to [" << tmpTurn <<"] but should be [>0]\n";
    }

    //Testing the road /w Left wall
    log << "Testing left wall road\n";
    tmpTurn = car7.calculateTurn();
    if (tmpTurn<0){
        log << "\tTurn: Pass\n" ;
    }else{
        log << "\tError  turn is equal to [" << tmpTurn <<"] but should be [<0]\n";
    }

    //Testing the road /w Right wall
    log << "Testing Right wall road\n";
    tmpTurn = car8.calculateTurn();
    if (tmpTurn>0){
        log << "\tTurn: Pass\n" ;
    }else{
        log << "\tError  turn is equal to [" << tmpTurn <<"] but should be [>0]\n";
    }

    //Testing the road /w Righobstacles & sidewalks wall
    log << "Testing rock + sidewalks\n";
    tmpTurn = car9.calculateTurn();
    if (tmpTurn>0){
        log << "\tTurn: Pass\n" ;
    }else{
        log << "\tError  turn is equal to [" << tmpTurn <<"] but should be [>0]\n";
    }

    //Testing the road /w Righobstacles & sidewalks wall
    log << "Testing Left obs + sidewalks\n";
    tmpTurn = car10.calculateTurn();
    if (tmpTurn<0){
        log << "\tTurn: Pass\n" ;
    }else{
        log << "\tError  turn is equal to [" << tmpTurn <<"] but should be [<0]\n";
    }

    //Testing the road /w Righobstacles & sidewalks wall
    log << "Testing Right obs + sidewalks\n";
    tmpTurn = car11.calculateTurn();
    if (tmpTurn>0){
        log << "\tTurn: Pass\n" ;
    }else{
        log << "\tError  turn is equal to [" << tmpTurn <<"] but should be [>0]\n";
    }
    log << "\n\n";
    return log.str();
}



string createDistanceTestFunction(){
    stringstream log ;

    log << "*************** Testing distance between cars ***************\n";
    //Init every Car
    Car carTest[5];
    carTest[0] = Car(5, 5, 0);
    carTest[1] = Car(7, 5, 0);
    carTest[2] = Car(3, 5, 0);
    carTest[3] = Car(5, 3, 0);
    carTest[4] = Car(5, 7, 0);
    
    for(int i=0; i<5; i++){

        carTest[i].baseSpeed = 10;
        carTest[i].speed = carTest[i].baseSpeed;
        carTest[i].createDistanceFromCars(carTest, 5);

    }
   
    //Testing Car alone
    log << "Testing Car alone\n";
    Car aloneCar = Car(200,200,0);
    if (aloneCar.speed==aloneCar.baseSpeed && aloneCar.angle==0){
        log << "\tPass\n" ;
    }else{
        log << "\tError  speed is equal to [" << carTest[0].speed <<"] but should be ["<< carTest[0].baseSpeed <<"]\n";
    }


    //Testing Car front & Behinf
    log << "Testing Car Behind & Front\n";
    
    if (carTest[0].speed<carTest[0].baseSpeed){
        log << "\tPass\n" ;
    }else{
        log << "\tError  speed is equal to [" << carTest[0].speed <<"] but should be [<"<< carTest[0].baseSpeed <<"]\n";
    }

    //Testing Car behind
    log << "Testing Car Behind\n";
    
    if (carTest[1].speed>carTest[1].baseSpeed){
        log << "\tPass\n" ;
    }else{
        log << "\tError  speed is equal to [" << carTest[1].speed <<"] but should be [>"<< carTest[1].baseSpeed <<"]\n";
    }

    //Testing car in Front
    log << "Testing Car in Front\n";
    
    if (carTest[2].speed<carTest[2].baseSpeed){
        log << "\tPass\n" ;
    }else{
        log << "\tError  speed is equal to [" << carTest[2].speed <<"] but should be [<"<< carTest[2].baseSpeed <<"]\n";
    }

    //Testing car on right
    log << "Testing Car on the right\n";
    
    if (carTest[3].angle<0){
        log << "\tPass\n" ;
    }else{
        log << "\tError  angle is equal to [" << carTest[3].angle <<"] but should be [<0]\n";
    }

    //Testing car on left
    log << "Testing Car on the left\n";
    
    if (carTest[4].angle>0){
        log << "\tPass\n" ;
    }else{
        log << "\tError  angle is equal to [" << carTest[4].angle <<"] but should be [>0]\n";
    }

    log << "\n\n";
    return log.str();
}


int logFile(){
    // Create and open a text file
    ofstream MyFile("Log.txt");

    // Write to the file
    MyFile << pointTestFunction();
    MyFile << carTestFunction();
    MyFile << turnTestFunction();
    MyFile << createDistanceTestFunction();

    // Close the file
    MyFile.close();

    return EXIT_SUCCESS;
}


void testSequence(){

    
    //Loading Background
    testImage = SDL_LoadBMP("/home/tristan/Desktop/TS/AutoDriver/Source/Road2.bmp");
    if (testImage == NULL) {
        SDL_GetError();
        printf("[DEBUG] > %s\n", SDL_GetError());
    }

    //Testing everything
    logFile();

    SDL_FreeSurface(testImage);  
    SDL_Quit();   

}