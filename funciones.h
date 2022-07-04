#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct visibilidad
{
    long double u;
    long double v;
    long double real;
    long double img;
    long double ruido;
} vis;

typedef struct Disco
{
    long double u;
    long double v;
    long double real;
    long double img;
    long double ruido;
    long double potencia;
    int contadorVis;
} Disk;



void escribirArchivoSalida(char* archivo_salida, int cant_discos, Disk* Discos);
int hashDisk(int ancho_disco, int cant_discos, long double distancia);
