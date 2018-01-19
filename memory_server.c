//EVERYTHING HERE IS TEST AND WILL BE MOVED TO FORKING SERVER WHEN FULLY TESTED
#include "headers.h"
int pids[];

//creating shared memory that is an array of the actual order to hitthe screens in
int shmdt = shmget(KEY, sizeof(int), IPC_CREAT | 0600);

//attaching to the data to the shared data space
int * shm = shmat(shmdt, 0, SHM_RDONLY);

shm = pids;




