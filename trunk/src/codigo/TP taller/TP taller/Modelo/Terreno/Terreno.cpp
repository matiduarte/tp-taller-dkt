#include "Terreno.h"



Terreno::Terreno(b2World* world){

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(0,0);
	bodyDef.angle = 0;
	this->body = world->CreateBody(&bodyDef);
}


void Terreno::generarTerreno(string nombreArchivo){

	this->lectorTerreno = new LectorTerreno(nombreArchivo);
	pixel** matrizTerreno = lectorTerreno->getMatrizTerreno();
	int anchoMatriz = lectorTerreno->getAnchoMatriz();
	int altoMatriz = lectorTerreno->getAltoMatriz();
	bool huboTierra = false;
	bool aguasProfundas = false;
	int tamanioBorde = lectorTerreno->getTamanoBorde();
	bool hayTierra = false;
	int posVec = 0;
	int k = 0;
	int contCol = 0;


	b2Vec2* vecBorde = new b2Vec2[anchoMatriz*2];
	

	// Recorro la matriz hasta encontrar tierra
	for (int i = 0; i < anchoMatriz; i+=relacionPPU) { 
		hayTierra = false;
		int contFil = 0;
		for (int j = 0; ((j < altoMatriz) && !(hayTierra)); j++){
			//Encuentro el borde y genero un chain
			if (lectorTerreno->esTierra(matrizTerreno[i][j])){
				if (aguasProfundas){
					aguasProfundas = false;
					vecBorde[posVec].Set(i / relacionPPU,1000);
					posVec++;
				}

				hayTierra = true;
				huboTierra = true;
				//Seteo las cordenadas del borde en el vector
				if (posVec == 0) {
					vecBorde[posVec].Set(i /relacionPPU,1000);
					posVec++;
				}
				vecBorde[posVec].Set(i /relacionPPU,j /relacionPPU);
				posVec++;
				k++;
			} else {
				contFil++;
			}

		}

		if ((contFil == altoMatriz) && (huboTierra) && ( k != tamanioBorde)){
			vecBorde[posVec].Set((i- relacionPPU) / relacionPPU,1000);
			posVec++;
			aguasProfundas = true;
			huboTierra = false;
		}
	}

	b2ChainShape chain;
	chain.CreateChain(vecBorde, posVec);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &chain;
	fixtureDef.restitution = restitucion;
	fixtureDef.friction = friccion;
	this->body->CreateFixture(&fixtureDef);


	delete vecBorde;
}

b2Body* Terreno::getBody(){
	return this->body;
}

LectorTerreno* Terreno::getLector(){

	return this->lectorTerreno;
}

LectorTerreno* Terreno::getLectorTerreno(){
	return this->lectorTerreno;
}

Terreno::~Terreno(void)
{
	delete this->lectorTerreno;
}
