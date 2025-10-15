#include "GraphicLayer.h"
#include "SFMLGraphics.h"

#define USE_SFML

#ifdef USE_SFML
std::unique_ptr<GraphicLayer> graphics = std::make_unique<SFMLGraphics>();
#else
std::unique_ptr<GraphicLayer> graphics = nullptr;
#endif