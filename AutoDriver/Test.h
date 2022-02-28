#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <filesystem>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include "Car.h"
#include "config.h"
#include "Point.h"
#include "ColorDecoder.h"
#include <SDL2/SDL.h>


using namespace std;
using std::stringstream;

string carTestFunction();
string pointTestFunction();
string turnTestFunction();
string createDistanceTestFunction();
int logFile();
void testSequence();

