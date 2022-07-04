#include "funciones.h"

/*
entrada: string con el nombre del archivo a crear, int con la cantidad de discos, arreglo de la estuctura
Disk que posee los resultados por disco
salida: archivo .txt con los resultados impresos
funcion que imprime los resultados de los procesos hijos en un archivo .txt
*/
void escribirArchivoSalida(char* archivo_salida, int cant_discos, Disk* Discos){
    FILE* a= fopen(archivo_salida,"w");
    for (int i = 0; i < cant_discos; ++i){

        long double mediaReal = Discos[i].real/Discos[i].contadorVis;
        long double mediaImg = Discos[i].img/Discos[i].contadorVis;
        long double potencia = Discos[i].potencia;
        fprintf(a, "Disco %d:\n", i+1);
        fprintf(a, "Media Real: %Lf\n",mediaReal);
        fprintf(a, "Media imaginaria: %Lf\n",mediaImg);
        fprintf(a, "Potencia: %Lf\n",potencia);
        fprintf(a, "Ruido total: %Lf\n",Discos[i].ruido);
        
        
    }

    fclose(a);
}

/*
entrada: int con el ancho de los discos, int con la cantidad de discos, distancia que posee la visibilidad leida previamente
salida: int que detemina el indice del disco al que pertenece
Función que devuelve el identificador del disco al que pertence la distancia entrante 
*/
int hashDisk(int ancho_disco, int cant_discos, long double distancia){

    int limInf = 0;
    int limSup = ancho_disco;
    for (int i = 0; i < cant_discos; i++)
    {
        if (i == 0)
        {
            if (distancia >= limInf && distancia < limSup  )
            {
                return i;
            }            
            else{
                limInf = ancho_disco;
                limSup = limSup + ancho_disco;
            }
        }
        else if(i == cant_discos-1){
            if (distancia >= limInf)
            {
                return i;
            }
            
        }
        else{
            if (distancia >= limInf && distancia < limSup)
            {
                return i;
            }
            else{
                limInf = limInf + ancho_disco;
                limSup = limSup + ancho_disco;
            }
            
        }
        
    }
    return -1;
    
}