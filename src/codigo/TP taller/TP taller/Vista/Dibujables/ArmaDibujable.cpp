#include "ArmaDibujable.h"


ArmaDibujable::ArmaDibujable(void)
{
}

ArmaDibujable::ArmaDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF)
	: DibujableTextura(renderer, rect, pathImagen, pathDEF)
{
	
}

void ArmaDibujable::actualizar(Observable* observable){

	Bazooka* fig = (Bazooka*)observable;

	cout << "Pos x: " << fig->getPosicion().x << endl;
	cout << "Pos y: " << fig->getPosicion().y << endl;
	SDL_Rect rect = this->rect;
	rect.x = (fig->getPosicion().x * relacionPPU) - rect.w /2;
	rect.y = (fig->getPosicion().y * relacionPPU) - rect.h /2;

	this->setRect(rect);

}

void ArmaDibujable::dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom,int anchoPx, int altoPx){
	
	SDL_Rect rect = this->rect;
	rect.x -= corrimientoX;
	rect.y -= corrimientoY;

	if ((escalaZoom != escalaZoomDefault) && (escalaZoom <= zoomMax)) {
		rect = realizarZoom(this->rect, corrimientoX, corrimientoY, escalaZoom);
		SDL_RenderCopyEx(renderer,this->imagen, NULL , &rect, this->anguloRotacion ,NULL,SDL_FLIP_NONE);
	} else {
		SDL_RenderCopyEx(renderer,this->imagen, NULL, &rect, this->anguloRotacion,NULL,SDL_FLIP_NONE);
	}
}

ArmaDibujable::~ArmaDibujable(void)
{
}
