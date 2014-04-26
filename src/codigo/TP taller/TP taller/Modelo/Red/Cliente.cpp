#include "Cliente.h" 


Cliente::Cliente(string nombre)
{
	this->username=nombre;
    red = new ClienteRed();
	 // send init packet
	
	int largoMensaje = username.size()+1;
	int paquete_tamano = sizeof(int) + sizeof(int) + largoMensaje;
    char* paquete_data = new char[paquete_tamano];

    Paquete paquete;
    paquete.setTipo(paqueteInicial);
	paquete.setMensaje(username);
	paquete.setTamanio(largoMensaje);
    paquete.serializar(paquete_data);
	Servicio::enviarMensaje(red->socketCliente, paquete_data, paquete_tamano);
}

void Cliente::enviarEstado(){

	string mensaje = "";
	int largoMensaje = 1;
	int paquete_tamano = sizeof(int) + sizeof(int) + largoMensaje;
    char* paquete_data = new char[paquete_tamano];

    Paquete paquete;
    paquete.setTipo(paqueteEstado);
	paquete.setMensaje(mensaje);
	paquete.setTamanio(largoMensaje);
    paquete.serializar(paquete_data);
	Servicio::enviarMensaje(red->socketCliente, paquete_data, paquete_tamano);

}

void Cliente::recibirDeServidor()
{
    Paquete* paquete = new Paquete();
   
        // get data from server
        int data_length = red->recibirData(network_data);
		
        if (data_length <= 0) 
        {
            //no data recieved
			//ver q hacerrrrrrrrrrrrr
        }

        int i = 0;
		while (i < data_length) 
        {
            paquete->deserializar(&(network_data[i]));
			i += paquete->getPesoPaquete();

			switch (paquete->getTipo()) {

                case paqueteInicial:

					printf("El cliente recibio el paquete inicial del servidor.\n");
					break;

                case paqueteEvento:

					printf("El cliente recibio un paquete evento del servidor.\n");
					break;

				case paqueteVista:
					printf("El cliente recibio un paquete vista del servidor.\n");
					
					//memcpy(&lista, paquete.getMensaje().c_str(), paquete.getTamanio());
					//objetosSerializados = StringUtil::split(paquete.getMensaje(),'#');

					//printf("Tamano de la lista:%d .\n",lista.size());
					/*for (std::vector<string>::iterator it =objetosSerializados.begin(); it != objetosSerializados.end(); it++) {
						printf("FOR.\n");
						DibujableSerializado dib;
						memcpy(&dib, (*it).c_str(), sizeof(DibujableSerializado));
						printf("PosX:%d .\n",dib.posicionX);
						printf("PosY:%d .\n",dib.posicionY);
					}*/

					break;
                default:

                    printf("Error en el tipo de paquete.Tipo es %d\n",paquete->getTipo());

                    break;
            }
        }
		delete paquete;
    }


void Cliente::actualizar() 
{
	recibirDeServidor();
	enviarEstado();
}