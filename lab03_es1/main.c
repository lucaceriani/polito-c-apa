#include <stdio.h>
#include <stdlib.h>

#define max 50
#define n_file "mat.txt"

int main() {

	int r,c;
	int i, j, b, h, p, q;
	int m[max][max];
	int in_area;
	int regioni_trovate=0;
	FILE *fp;

	// apertura e controllo del file
    fp=fopen(n_file, "r");
	if (fp==NULL) printf("Errore! Impossibile aprire il file \"%s\"!", n_file), exit(1);

    fscanf(fp, "%d%d", &r, &c);

    // acquisizione della matrice
	for (i=0; i<r; i++) {
        for (j=0; j<c; j++) {
            fscanf(fp, "%d", &m[i][j]);
        }
	}

	// ricerca all'interno della matrice
    for (i=0; i<r; i++){
		for (j=0; j<c; j++) {

				// il trucco è impostare a -1 le caselle già visitate
				if (m[i][j]==1) {
					b=1; h=1;
					m[i][j]=-1;
					// ricerca dei limite dell'area appena trovata

					// ricerca orizozntale bordo
					while (m[i][j+b]==1 || j+b==c-1) {
						m[i][j+b]=-1;
						b++;
					}

					// ricerca verticale bordo
					while (m[i+h][j]==1 || i+h==r-1){
						m[i+h][j]=-1;
						h++;
					}

					// riempio lo spazio nei bordi che ho trovato
					for (p=i; p<i+h; p++) {
						for (q=j; q<j+b; q++)
							m[p][q]=-1;
					}

					printf("Regione %d: estr. sup. SX = <%d,%d>, b = %d, h = %d, Area = %d\n",
							++regioni_trovate, i, j, b, h, b*h);
				}
            }
		}

	return 0;
}
