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

#define GRIDW 51
#define GRIDH 51
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
#define LOOTLIMIT 2

#define TAGCOUNT 11

static const int IMMUNELENGTH = 50;
static const int MASTERDISEASE = 25;
static const int MAXDISEASELEN = 10;
static const int DISEASESGIVEN = 10;

static const int CREDITDURATION = 10;
static const int CREDITRATE = 10;

static const int AGENTS = 400;
static const int HISTOGRAMH = 200;


enum movement {NormalM, WithPollution, WithCombat, WithTrade};
enum growback {NormalG, Seasonal};

static const movement MOVEMENT = NormalM;
static const growback GROWBACK = NormalG;
static const bool INHERITANCE = true;
static const bool CULTURE = true;
static const bool MATING = true;
static const bool REPLACEMENT = false;
static const bool DISEASE = false;
static const bool TRADING = false;
static const bool CREDITRULE = true;