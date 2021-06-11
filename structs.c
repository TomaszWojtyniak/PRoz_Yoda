#include <stdlib.h>
#include "structs.h"

// int is_less(process_s *p1, process_s *p2)
// {
//     return (p1->ts < p2->ts) || ((p1->ts == p2->ts) && (p1->id < p2->id));
// }

// process_s *create_process_s(int id, int ts, int data)
// {
//     process_s *p = malloc(sizeof(process_s));
//     p->id = id;
//     p->ts = ts;
//     p->data = data;
//     return p;
// }

void initQueue(struct_t * structQueue, int initialReserved)
{
    structQueue->reserved = 0;
    structQueue->size = 0;
    reserveQueue(structQueue, initialReserved);
}

void reserveQueue(struct_t * structQueue, int reserved)
{
    structElement_t * ptr = (structElement_t *)(malloc(sizeof(structElement_t)*reserved));
    for(int i = 0; i < structQueue->size; i++){
        ptr[i] = structQueue->data[i];
    }

    free(structQueue->data);
    structQueue->data = ptr;
    structQueue->reserved = reserved;
}
