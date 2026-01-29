#include <stdio.h>
#include <stdlib.h>

typedef struct _Obj {
    void *current;
    void * next;
    void (*callback)();
} Obj;

void legit() {
    printf("Hello\n");
}

void uaf() {
    printf("UAF\n");
}

int main() {

    int size = 10;

    Obj *spray[10];

    for(int i = 0; i < size; i++) {
        spray[i] = malloc(sizeof(Obj));
        spray[i]->current = NULL;
        spray[i]->next = NULL;
        spray[i]->callback = legit;
    }

    for(int i = 0; i < size-1; i++) {
        spray[i]->next = spray[i+1];
        spray[i]->current = spray[i];
    }
    spray[size-1]->next = NULL; // breaks the loop by null the last ptr


    // free the spray we dont NULL the ptrs for UAF here
    for(int i = 0; i < size; i++) {
        free(spray[i]);
    }

    
    Obj *evil = malloc(sizeof(Obj)); // create evil obj for uaf
    evil->callback = uaf;           // we put the uaf func on the callback on the evil obj, in hope that evil obj will be in a spray space
    evil->next = NULL;              // nullify the next to stop pointer chasing so we have a control

    Obj *p = spray[0];
    // Loop on the spray for info
    while(p) {
        printf("Current: %p | Next: %p | Callback: %p\n", p->current, p->next, p->callback);
        p->callback();  // we call the callback func on spray object, but i MAY have been UAF by the evil
        p = p->next;
    }

    return 0;
    
}