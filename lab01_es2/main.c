#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 20
#define filename "testo_1.txt"

#define mass(a,b) (a>b?a:b)

int mat_search (char mat[N][N+2], int r, int c, char s[N]);
int real_column (char*);
char* inverti_parola(char*);

int main(){

	/*
		matrice dichiarata di +2 nelle colonne
		per il /0 e per il /n lasciati da fscanf
		in lettura da file
	*/
	char mat[N][N+2];
	FILE* fp;
	int r=0, c=0; // righe e colonne effettive
	char parola[N+1];

	fp=fopen(filename, "r");
    if (fp==NULL) printf("Errore! Impossibile aprire il file '%s'\n", filename), exit(-1);

    while(fgets(mat[r], N+2, fp)!=NULL) r++;

    /*
    conto le vere colonne della prima riga della matrice
    assumendola diu lunghezza pari alle sucessive
    */
    c=real_column(mat[0]);

	printf("Parola da cercare: ");
	scanf("%s", parola);

	if (strlen(parola)>mass(r,c)) puts("Errore! Parola troppo lunga!"), exit(-1);

	printf("%d occorenze della parola '%s' trovate!\n", mat_search(mat, r, c, parola), parola );

    return 0;
}

int mat_search(char mat[N][N+2], int r, int c, char s[N]) {
	int n=0, i, j, k;
	int len;
	len = strlen(s);
	char* s_inv=inverti_parola(s);

	// ricerca orizzontale
	for (i=0; i<r; i++) {
		k=0;
		for (j=0; j<c-len+1; j++) {
			while (mat[i][j+k]==s[k]) k++;
			if (k==len) n++;
			k=0;
			while (mat[i][j+k]==s_inv[k]) k++;
			if (k==len) n++;
			k=0;
		}
	}
	//ricerca verticale
	for (j=0; j<c; j++) {
		k=0;
		for (i=0; i<r-len+1; i++) {
			while (mat[i+k][j]==s[k]) k++;
			if (k==len) n++;
			k=0;
			while (mat[i+k][j]==s_inv[k]) k++;
			if (k==len) n++;
			k=0;
		}
	}

	return n;
}

char* inverti_parola(char parola[]) {
    int i,c;
    char tmp;
    char p[N+1];
    strcpy(p, parola);
    c=strlen(p)-1;
    for (i=0; i<=c/2;  i++) {
		tmp=p[i];
		p[i]=p[c-i];
		p[c-i]=tmp;
    }
    return p;
}

int real_column(char* mat) {
	/*
	real_column() sarebbe un metodo inutile se si sapessero le dimensioni
	della matrice a priori, non avendo questo dato, non mi viene il mente
	nessun'altra soluzione possibile.
	non voglio usare il metodo fscanf perché potrebbe andare a scrivere
	oltre la dimensione massima del vettore.
	*/

	int i=0, c;
	c=strlen(mat);
	while (mat[i]!='\n' && mat[i]!='\0' && i<c) i++;
	return i;
}
