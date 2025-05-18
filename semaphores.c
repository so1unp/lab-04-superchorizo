#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s [OPCION] [PARAMETRO]\n", argv[0]);
    fprintf(stderr, "Opciones:\n");
    fprintf(stderr, "\t-c semaforo valor\tcrea semáforo con el nombre y valor inicial indicado.\n");
    fprintf(stderr, "\t-u semaforo\t\trealiza un UP en el semáforo indicado.\n");
    fprintf(stderr, "\t-d semaforo\t\trealiza un DOWN en el semaforo indicado.\n");
    fprintf(stderr, "\t-b semaforo\t\telimina el semáforo indicado.\n");
    fprintf(stderr, "\t-i semaforo\t\tinformación del semáforo indicado.\n");
    fprintf(stderr, "\t-h\t\t\timprime este mensaje.\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    if (argv[1][0] != '-')
    {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    char option = argv[1][1];

    switch (option)
    {
    case 'c':
    
        if (argc < 4)
        {
            fprintf(stderr, "Uso: %s -c semaforo valor\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        int valor = atoi(argv[3]);

        sem_t *semaforo = sem_open(argv[2], O_CREAT, 0644, valor);

        if (semaforo == SEM_FAILED)
        {
            perror("sem_open");
            exit(EXIT_FAILURE);
        }

        printf("Semáforo '%s' creado con valor inicial %d\n", argv[2], valor);

        if (sem_close(semaforo) == -1)
        {
            perror("sem_close");
        }
        break;

    case 'u':
        // realiza un _up_
        break;
    case 'd':
        // realiza una operacion _down_
        break;
    case 'b':

        if (argc < 3)
        {
            fprintf(stderr, "Uso: %s -b <nombre_semaforo>\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        // abre el semaforo si y solo si existe.
        sem_t *sem = sem_open(argv[2], 0);
        if (sem == SEM_FAILED)
        {
            perror("sem_open");
            exit(EXIT_FAILURE);
        }

        // Se elimina el semaforo
        if (sem_unlink(argv[2]) != -1)
        {
            printf("Semáforo '%s' eliminado\n", argv[2]);
        }
        else
        {
            perror("sem_unlink");
        }

        if (sem_close(sem) == -1)
        {
            perror("sem_close");
        }
        break;
    case 'i':
        // informacion

        break;
    case 'h':
        usage(argv);
        break;
    default:
        fprintf(stderr, "Opción desconocida: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
