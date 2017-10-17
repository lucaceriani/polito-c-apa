#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define f_seq "sequenze.txt"
#define f_tes "testo.txt"

#define MIN(a,b)(((a)<(b))?(a):(b))

#define maxriga 200
#define maxparola 25
#define maxn_seq 20
#define maxseq 5

int startsWhith(char*, char*); // comincia con...?

int main(){

    char p[maxparola+1];			// parola considerata per la ricerca
    char seq[maxn_seq][maxseq];		// sequenze
    char c;
    int n_seq=0, i=0;
    int parole, lettera, in_parola;
	FILE *fSeq, *fTes;

    fSeq=fopen(f_seq, "r");
    if (fSeq==NULL) printf("Errore! Impossibile aprire il file '%s'\n", f_seq), exit(1);

	// leggo quante sequenze ci sono nel file
    fscanf(fSeq, "%d", &n_seq);

    // non si svuole sforare il limite imposto dalla prima riga del file delle sequenze
	while(fscanf(fSeq, "%s", seq[i])==1 && i<n_seq) i++;

	// controllo sulla correttezza dei dati
	if (i<n_seq){
		printf("Warning! Lette solamente %d sequenze su %d dichiarate in '%s'\n", i, n_seq, f_seq);
		n_seq=i;
	} // nessun caso else, se ce ne sono di più di qunto dichiarate a inizio file non le considero

    fclose(fSeq);

	// apro e controllo l'apertura
    fTes=fopen(f_tes, "r");
    if (fTes==NULL) printf("Errore! Impossibile aprire il file '%s'\n", f_tes), exit(1);


    // for tra tutte le sequenze possibili
    for (i=0; i<n_seq; i++) {
    	/*
			Per stamapare i risultati come è visualizzato nell'esempio
			o si salvano tutte le parole che cominciano con una certa sequenza
			in un vettore, cosa che però mi riesce difficile non sapendo quante
			parole potrei trovare, oppure si perde tempo a scansionare il file
			n_seq volte dall'inizio con la funzione rewind. Ho scelto quest'ultima
			soluzione per evitare errori di tipo 'segmentation fault'.
    	*/
		rewind(fTes);

		// inizializzazione variabili (si poteva fare nella dichiarazione ma secondo
		// me era più chiaro inizializzare vicino al codiche che le avrebbe usate)
		in_parola=0;
		lettera=0;
		parole=0;

		printf("%s\n", seq[i]);

		while (fscanf(fTes, "%c", &c)==1) {

			// se sono alla fine di una parola
			if ((isspace(c) || ispunct(c)) && in_parola==1) {

				// termino la parola correttamente
				p[lettera]='\0';

				// aumento il conteggio delle parole
			 	parole++;

				// controllo le la parola appartiene alla sequenza seq[i]
				if (startsWhith(p, seq[i])) {
					printf("%s %d\n", p, parole);
				}

				in_parola=0;
				lettera=0;

			// se è un carattere alfanumerico
			} else if (isalnum(c)) {

				// se è il primo della parola
				if (in_parola==0) {
					in_parola=1;
				}

				p[lettera]=tolower(c);
				lettera++;
			}
		}
		printf("\n");
    }

	printf("Parole totali: %d", parole);

    fclose(fTes);

    return 0;
}

// funzione che ritorna se la stringa 'a' comincia con la stringa 'b'
// ignorando maiuscole e minuscole
int startsWhith(char *a, char *b) {
    int i, n=0;
    // voglio proseguire il confronto fino all'ultima
    // lettera della parola più corta
    n=MIN(strlen(a), strlen(b));

    for (i=0; i<n; i++) {
		if (tolower(a[i])!=tolower(b[i])) {
			return 0;
		}
    }
    return 1;
}

