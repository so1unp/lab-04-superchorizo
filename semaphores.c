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

        if (argc < 4){
            fprintf(stderr, "Uso: %s -c semaforo valor\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        int valor = atoi(argv[3]);
        // se crea el semaforo
        sem_t *sem_o = sem_open(argv[2], O_CREAT, 0644, valor);

        if (sem_o == SEM_FAILED){
            perror("sem_open");
            exit(EXIT_FAILURE);
        }

        printf("Semáforo '%s' creado con valor inicial %d\n", argv[2], valor);

        if (sem_close(sem_o) == -1){
            perror("sem_close");
        }
        break;
    case 'u':

        if (argc < 3) {
            fprintf(stderr, "Uso: %s -u <nombre_semaforo>\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        // abre el semaforo si y solo si existe.
        sem_t *sem_u = sem_open(argv[2], 0);

        if (sem_u == SEM_FAILED) {
            perror("sem_open");
            exit(EXIT_FAILURE);
        }
        // aca se realizaria el UP
        if (sem_post(sem_u) != -1) {
            printf("Se realizó UP sobre el semáforo '%s'\n", argv[2]);
        } else {
            perror("sem_post");
        }

        if (sem_close(sem_u) == -1) {
            perror("sem_close");
        }
        break;
    case 'd':

        if (argc < 3) {
            fprintf(stderr, "Uso: %s -d <nombre_semaforo>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        // abre el semaforo si y solo si existe.
        sem_t *sem_d = sem_open(argv[2], 0);

        if (sem_d == SEM_FAILED) {
            perror("sem_open");
            exit(EXIT_FAILURE);
        }

        // se realiza DOWN
        if (sem_wait(sem_d) != -1) {
            printf("Se realizó DOWN sobre el semáforo '%s'\n", argv[2]);
        } else {
            perror("sem_wait");
        }

        if (sem_close(sem_d) == -1) {
            perror("sem_close");
        }
        break;
    case 'b':

        if (argc < 3) {
            fprintf(stderr, "Uso: %s -b <nombre_semaforo>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        // abre el semaforo si y solo si existe.
        sem_t *sem = sem_open(argv[2], 0);
        if (sem == SEM_FAILED) {
            perror("sem_open");
            exit(EXIT_FAILURE);
        }
        // Se elimina el semaforo
        if (sem_unlink(argv[2]) != -1) {
            printf("Semáforo '%s' eliminado\n", argv[2]);
        } else {
            perror("sem_unlink");
        }

        if (sem_close(sem) == -1) {
            perror("sem_close");
        }
        break;
    case 'i':

        if (argc < 3) {
            fprintf(stderr, "Uso: %s -i <nombre_semaforo>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        // abre el semaforo si y solo si existe.
        sem_t *sem_i = sem_open(argv[2], 0);
        if (sem_i == SEM_FAILED) {
            perror("sem_open");
            exit(EXIT_FAILURE);
        }
        // aca te dice la informacion actual del semaforo
        int valor_actual;
        if (sem_getvalue(sem_i, &valor_actual) != -1) {
            printf("El valor actual del semáforo '%s' es: %d\n", argv[2], valor_actual);
        } else{
            perror("sem_getvalue");
        }

        if (sem_close(sem_i) == -1) {
            perror("sem_close");
        }
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
