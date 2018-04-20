#include "../include/cdata.h"

int cyield(void){
    return 0;
}

int ccreate (void* (*start)(void*), void *arg, int prio){
    return 1;
}

int cidentify (char *name, int size){
    char id_grupo[IDENTIFY] = "Leonardo Eich 242314\nFelipe Fischer 242264\nCaetano Jaeger 242309\n";

	if (size >= IDENTIFY){
		strcpy(name, id_grupo);
		return RETURN_OK;
	}

	return RETURN_ERROR;
}
