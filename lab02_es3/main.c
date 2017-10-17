#include <stdio.h>
#include <stdlib.h>

#define N 20
#define f "matrice.txt"

int sommaMatrice(int [N][N], int, int, int, int);

int main() {

    int m[N][N];
    int nr, nc, r, c;
	int i,j;
	FILE *fp;

	// apertira e controllo del file
    fp=fopen(f, "r");
    if (fp==NULL) printf("Errore! Impossibile aprire il file \"%s\"\n", f), exit(1);

    // lettura delle dimensioni della matrice con controllo abbreviato
    fscanf(fp, "%d %d", &nr, &nc)!=2?exit(2):0;

    printf("Matrice %d x %d\n\n", nr, nc);

    // lettura e stampa della matrice
    for (i=0; i<nr; i++) {
		for (j=0; j<nc; j++) {
			fscanf(fp, "%d", &m[i][j]);
			printf("%5d", m[i][j]);
		}
		printf("\n");
    }

    // while non strutturato per evitare le cose noiose dei menu'
    while (1337) {
		puts("\n\nInserici le coordinate:");

		// controllo abbreviato, come sopra
		scanf("%d %d", &r, &c)!=2?exit(2):0;

		// controllo uscita del while
		if (r<0 || c<0 || r>nr || c>nc) break;

		printf("NO = %2d\n", sommaMatrice(m,0,r-1,0,c-1));
		printf("NE = %2d\n", sommaMatrice(m,0,r-1,c+1,nc-1));
		printf("SO = %2d\n", sommaMatrice(m,r+1,nr-1,0,c-1));
		printf("SO = %2d\n", sommaMatrice(m,r+1,nr-1,c+1,nc-1));

		// riga orizzontale
		for (i=0, printf("\n"); i<80; printf("-"), i++);
    }

	fclose(fp);

    return 0;
}

int sommaMatrice(int m[N][N], int rs, int re, int cs, int ce){ // riga start, riga end ...
	int sum=0;
	int i,j;

    for (i=rs; i<=re; i++) {
		for (j=cs; j<=ce; j++) {
			sum+=m[i][j];
		}
    }

	return sum;
}

