#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

void* thread_routine(void *arg){
    int nr_lines = ((int*) arg);
    char buf[] = "Nueva linea \n";
    printf("El hilo comienza a ejecutarse...\n");
    
    for (int i = 0; i < nr_lines; i++)
    {
        int a = open("archivoTest0.txt", O_WRONLY|O_APPEND);
        write(a, buf, sizeof(buf)-1);
        close(a);
    }
    
    
}
int main(int argc, char *argv[])
{
    pthread_t thread1;
    int value= atoi(argv[1]);
    // thread - atributos - rutina que se ejecuta al iniciar
    if( 0 != pthread_create(&thread1, NULL, thread_routine, &value)){
        return -1;
    }    

    //espera a que hilo 1 finalice
    pthread_join(thread1, NULL);
    return 0;
}

