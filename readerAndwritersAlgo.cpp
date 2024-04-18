#include <iostream>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

// Initialize semaphores and variables
sem_t db, rcMutex;
int readCount = 0;

void* reader(void* arg) {
    int f = ((int*)arg)[0];
    sem_wait(&rcMutex);
    readCount++;
    if(readCount == 1) sem_wait(&db);
    sem_post(&rcMutex);

    // Reading Section
    cout << "Reader " << f << " is reading" << endl;

    sem_wait(&rcMutex);
    readCount--;
    if(readCount == 0) sem_post(&db);
    sem_post(&rcMutex);
}

void* writer(void* arg) {
    int f = ((int*)arg)[0];
    sem_wait(&db);

    // Writing Section
    cout << "Writer " << f << " is writing" << endl;

    sem_post(&db);
}

int main() {
    // Initialize semaphores
    sem_init(&db, 0, 1);
    sem_init(&rcMutex, 0, 1);

    pthread_t rtid[5], wtid[5];
    int i;
    for(i = 0; i < 5; i++) {
        pthread_create(&wtid[i], NULL, writer, (void*)&i);
        pthread_create(&rtid[i], NULL, reader, (void*)&i);
    }
    for(i = 0; i < 5; i++) {
        pthread_join(wtid[i], NULL);
        pthread_join(rtid[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&db);
    sem_destroy(&rcMutex);

    return 0;
}
