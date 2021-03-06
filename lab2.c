/*
Laboratorio 2 Sistemas Operativos 2022-1

Nombres: David Valero Croma ;  Israel Gruzman Oyarzún 
rut: 20.636.919-1  ;  20.613.921-8
Seccion: A-1
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "funciones.h"
#define LECTURA 0
#define ESCRITURA 1


FILE * flujo;
pthread_mutex_t mutexLectura;
pthread_mutex_t mutexEscritura;
int ancho_disco, cant_discos, chunk;
int contador = 0; //cuenta visibilidades
int contadorH = 0; //cuenta hebras
int leido = 0;
Disk* Discos;



/*
entrada: parametros declarados globalmente
salida: propiedades de los discos agregados a la lista de struct Disk llamada Discos
Esta funcion/rutina realiza el procesamiento de los datos de entrada mediando el acceso
a secciones criticas por parte de las hebras, calculando y actualizando los datos 
de los discos
*/
void * thread_rutine(void *unused){
    
    if (!feof(flujo)) //revisamos que no llegamos al final del archivo
    {
        
        int chunkLeido = 0;
        pthread_mutex_lock(&mutexLectura);
        while(leido != 1 && chunkLeido < chunk && !feof(flujo)){

            if (leido == 0)
            {
                //lectura de visibilidad 
                vis auxVis;

                //----------------------------------------- Inicio Lectura -----------------------------------------


                //printf("----- Soy la hebra %d ----- \n", contadorH);
                if(fscanf(flujo, "%Lf,%Lf,%Lf,%Lf,%Lf", &auxVis.u, &auxVis.v, &auxVis.real, &auxVis.img, &auxVis.ruido)){
                    
                    
                    

                    //------------------------------- Inicio Calculo de la distancia de la vis ----------------------------
                    //Calculo de distancia de la visibilidad
                    long double distancia =  sqrt((auxVis.u * auxVis.u) + (auxVis.v* auxVis.v));

                    //asignación de disco
                    int discoID = hashDisk(ancho_disco, cant_discos, distancia);
                    
                    chunkLeido++;
                    contador++;
                    
                    

                    // ------------------------ INICIO DE ACTUALIZAR PROPIEDADES ------------------------------

                    // hacemos el lock del mutex de escritura

                    pthread_mutex_lock(&mutexEscritura); 

                    Discos[discoID].u = Discos[discoID].u + auxVis.u;
                    Discos[discoID].v = Discos[discoID].v + auxVis.v;
                    Discos[discoID].real = Discos[discoID].real + auxVis.real;
                    Discos[discoID].img = Discos[discoID].img + auxVis.img;
                    Discos[discoID].ruido = Discos[discoID].ruido + auxVis.ruido;
                    Discos[discoID].potencia = Discos[discoID].potencia + sqrt((auxVis.real*auxVis.real) + (auxVis.img*auxVis.img));
                    Discos[discoID].contadorVis = Discos[discoID].contadorVis + 1;

                    
                    // hacemos unlock de el mutex de escritura
                    pthread_mutex_unlock(&mutexEscritura); 
                    
                    // ------------------------ FIN DE ACTUALIZAR PROPIEDADES ------------------------------

                }
                else{
                    // se da el unlock al mutex de lectura en caso de haber fallado la lectura evitando deathlock
                    
                    leido = 1;

                }
                
            }             
            
        }    
        pthread_mutex_unlock(&mutexLectura); 
        
        thread_rutine(NULL);   
        
    }
    else{
        pthread_exit(NULL);
        
    }
    pthread_exit(NULL);
}




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

    printf("\n################################## INICIO DEL PROGRAMA ##################################\n");

    //-----------------------------------------------------------------getopt
    char archivo_entrada[100];
    char archivo_salida[100];
    int variable_control, cant_hebras;
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

    //----------------------- declaracion de los strcut que contendran los valores de los discos --------------------    
    
    Discos = (Disk*)malloc(sizeof(Disk)*cant_discos);

    for (int i = 0; i < cant_discos; i++)
    {
        Discos[i].u = 0;
        Discos[i].v = 0;
        Discos[i].real = 0;
        Discos[i].img = 0;
        Discos[i].ruido = 0;
        Discos[i].potencia = 0;
        Discos[i].contadorVis = 0;
    }
    

    //----------------------- Inicialización de mutex  -------------------- 
      
    pthread_mutex_init(&mutexLectura, NULL);
    pthread_mutex_init(&mutexEscritura, NULL);

   
    //----------------------- creacion y declaracion de threads -------------------- 

    printf("--------INCIO DEL USO DE THREADS--------\n");

    pthread_attr_t attr[cant_hebras];
    for (int i = 0; i < cant_hebras; i++)
    {
        pthread_attr_init(&attr[i]);
    }
    //creación de hebras
    pthread_t thread[cant_hebras];


    for (int i = 0; i < cant_hebras; ++i)
    {    
        pthread_create(&thread[i], &attr[i], thread_rutine, NULL);        
        contadorH++;
    }
    

    
    //------------------------ Esperar a que terminen las threads ------------------

    for (int j = 0; j < cant_hebras ; ++j)
    {
        pthread_join(thread[j], NULL);
    }
    printf("--------FIN DEL USO DE THREADS--------\n");
    
    // cerramos el archivo .csv de lectura

    fclose(flujo);


    //-------------------------- escritura de resultados --------------------------


    //escritura de archivo de salida 

    escribirArchivoSalida(archivo_salida, cant_discos, Discos);

    
    // impresiones por consola para controlar los resultados

    if (variable_control == 1)
    {
        for (int i = 0; i < cant_discos; ++i){

            long double mediaReal = Discos[i].real/Discos[i].contadorVis;
            long double mediaImg = Discos[i].img/Discos[i].contadorVis;
            long double potencia = sqrt(Discos[i].potencia);
            printf("Disco %d:\n", i+1);
            printf("Media Real: %Lf\n",mediaReal);
            printf("Media imaginaria: %Lf\n",mediaImg);
            printf("Potencia: %Lf\n",potencia);
            printf("Ruido total: %Lf\n",Discos[i].ruido);
            
            
        }
    }

    // --------------------------- destruccion de los mutex ------------------------

    pthread_mutex_destroy(&mutexLectura);
    pthread_mutex_destroy(&mutexEscritura);
    free(Discos);

    printf("\n################################## FINAL DEL PROGRAMA ##################################\n");

    return 0;
}