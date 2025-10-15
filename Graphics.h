#pragma once
#ifndef GRAPHICS
#define GRAPHICS

#ifdef USE_SFML
#include <memory>
#include "GraphicLayer.h"
extern std::unique_ptr<GraphicLayer> graphics;
#else
#include <memory>
#include "GraphicLayer.h"
extern std::unique_ptr<GraphicLayer> graphics;
#endif

#endif // !GRAPHICS
