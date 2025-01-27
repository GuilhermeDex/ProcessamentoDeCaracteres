#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "../include/timer.h"

struct timeval start_time, end_time;
struct rusage usage;

void start_timer() {
    gettimeofday(&start_time, NULL);
}

void stop_timer() {
    gettimeofday(&end_time, NULL);
}

void print_timer() {
    getrusage(RUSAGE_SELF, &usage);

    printf("Tempo total: %ld.%06ld segundos\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
}

void print_timer_to_file(FILE *saida) {
    getrusage(RUSAGE_SELF, &usage);
    
    fprintf(saida, "Tempo total: %ld.%06ld segundos\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
}
