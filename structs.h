#ifndef STRUCTS_H
#define STRUCTS_H

#define TRUE 1
#define FALSE 0

typedef struct {
    int process;
    int priority;
} structElement_t;

typedef struct {
    structElement_t * data;
    int size;
    int reserved;
}struct_t;

// int is_less(process_s *, process_s *);
// process_s *create_process_s(int, int, int);

void reserveQueue(struct_t * structQueue, int reserved);
void initQueue(struct_t * structQueue, int initialReserved);

#endif
