#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>




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


    return 0;
}