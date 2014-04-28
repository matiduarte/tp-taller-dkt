#ifndef __RECTANGULO_DIBUJABLE_H__
#define __RECTANGULO_DIBUJABLE_H__

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "DibujableGFX.h"
#include "../../Observador/Observador.h"
#include "../../Modelo/Figuras/Figura.h"

using namespace std;

class RectanguloDibujable : public Observador, public DibujableGFX{
private:
	short int* posicionesX;
	short int* posicionesY;
	float ancho;
	float alto;
	short int n;
public:
	RectanguloDibujable(float escalaAncho, float escalaAlto);
	RectanguloDibujable();
	~RectanguloDibujable();
	void actualizar(Observable* observable);
	void dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx);
	virtual string serializar();
	virtual void deserealizar(string aDeserealizar);
};

#endif