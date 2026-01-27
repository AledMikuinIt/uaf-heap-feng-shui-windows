#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Obj {   // struct Obj that handle a name & callback (for UAF & feng shui)
    char name[32];
    void (*callback)(); // here is what is interesting
} Obj;

void legit() {      // random normal func for testing
    puts("legit callback");
}


int main() {

    Obj *o = malloc(sizeof(Obj));   // create the 1st obj on the heap
    strcpy(o->name, "Hello");   // random writing on o name
    o->callback = legit;        // legit function on o callback
    printf("Initial Addr: %p\n", o->callback); 

    Obj *spray[8096];   // create 8096 obj for spraying that can handle caching, segmentation heap, etc.
    for (int i = 0; i < 8096; i++) {
        spray[i] = malloc(sizeof(Obj));
    }

    free(o);    // free the initial obj for having a open space

    for (int i = 0; i < 8096; i++) {    // we free them for no memeory leaks & clean heap (for reuse)
        free(spray[i]);
    }

    Obj *p = malloc(sizeof(Obj));               // alloc new obj for UAF
    p->callback = (void*)0x4141414141414141;    // force overwrite on callback address, can replace by real function or w.e
    printf("After Free Addr: %p\n", o->callback);
    printf("After Free Name: %s\n", o->name);
    o->callback();  // call the function from the address, can be dangerous // its here that the real exploit it





    return 0;
}