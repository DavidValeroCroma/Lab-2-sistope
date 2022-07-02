#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>


/*
entrada:
salida:
Esta funcion/rutina realiza el procesamiento de los datos de entrada 
mediando el acceso a secciones criticas por parte de las hebras 
*/
void * thread_rutine(){

}

/*
entrada:
salida:
Esta funcion corresponde a la escritura de los resultados en un archivo .out 
el cual contendra estos ordenados por anillo
*/
void * escrituraDeArchivo(){

}

/*
entrada:
salida:
Esta funcion corresponde a la lectura del archivo propiedades.csv, el cual contiene 
las propiedades de las muestras tomadas
*/
void * lecturaDeArchivo(){

}


FILE * flujo;
pthread_mutex_t **MUTEX;
int main(int argc, char** argv){
    

    /*
    $ ./lab2 -i visibilidades.csv -o propiedades.txt -d ancho -n ndiscos -h numerohebras -c chunk -b
    • -i: nombre de archivo con visibilidades
    • -o: nombre de archivo de salida
    • -n: cantidad de discos
    • -d: ancho de cada disco.
    • -h: n´umero de hebras trabajadoras
    • -c: chunk, n´umero de l´ıneas que la hebra lee por vez
    • -b: bandera que permite indicar si se quiere ver por consola las propiedades calculadas. Es decir, si
    aparece la opci´on, entonces se muestra la salida por consola.
    */

    //-----------------------------------------------------------------getopt
    char archivo_entrada[100];
    char archivo_salida[100];
    int ancho_disco, cant_discos,variable_control, cant_hebras, chunk = 0;
    int c;
    while((c = getopt(argc, argv, "i:o:n:d:h:c:b")) != -1){
        switch(c)
        {
            case 'i': //capturar nombre del archivo de entrada
                sscanf(optarg, "%s", archivo_entrada); 
                break;
            case 'o': //capturar nombre del archivo de salida
                sscanf(optarg, "%s", archivo_salida); 
                break;
            case 'n': //capturar cantidad de discos
                sscanf(optarg, "%d", &cant_discos);
                if(cant_discos == 0)
                {
                    printf("Error al ingresar parametro de n\n");
                    printf("\n -----FORMATO-----\n ./lab1 -i visibilidades.csv -o propiedades.txt -d ancho_disco -n ndiscos -b");

                    return(1);
                }
                break;
            case 'd': //capturar ancho_disco de cada disco
                sscanf(optarg, "%d", &ancho_disco);
                if(ancho_disco == 0)
                {
                    printf("Error al ingresar parametro de d\n");
                    printf("\n -----FORMATO-----\n ./lab1 -i visibilidades.csv -o propiedades.txt -d ancho_disco -n ndiscos -b");

                    return(1);
                }
                break;
            case 'h': //capturar cantidad de discos
                sscanf(optarg, "%d", &cant_hebras);
                if(cant_hebras == 0)
                {
                    printf("Error al ingresar parametro de h\n");
                    printf("\n -----FORMATO-----\n ./lab1 -i visibilidades.csv -o propiedades.txt -d ancho_disco -n ndiscos -b");

                    return(1);
                }
                break;
            case 'c': //capturar cantidad de discos
                sscanf(optarg, "%d", &chunk);
                if(chunk == 0)
                {
                    printf("Error al ingresar parametro de c\n");
                    printf("\n -----FORMATO-----\n ./lab1 -i visibilidades.csv -o propiedades.txt -d ancho_disco -n ndiscos -b");

                    return(1);
                }
                break;
            
            
            case 'b': //capturar variable de control
                variable_control = 1;
                break;
            default:
                printf("Caracter desconocido \n -----FORMATO-----\n ./lab1 -i visibilidades.csv -o propiedades.txt -d ancho_disco -n ndiscos -b");
        }
    }

    //---------------------------- Apertura del archivo ----------------------------

    flujo = fopen(archivo_entrada, "r");

    //----------------------- creacion y declaracion de threads --------------------
    
    //inicialización de atributos
    pthread_attr_t attr[cant_hebras];
    for (int i = 0; i < cant_hebras; i++)
    {
        pthread_attr_init(&attr[i]);
    }
    //Reserva de memoria mutex
    MUTEX = (pthread_mutex_t**)malloc(sizeof(pthread_mutex_t*)*cant_hebras);
    for (int i = 0; i < cant_hebras; i++)
    {
        MUTEX[i] = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t)*2);
    }
    //Inicialización de mutex
    for (int i = 0; i < cant_hebras; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            pthread_mutex_init(&MUTEX[i][j], NULL);
        }
        
    }
    //creación de hebras
    pthread_t thread[cant_hebras];
    for (int i = 0; i < cant_hebras; ++i)
    {
        if (0 != pthread_create(&thread[i], &attr[i], thread_rutine, NULL))
        {
            printf("la hebra %d no se pudo ejecutar correctamente", i);
        }
    }

    for (int j = 0; j < cant_hebras ; ++j)
    {
        pthread_join(thread[j], NULL);
    }

    //------------------------ Esperar a que terminen las threads ------------------

    return 0;
}