#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>

#define f_seq "sequenze.txt"
#define f_tes "testo.txt"

#define MIN(a,b)(((a)<(b))?(a):(b))

#define maxriga 200
#define maxparola 25
#define maxn_seq 20
#define maxseq 5

typedef struct l{
	char parola[maxparola+1];
	struct l *p;
};

struct l *list_init();
//void visualizza_lista(struct l*);
void add_element(struct l*, char*);

int startsWhith(char*, char*); // comincia con...?

void visualizza_lista(struct l *lista) {
    while(1337) {
		printf("%s\n", lista->parola);
		if (lista->p!=NULL) {
			lista = lista->p;
		} else {
			break;
		}
    }
}

int main(){

    char p[maxparola+1];			// parola considerata per la ricerca
    char seq[maxn_seq][maxseq];		// sequenze
    char c;
    int n_seq=0, i=0, seq_trov=0;
    int parole, lettera, in_parola;
	FILE *fSeq, *fTes;
    struct l *list_start;
    struct l *lista;

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

    //inizializzazione lista
    list_start=list_init();

    // inizializzazione variabili (si poteva fare nella dichiarazione ma secondo
	// me era più chiaro inizializzare vicino al codiche che le avrebbe usate)
	in_parola=0;
	lettera=0;
	parole=0;

    // acquisizione di tutte le parole nella lista
    while (fscanf(fTes, "%c", &c)==1) {

			// se sono alla fine di una parola
			if ((isspace(c) || ispunct(c)) && in_parola==1) {

				// termino la parola correttamente
				// N.B.: 'lettera' è già uno piu' grande dell'indice dell'ultima lettera
				p[lettera]='\0';

				// aggiungo la parola alla lista
				add_element(list_start, p);

				// aumento il conteggio delle parole
			 	parole++;

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


		//visualizza_lista(list_start);


    // for tra tutte le sequenze possibili
    for (i=0; i<n_seq; i++) {



		//mi porto all'inizio della lista
		lista=list_start;
		seq_trov=0;

		// trovo almeno un'occorrenza della sequenza per stampare
		// il titolo della sequenza
		while (lista->p!=NULL && seq_trov==0) {
			if (startsWhith(lista->parola, seq[i])) {
				seq_trov=1;
			}
			lista=lista->p;
		}

		if (seq_trov==1) {
			printf("%s\n", seq[i]);
			lista=list_start;
		 } else {
			lista->p=NULL;
		 }


		//while tra gli elementi della lista
		parole=0;
		while (lista->p!=NULL) {
			if (startsWhith(lista->parola, seq[i])) {
				printf("%s %d\n", lista->parola, parole);
			}
			lista=lista->p;
			parole++;
		}
		printf("\n");
    }

	printf("Parole totali: %d\n", parole);

    fclose(fTes);

    getc(stdin);

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

struct l *list_init() {
	struct l *lista;
    lista=(struct l*)malloc(sizeof(struct l));
    lista->p=NULL;
    lista->parola[0]='-';
    return lista;
}

void add_element(struct l *lista, char *s) {
    //trova l'ultimo
    struct l *punt;
	while (lista->p!=NULL) {
		lista = lista->p;
	}
    punt=(struct l*)malloc(sizeof(struct l));
    lista->p=punt;
    strcpy(punt->parola, s);
    punt->p=NULL;

}
