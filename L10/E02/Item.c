#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Item.h"

struct _item {
    // l'indice di volte è l'indice della stazione nel main
    // per esempio la stazione con indice 17 sarà stata usara volte[17] volte
    int *volte;
    char *id;
    int noleggioAttivo;
    int tempo;    
};

Item ITEMnew (char *id, int nolAtt, int tempo, int nStaz){
    Item tmp=malloc(sizeof(struct _item));
    if (tmp == NULL) return ITEMsetvoid(); // controllo malloc
    tmp->volte=calloc(nStaz,sizeof(int));
    tmp->id=strdup(id);
    tmp->noleggioAttivo=nolAtt;
    tmp->tempo=tempo;
    return tmp;
}

void ITEMaggiornaStazione(Item data, int stationIndex) {
    data->volte[stationIndex]++;
}

int ITEMgetNoleggio(Item data) {
    return data->noleggioAttivo;
}

void ITEMsetNoleggio(Item data, int nol) {
    data->noleggioAttivo=nol;
}

/*
Item ITEMscan() {
  char name[MAXC];
  int value;

  printf("name = ");
  scanf("%s", name);
  printf("value = ");
  scanf("%d", &value);

  Item tmp = (Item) malloc(sizeof(struct item));
  if (tmp == NULL)
    return ITEMsetvoid();

  else {
    tmp->name = strdup(name);
    tmp->value = value;
  }
  return tmp;
}
*/
void ITEMshow(Item data) {
  printf("userID: %3s inNoleggio: %d tempo: %d\n", data->id, data->noleggioAttivo, data->tempo);
}

int ITEMcheckvoid(Item data) {
  Key k1, k2="";
  k1 = KEYget(data);
  if (KEYcompare(k1,k2)==0)
    return 1;
  else
    return 0;
}

Item ITEMsetvoid() {
  return NULL;
}

int ITEMless (Item data1, Item data2) {
  Key k1 = KEYget(data1), k2 = KEYget(data2);
  if (KEYcompare(k1, k2) == -1) return 1;
  else return 0;
}

int KEYcompare(Key k1, Key k2) {
  return strcmp(k1, k2);
}

Key KEYget(Item data) {
  return data->id;
}
