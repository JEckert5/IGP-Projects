#ifndef INCLUDES_HPP
#define INCLUDES_HPP

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <string>
#include <random>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>
#include <functional>

extern float DeltaTime;
extern std::mt19937 m;
extern float lerp(float a, float b, float t);
extern const int Width;
extern const int Height;

#endif