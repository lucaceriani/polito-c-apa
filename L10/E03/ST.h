#ifndef ST_H_INCLUDED
#define ST_H_INCLUDED

typedef struct _ST *ST;

ST    STinit(int);
int   STinsert(ST, char*, char*, int);
void  STdump(ST);
int   STsearch(ST, char*);
char* STgetNome(ST, int);
char* STgetRete(ST, int);
int   STsameSubnet(ST, int, int);

#endif