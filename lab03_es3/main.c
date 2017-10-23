#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_M 50

FILE *apriFile(char*, char*);
int checkValido();

int main(int argc, char **argv) {

	FILE *fMap, *fSpo;
	char m[MAX_M][MAX_M];
	char s[MAX_M+2];
	int nr=0, nc=0;
	int mv, mo; //mossa verticale, mossa orizzontale
	int i,j, I, J;
	int ing[]={-1, -1}, usc[]={-1, -1}; // posizione dell'ingresso e dell'uscita
	int valido, semplice, lunghezza;

	// controllo numero dei parametri
	if (argc!=3) puts("Errore! I parametri sono <file_mappa> <file_spostamenti>!"), exit(1);

	fMap=apriFile(argv[1], "r");

	// controllo validità nr e nc
	if (fscanf(fMap, "%d%d\n", &nr, &nc)!=2) // fondamentale lo '\n' per dopo!!
		puts("Errore! File di mappa non formattato correttamente!"), exit(1);
	if (nr>MAX_M)
		printf("Attenzione! Dimensioni della tabella massimo %dx%d", MAX_M, MAX_M), nr=MAX_M;
	if (nc>MAX_M)
		printf("Attenzione! Dimensioni della tabella massimo %dx%d", MAX_M, MAX_M), nc=MAX_M;

	printf("Lettura di una matrice %dx%d...\n", nr, nc);

	// acquisizione valori nella matrice (come righe)
	for (i=0; i<nr; i++) {
		fgets(s, MAX_M+2, fMap);
		sscanf(s, "%s", m[i]);
	}

    fclose(fMap);

	// la ricerca dell'ingresso e dell'uscita potevo evitarla se avessi letto il file
	//	carattere per carattere e controllato ogni volta
	for (i=0; i<nr; i++){
		for (j=0; j<nc; j++) {
            if (m[i][j]=='I')
				ing[0]=i, ing[1]=j;
			else if (m[i][j]=='U')
            	usc[0]=i, usc[1]=j;
		}
	}

	if (ing[0]==-1 || usc[0]==-1) puts("Errore! Impossibile trovare l'ingresso o l'uscita del percorso!"), exit(1);

    // lettura del percorso
    fSpo=apriFile(argv[2], "r");

	// per capire se il cammino è semplice sostituisco il carattere della
	// cella che sto visitando con '#'se passo sopra un '#' allora semplice=0
	semplice=1;
	valido=1;
	lunghezza=0;

	// mi posiziono all'ingresso del labirinto
	I=ing[0], J=ing[1];

    while (fscanf(fSpo, "%d %d", &mo, &mv)==2 && valido) {

		i=0, j=0;

		while (mo!=0 && abs(j)<abs(mo) && valido) {
			// incremento o decremento a seconda del segno
			j+=(mo/abs(mo));
			// controllo
			if (toupper(m[I][J+j])=='X' || I+i>nr) valido=0;
			if (m[I][J+j]=='#') semplice=0;

			m[I][J+j]='#';
			lunghezza++;
		}
		// salvo la posizione
		J+=j;

		i=0, j=0;
		while (mv!=0 && abs(i)<abs(mv) && valido) {
			// col meno si scende, non si sale!
			i+=-(mv/abs(mv));
			if (toupper(m[I+i][J])=='X' || I+i>nr) valido=0;
			if (m[I+i][J]=='#') semplice=0;
			m[I+i][J]='#';
			lunghezza++;
		}
		I+=i;
    }

    // se non mi sono fermato all'uscita il percorso non è valido
    if (usc[0]!=I || usc[1]!=J) valido=0;

    printf("Il cammino%se' valido\n", valido?": - ":" non ");

    // se non è valido esco
    if(!valido) return 0;

    printf("            - %se' semplice\n", semplice?"":"non ");
    printf("            - ha lunghezza %d", lunghezza);

    return 0;
}

FILE *apriFile(char *nome, char *modo) {
	FILE *fp;
	fp=fopen(nome, modo);
	if (fp==NULL) {
			printf("Errore! Impossibile aprire il file \"%s\"!\n", nome);
			exit(1);
	} else {
		return fp;
	}
}
