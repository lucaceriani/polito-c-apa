#include <stdio.h>
#include <stdlib.h>

#define MAXR 50
#define NOME_FILE "mat.txt"

void leggiMatrice(int[MAXR][MAXR], int, int*, int*);
int riconosciRegione(int[MAXR][MAXR], int, int, int, int, int*, int*);

int main() {

	int nr=0,nc=0;
	int i, j, b, h;
	int m[MAXR][MAXR];
	int in_area;
	int regioni_trovate=0;

	leggiMatrice(m,MAXR,&nr,&nc);
	printf("%d, %d\n",nr,nc);
	for (i=0; i<nr; i++){
		for (j=0; j<nc; j++) {
            printf("%d ", m[i][j]);
		}
		puts("");
	}


	// ricerca all'interno della matrice
    for (i=0; i<nr; i++){
        for (j=0; j<nc; j++) {
            if (riconosciRegione(m,nr,nc,i,j,&b,&h)) {
                printf("Regione %d: estr. sup. SX = <%d,%d>, b = %d, h = %d, Area = %d\n",
                        ++regioni_trovate, i, j, b, h, b*h);
            }

        }
    }

	return 0;
}

void leggiMatrice(int m[MAXR][MAXR], int max, int* nr, int* nc) {

    int i,j;
    char s[2*max+1];
    FILE *fp;

    // apertura e controllo del file
    fp=fopen(NOME_FILE, "r");
	if (fp==NULL) printf("Errore! Impossibile aprire il file \"%s\"!", NOME_FILE), exit(1);

    // ignnoro i valori delle dimensioni all'inizio del file
    // importante il ritorno a capo!!
    fscanf(fp, "%*d%*d\n");

    // mi servono a tenere traccia di dove è arrivata la sscanf
    int n=0, n_letti=0;

    i=0;
    while (fgets(s, 2*max, fp)!=NULL && i<max) {
        j=0;
        n_letti=0;
        while (sscanf(s+n_letti, "%d%n", &m[i][j], &n)!=EOF && j<max) {
            n_letti+=n; // salvo la nuova posizione dall'inio della riga
            j++;
        }
        i++;
    }

    // salvo le righe e le colonne nei puntatori passati
    *nr=i; *nc=j;

    fclose(fp);
}

int riconosciRegione(int m[MAXR][MAXR], int nr, int nc, int r, int c, int *p_b, int *p_h) {

    int i, j, b, h, p, q;
    i=r; j=c;

    // il trucco è impostare a -1 le caselle già visitate
    if (m[i][j]==1) {
        b=1; h=1;
        m[i][j]=-1;
        // ricerca dei limite dell'area appena trovata

        // ricerca orizozntale bordo
        while (m[i][j+b]==1 || j+b==nc-1) {
            m[i][j+b]=-1;
            b++;
        }

        // ricerca verticale bordo
        while (m[i+h][j]==1 || i+h==nr-1){
            m[i+h][j]=-1;
            h++;
        }

        // riempio lo spazio nei bordi che ho trovato
        for (p=i; p<i+h; p++) {
            for (q=j; q<j+b; q++)
                m[p][q]=-1;
        }

        // assegnazione ai puntatori
        *p_b=b; *p_h=h;

        return 1;
    }

    return 0;
}

//fine
