#include <stdio.h>
#include <stdlib.h>

#define N 20

void stampaVett(int*, int);
int linSearch(int, int*, int);
int inSort(int, int*, int); //ritorna la nuova lunghezza dell'array dopo l'inserzione

int* unione(int*, int, int*, int);
int* differenza(int*, int, int*, int);
int* intersezione(int*, int, int*, int);

int main() {
	int v1[N]={0}, v2[N]={0};
	int x;
	//int v_res[N*2];
	int len_v1=0;
	int len_v2=0;
	char c;
	int i;

	printf("Inserisci V1 ('x' per terminare):\n");
	for (i=0; i<N; i++) {
		if (scanf("%d", &v1[i])==0){
			if (scanf("%c", &c)==1 && c=='x'){
					break;
			} else {
				return -1;
			}
		}
	}
	len_v1=i;

	printf("Inserisci V2 ('x' per terminare):\n");
	for (i=0; i<N; i++) {
		if (scanf("%d", &v2[i])==0) {
			if (scanf("%c", &c)==1 && c=='x'){
					break;
			} else {
				return -1;
			}
		}
	}
	len_v2=i;

	stampaVett(v1, len_v1);
	stampaVett(v2, len_v2);

	unione(v1, len_v1, v2, len_v2);
	differenza(v1, len_v1, v2, len_v2);
	intersezione(v1, len_v1, v2, len_v2);


    return 0;
}

void stampaVett(int* v, int len) {

	int i;
	printf("-> ");
	for (i=0; i<len; i++) printf("%d ", v[i]);
	printf("\n");
}

int inSort(int x, int *v, int len) {
	int i, tmp;
	v[len]=x;
	len++;
	if (len==0) return 1;

	for (i=len-1; i>0; i--) {
		if (v[i]<v[i-1]) {
            tmp=v[i];
            v[i]=v[i-1];
            v[i-1]=tmp;
		} else {
			return len;
		}
	}
	return len;
}

int linSearch(int x, int *v, int len) {
	int i;
    for (i=0; i<len; i++) {
        if (v[i]==x) {
			return 1;
        }
    }
    return 0;
}

int* unione(int *v1, int v1_len, int *v2, int v2_len) {
	/*
	al peggio la soluzione ha il doppio	deglielementi
	del massimo vettore di partenza, cioe' 2*N
	*/
	int res[2*N];
	int len=0;
	int i;

    for (i=0; i<v1_len; i++) {
		if (linSearch(v1[i], res, len)==0) {
			len=inSort(v1[i], res, len);
		}
    }
    for (i=0; i<v2_len; i++) {
		if (linSearch(v2[i], res, len)==0) {
			len=inSort(v2[i], res, len);
		}
    }

	printf("U ");
	stampaVett(res, len);

	return res;
}

int* differenza(int *v1, int v1_len, int *v2, int v2_len) {

	int res[2*N];
	int len=0;
	int i;

    for (i=0; i<v1_len; i++) {
    	// controllo che v[i] non sia presente in res
		if (linSearch(v1[i], res, len)==0) {
			// se non è presente verifico che v1[i] non sia
			// presente in v2, per effettuare la differenza
			// tra insiemi
			if (linSearch(v1[i], v2, v2_len)==0)
				len=inSort(v1[i], res, len);
		}
    }

    for (i=0; i<v2_len; i++) {
    	if (linSearch(v2[i], res, len)==0) {
			if (linSearch(v2[i], v1, v1_len)==0)
				len=inSort(v2[i], res, len);
		}
    }

    printf("D ");
	stampaVett(res, len);

	return res;
}

int* intersezione(int *v1, int v1_len, int *v2, int v2_len) {
	int res[2*N];
	int len=0;
	int i;

    // devo cercare se c'e' in uno && c'e' nell'altro
    // quindi basta scorrere v1
    for (i=0; i<v1_len; i++) {
		// cerco che v[i] non sia prensente nella soluzione
        if (linSearch(v1[i], res, len)==0) {
			// deve essere presente in uno && nell'altro
            if (linSearch(v1[i], v2, v2_len)==1) {
                len=inSort(v1[i], res, len);
            }
        }
    }

	printf("I ");
	stampaVett(res, len);

    return res;
}
