#include <stdio.h>
#include <stdlib.h>

#define MAX 30

int mediaMobile(int*, int, int, float*);

int main() {

    int i, k=0;
    int V[MAX]={0};
    float M[MAX]={.0};
    int v, m;

    printf("Vettore di partenza (max %d valori). 'x' per terminare:\n", MAX);

    // scansione fino al massimo o fino a quando si
    // inserice un valore non valido (carattere)
    for (i=0; i<MAX; i++) {
		printf("> ");
        if (!scanf("%d", &V[i])) {
            break;
        }
    }
    v=i;

	// annullo l'ultimo ritorno a capo
	getc(stdin);

    printf("Inserire il valore di K (minore o uguale a %d):\n> ", v);
    if (!scanf("%d", &k) || k>v) exit(1); //conotrollo su k

    m=mediaMobile(V, v, k, M);

	puts("Vettore media mobile:");
    for (i=0; i<m; i++){
		printf("%.3f\n", M[i]);
    }


    return 0;
}

int mediaMobile(int V[], int v, int k, float M[]) {
	int i,j,m=0;
	int m_tmp=0;

	for (i=0; i<=v-k; i++) {
		m_tmp=0;
        for (j=0; j<k; j++) {
            m_tmp+=V[i+j];
        }
		// aggiorno il vettore
        M[m]=(float)m_tmp/k;
        m++;
	}
	return m;
}
