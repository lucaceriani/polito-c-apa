#include <stdio.h>
#define N 5

int main() {

    float sommaColonne[N]={0};
    float sommaRiga=0;
    int numeroRighe=0;
    int n[5];
    int i=0;
    FILE *fp;

    fp=fopen("testo.txt","r");

    if (fp==NULL) {
		printf("Errore nell'apertura del file");
		return -1;
    }

    while(fscanf(fp,"%d %d %d %d %d",&n[0],&n[1],&n[2],&n[3],&n[4])!=EOF) {
		numeroRighe++;
		sommaRiga=0;

		for (i=0;i<5;i++) {
            sommaColonne[i]+=n[i];
            sommaRiga+=n[i];
		}
		printf("Media riga %d\t%6.1f\n", numeroRighe, sommaRiga/N);
    }
    fclose(fp);

    for (i=0; i<numeroRighe; i++) {
		printf("Media colonna %d\t%6.1f\n", i+1, sommaColonne[i]/N);
    }

    return 0;
}
