//
// Problema del búfer limitado.
//
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <semaphore.h>

static void *producer(void *);
static void *consumer(void *);

struct buffer {
    int size;
    int *buf;
};

struct params {
    int wait_prod;
    int wait_cons;
    int items;
    struct buffer *buf;
    pthread_mutex_t mutex;
    sem_t empty_slots;
    sem_t filled_slots;
} params_t;

/* Productor */
static void *producer(void *p) {
    int i = 0;

    struct params *params = (struct params *)p;

    for (i = 0; i < params->items; i++) {
        sem_wait(&params->empty_slots);
        pthread_mutex_lock(&params->mutex);

        params->buf->buf[i % params->buf->size] = i;
        // Espera una cantidad aleatoria de microsegundos.
        pthread_mutex_unlock(&params->mutex);
        sem_post(&params->filled_slots);

        // usleep(rand() % params->wait_prod);
        usleep((useconds_t)(rand() % params->wait_prod));
    }

    pthread_exit(0);
}

/* Consumidor */
static void *consumer(void *p) {
    int i;
    struct params *params = (struct params *)p;

    // Reserva memoria para guardar lo que lee el consumidor.
    int *reader_results = (int *)malloc(sizeof(int) * (size_t)params->items);

    for (i = 0; i < params->items; i++) {
        sem_wait(&params->filled_slots);
        pthread_mutex_lock(&params->mutex);

        reader_results[i] = params->buf->buf[i % params->buf->size];

        pthread_mutex_unlock(&params->mutex);
        sem_post(&params->empty_slots);

        // Espera una cantidad aleatoria de microsegundos.
        // usleep(rand() % params->wait_cons);
        usleep((useconds_t)(rand() % params->wait_cons));
    }

    // Imprime lo que leyo
    for (i = 0; i < params->items; i++) {
        printf("%d ", reader_results[i]);
    }
    printf("\n");

    pthread_exit(0);
}

int main(int argc, char **argv) {
    pthread_t producer_t, consumer_t;

    // Controla argumentos.
    if (argc != 5) {
        fprintf(stderr, "Uso: %s size items wait-prod wait-cons rand\n", argv[0]);
        fprintf(stderr, "\tsize:\ttamaño del buffer.\n");
        fprintf(stderr, "\titems:\tnúmero de items a producir/consumir.\n");
        fprintf(stderr, "\twaitp:\tnúmero de microsegundos que espera el productor.\n");
        fprintf(stderr, "\twaitc:\tnúmero de microsegundos que espera el consumidor.\n");
        exit(EXIT_FAILURE);
    }

    struct buffer *buf;
    buf = (struct buffer *)malloc(sizeof(struct buffer));
    if (buf == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Tamaño del buffer.
    buf->size = atoi(argv[1]);
    if (buf->size <= 0) {
        fprintf(stderr, "bufsize tiene que ser mayor que cero.\n");
        exit(EXIT_FAILURE);
    }

    // Crea el buffer
    buf->buf = (int *)malloc(sizeof(int) * (size_t)buf->size);
    if (buf->buf == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Instancia una estructura de parámetros
    struct params *params;
    params = (struct params *)malloc(sizeof(struct params));
    if (params == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    params->buf = buf;

    // Cantidad de items a producir.
    params->items = atoi(argv[2]);
    if (params->items <= 0) {
        fprintf(stderr, "counter tiene que ser mayor que cero.\n");
        exit(EXIT_FAILURE);
    }

    params->wait_prod = atoi(argv[3]);
    if (params->wait_prod <= 0) {
        fprintf(stderr, "wait-prod tiene que ser mayor que cero.\n");
        exit(EXIT_FAILURE);
    }

    params->wait_cons = atoi(argv[4]);
    if (params->wait_cons <= 0) {
        fprintf(stderr, "cons-wait tiene que ser mayor que cero.\n");
        exit(EXIT_FAILURE);
    }

    // Inicializa semilla para números pseudo-aleatorios.
    // srand(getpid());
    srand((unsigned int)getpid());

    pthread_mutex_init(&params->mutex, NULL);
    // sem_init(&params->empty_slots, 0, buf->size);
    sem_init(&params->empty_slots, 0, (unsigned int)buf->size);
    sem_init(&params->filled_slots, 0, 0);
    // Crea productor y consumidor
    pthread_create(&producer_t, NULL, producer, params);
    pthread_create(&consumer_t, NULL, consumer, params);

    // Esperar a que los hilos terminen
    //  pthread_join(producer_t, NULL);
    //  pthread_join(consumer_t, NULL);

    // Destruir mutex y semáforos
    //  pthread_mutex_destroy(&params->mutex);
    //  sem_destroy(&params->empty_slots);
    // sem_destroy(&params->filled_slots);

    // Liberar memoria
    // free(buf->buf);
    // free(buf);
    // free(params);

    // Mi trabajo ya esta hecho ...
    pthread_exit(NULL);
}
