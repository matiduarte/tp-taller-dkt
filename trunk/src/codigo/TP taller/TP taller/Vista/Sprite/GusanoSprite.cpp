#include "GusanoSprite.h"


GusanoSprite::GusanoSprite(void)
{
	this->recCuadro = NULL;
	this->cartel = NULL;
	this->imagen = NULL;
}

GusanoSprite::GusanoSprite(SDL_Renderer* renderer, SDL_Rect recDestino, string path, int col, int fil, int anchoTex, int altoTex, string nombre,int maximosCLientes): DibujableTextura(){

	this->numCuadros = col*fil;
	this->velocidadRefresco = timeGusanoQuieto;
	this->contador = 0;

	int tamanioCuadroX = anchoTex / col;
	int tamanioCuadroY = altoTex / fil;
	this->frame = 0;
	this->rect = recDestino;

	this->rectApuntando = new SDL_Rect[32];
	for(int i=0; i< 32; i++){
		rectApuntando[i].h = tamanioCuadroY;
		rectApuntando[i].w = tamanioCuadroX;
	}

	for(int i=0; i<fil; i++){
		for(int j=0; j<col; j++){
			rectApuntando[j + i*col].x = j* tamanioCuadroX;
			rectApuntando[j + i*col].y = i* tamanioCuadroY;
		}
	}

	this->recCuadro = new SDL_Rect[numCuadros];
	for(int i=0; i< numCuadros; i++){
		recCuadro[i].h = tamanioCuadroY;
		recCuadro[i].w = tamanioCuadroX;
	}

	for(int i=0; i<fil; i++){
		for(int j=0; j<col; j++){

			recCuadro[j + i*col].x = j* tamanioCuadroX;
			recCuadro[j + i*col].y = i* tamanioCuadroY;
		}
	}
	
	this->imagen = IMG_LoadTexture(renderer, path.c_str());
	this->cambiarImgDer = false;
	this->cambiarImgIzq = false;
	this->contIzq = 0;
	this->contDer = 0;
	this->contFrent = 0;
	this->contMuerte = 0;
	this->estado = IZQ;
	this->nombre = nombre;
	SDL_Rect rectCart = this->rect;
	rectCart.h = rect.h / 4;
	rectCart.x = this->rect.x + this->rect.w/2;
	this->maximosCLientes = maximosCLientes;
	
	//this->cartel = NULL;
	this->cartel = new CartelDibujable(renderer, rectCart, rutaCartel, rutaCartelDEF, this->nombre);
	
	//this->mostrarCartel = false;
	for(int i=0; i < this->maximosCLientes; i++){
		this->mostrarCartel.push_back(false);
	}
	this->cliente = 0;
}

GusanoSprite::~GusanoSprite(void)
{
	if(this->recCuadro != NULL){
		delete []this->recCuadro;
	}

	if(this->cartel != NULL){
		delete this->cartel;
	}
	if(this->imagen != NULL){
		SDL_DestroyTexture(this->imagen);
		this->imagen = NULL;
	}
}

void GusanoSprite::actualizar(Observable* observable) {

	Gusano* fig = (Gusano*)observable;
	
	if (!(fig->estaMuerto())){
		this->contMuerte = 0;
		//No se mueve
		if ( !(fig->seMueveALaDer() ) && !(fig->seMueveALaIzq()) ) {
			this->contFrent++;
			this->contIzq = 0;
			this->contDer = 0;
			this->setCambiarImgDer(false);
			this->setCambiarImgIzq(false);
			this->velocidadRefresco = timeGusanoQuieto;
			this->actualizarFrame();
		} else {
			this->velocidadRefresco = timeGusanoMovil;
				if ((fig->seMueveALaDer())){
					this->contIzq = 0;
					this->contFrent = 0;
					this->contDer++;
					this->setCambiarImgDer(true);
					this->setCambiarImgIzq(false);
					this->actualizarFrame();
					this->estado = DER;
				} else {
					//Se mueve a la izquierda
					this->contDer = 0;
					this->contFrent = 0;
					this->contIzq++;
					this->setCambiarImgDer(false);
					this->setCambiarImgIzq(true);
					this->actualizarFrame();
					this->estado = IZQ;
				}
			}
	} else {
		this->velocidadRefresco = timeGrave;
		this->contIzq = 0;
		this->contDer = 0;
		this->contFrent = 0;
		this->contMuerte++;
		//this->actualizarFrame();
		this->estado = MUERTO;	
	}

	SDL_Rect rect = this->rect;
	rect.x = (fig->getPosicion().x * relacionPPU) - rect.w /2;
	rect.y = (fig->getPosicion().y * relacionPPU) - rect.h /2;
	this->setRect(rect);

	SDL_Rect rectCartel = this->cartel->getRect();
	rectCartel.x = (fig->getPosicion().x * relacionPPU) - rectCartel.w / 2;
	rectCartel.y = ((fig->getPosicion().y * relacionPPU) - rect.h / 2) - alturaCartel;
	this->cartel->setRect(rectCartel);

	for(int i=0; i < this->maximosCLientes; i++){
		this->mostrarCartel[i] = fig->getMeClickearon(i);
	}

	/*if (fig->getMeClickearon()) {
		this->mostrarCartel = true;
	} else {
		this->mostrarCartel = false;
	}*/

	this->congelado = fig->getCongelado();
}

void GusanoSprite::dibujar(SDL_Renderer *renderer, int corrimientoX,int corrimientoY, float escalaZoom,int anchoPx, int altoPx){
	SDL_Rect rect = this->rect;

	if ((this->estado == MUERTO) && (this->contMuerte >= 1)){
		this->setFrame(0);
		this->setImagen(renderer, rutaGrave);
	} else {
		if ( !(this->hayCambioImgDer()) && !(this->hayCambioImgIzq()) && (this->contFrent >= 1) ){
				if (this->estado == IZQ){
					if(!this->congelado){
						this->setImagen(renderer, spriteWormIzq);
					}else{
						this->setImagen(renderer, rutaWormGrisIzq);
					}
				}
				else{	
					if(!this->congelado){
						this->setImagen(renderer, spriteWormDer);
					}else{
						this->setImagen(renderer, rutaWormGrisDer);
					}
				}
		}
		if ( (this->hayCambioImgDer()) && (this->contDer >= 1 ) ){
			if (this->estado == MUERTO){
				this->setImagen(renderer, rutaGrave);
			} else {
				this->setImagen(renderer, rutaGusanoDer);
			}
		}
		if ( (this->hayCambioImgIzq())  && (this->contIzq >= 1) ){
			if (this->estado == MUERTO){
				this->setImagen(renderer, rutaGrave);
			} else {
				this->setImagen(renderer, rutaGusanoIzq);
			}
		}
	}
	
	if ((escalaZoom != escalaZoomDefault) && (escalaZoom <= zoomMax)) {
		rect = realizarZoom(rect, corrimientoX, corrimientoY, escalaZoom);
		SDL_RenderCopy(renderer, this->imagen, &this->recCuadro[frame], &rect);
	} else {
		rect.x -=corrimientoX;
		rect.y -=corrimientoY;
		SDL_RenderCopy(renderer, this->imagen, &this->recCuadro[frame], &rect);
	}

	if (this->mostrarCartel[this->cliente] && this->estado != MUERTO) {
		this->cartel->dibujar(renderer,corrimientoX,corrimientoY,escalaZoom,anchoPx,altoPx);
	}
}

void GusanoSprite::setCambiarImgDer(bool cambio){
	this->cambiarImgDer = cambio;
}

bool GusanoSprite::hayCambioImgDer(){
	return this->cambiarImgDer;
}

void GusanoSprite::setCambiarImgIzq(bool cambio){
	this->cambiarImgIzq = cambio;
}

bool GusanoSprite::hayCambioImgIzq(){
	return this->cambiarImgIzq;
}

void GusanoSprite::setNumCuadros(int numCuad){
	this->numCuadros;
}

void GusanoSprite::actualizarFrame(){
	this->contador++;
	if(this->contador >= this->velocidadRefresco){
			this->frame++;
			this->contador = 0;
	}
	if(this->frame >= this->numCuadros) this->frame = 0;
}

int GusanoSprite::getFrame(){
	return this->frame;
}

void GusanoSprite::setFrame(int frame){
	this->frame = frame;
}

string GusanoSprite::serializar(){
	string serializado = "";
	serializado = StringUtil::int2string(serializadoGusanoSprite);
	serializado += separadorCampoTipoEntidades;    
	serializado += StringUtil::float2string(this->frame);
	serializado += separadorCamposEntidades;    
	serializado += StringUtil::int2string(this->rect.x);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->rect.y);
    serializado += separadorCamposEntidades;
    serializado += StringUtil::int2string(this->rect.w);
	serializado += separadorCamposEntidades;
    serializado += StringUtil::int2string(this->rect.h);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->contDer);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->contIzq);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->cambiarImgDer);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->cambiarImgIzq);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->contFrent);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->contMuerte);
	serializado += separadorCamposEntidades;
	serializado += this->nombre;
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->contador);
	serializado += separadorCamposEntidades;
	
	int estadoNumero = 1;
	if(this->estado == DER){
		estadoNumero = 2;
	}
	else if( this->estado == MUERTO){
		estadoNumero = 3;
	}
	serializado += StringUtil::int2string(estadoNumero);

	serializado += separadorCamposEntidades;
	//serializado += StringUtil::int2string(this->mostrarCartel);
	std::stringstream ss;
	for(size_t j = 0; j < this->mostrarCartel.size(); ++j)
	{
		if(j != 0){
			ss << separadorCamposArreglo;
		}
		ss << StringUtil::int2string(this->mostrarCartel[j]);
	}
	std::string vectorSerializado = ss.str();
	serializado += vectorSerializado;
	
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->velocidadRefresco);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->numCuadros);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->congelado);
	
	return serializado;
}

void GusanoSprite::deserealizar(string aDeserealizar){
	vector<string> des = StringUtil::split(aDeserealizar,separadorCampoTipoEntidades);
	//des.at(0) tiene el tipo, des.at(0) tiene el resto de los atributos
	vector<string> atributos = StringUtil::split(des.at(1),separadorCamposEntidades);
	this->frame = StringUtil::str2float(atributos.at(0).c_str());
    SDL_Rect rectAux;
	rectAux.x = StringUtil::str2int(atributos.at(1));
	rectAux.y = StringUtil::str2int(atributos.at(2));
	rectAux.w = StringUtil::str2int(atributos.at(3));
	rectAux.h = StringUtil::str2int(atributos.at(4));
	this->setRect(rectAux);
	this->contDer = StringUtil::str2int(atributos.at(5));
	this->contIzq = StringUtil::str2int(atributos.at(6));

	bool camb = false;
	int cambNumero = StringUtil::str2int(atributos.at(7));
	if(cambNumero > 0){
		camb = true;
	}
	this->cambiarImgDer = camb;
	
	camb = false;
	cambNumero = StringUtil::str2int(atributos.at(8));
	if(cambNumero > 0){
		camb = true;
	}
	this->cambiarImgIzq = camb;

	this->contFrent = StringUtil::str2int(atributos.at(9));
	this->contMuerte = StringUtil::str2int(atributos.at(10));
	this->nombre = atributos.at(11);
	this->contador = StringUtil::str2int(atributos.at(12));

	int estadoNumero = StringUtil::str2int(atributos.at(13));
	if(estadoNumero == 1){
		this->estado = IZQ;
	}
	else if(estadoNumero == 2){
		this->estado = DER;
	}
	else if(estadoNumero == 3){

		this->estado = MUERTO;
	}	

	//this->mostrarCartel = StringUtil::str2int(atributos.at(14));
	vector<string> mostrarCartelArreglo = StringUtil::split(atributos.at(14),separadorCamposArreglo);
	for (int i = 0; i < mostrarCartelArreglo.size(); i++) {
		this->mostrarCartel.push_back(StringUtil::str2int(mostrarCartelArreglo.at(i)));
	}

	this->velocidadRefresco = StringUtil::str2int(atributos.at(15));
	this->numCuadros = StringUtil::str2int(atributos.at(16));
	this->congelado = StringUtil::str2int(atributos.at(17));

	this->recCuadro = NULL;
	this->cartel = NULL;
	this->imagen = NULL;
}

CartelDibujable* GusanoSprite::getCartel(){
	return this->cartel;
}

string GusanoSprite::getNombre(){
	return this->nombre;
}