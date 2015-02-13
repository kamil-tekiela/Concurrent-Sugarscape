#pragma once

#ifdef _DEBUG
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-network-d.lib")
#else
#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-audio.lib")
#pragma comment(lib,"sfml-system.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-network.lib")
#endif
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib") 
#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

#define GRIDW 50
#define GRIDH 50
#define TILEW 10
#define TILEH 10

#define MAXLEVEL 5
#define POLLUTIONALPHA 1
#define POLLUTIONBETA 1
#define DIFFUSION 1

#define RADIUS 5
#define MAXVISION 5
#define MAXMETABOL 4
#define AGEM 10
#define LOOTLIMIT 100000

#define TAGCOUNT 11

#define AGENTS 400
#define HISTOGRAMH 200