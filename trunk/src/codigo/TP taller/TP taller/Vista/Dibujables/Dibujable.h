#ifndef __DIBUJABLE_H__
#define __DIBUJABLE_H__

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL2_gfxPrimitives.h"
#include "Box2D/Box2D.h"
#include "../../Parser/yaml/ParserYaml.h"
#include <string>

using namespace std;

struct DibujableSerializado{
    int posicionX;
	int posicionY;
	int radioHorizontal;
    int radioVertical;
};

class Dibujable
{
public:
	Dibujable();
	~Dibujable();
	virtual void setColor(int* rgb);
	virtual void dibujar(SDL_Renderer* renderer, int corrimientoX, int corrimientoY, float escalaZoom, int posZoomX, int posZoomY);
	virtual DibujableSerializado getDibujableSerializado(int& tamano);
};

#endif