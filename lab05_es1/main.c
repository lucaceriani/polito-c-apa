#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NOME_FILE "atleti.txt"
#define MAX_NOME 25
#define LUNG_CODICE 5
#define non_strutturato ;;
#define MAX_PERCORSO_FILE 100

#define MIN(a,b) (((a)<(b))?(a):(b))

#ifdef _WIN32
    #define F_CLEAR "cls"
    #define _comp(a,b) stricmp(a,b)
#else
    #define F_CLEAR "clear"
    #define _comp(a,b) strcasecmp(a,b)
#endif


typedef struct {
    char *nome;
    char *cognome;
    char *cognomenome;
    char *categoria;
    char codice[LUNG_CODICE+1];
    char data[11];
    int ore;
} atleta_t;

typedef struct {
    char nome[MAX_NOME+1];
    char cognome[MAX_NOME+1];
    char categoria[MAX_NOME+1];
    char codice[LUNG_CODICE+1];
    char data[11];
    int ore;
} atleta_temp_t;

typedef enum {
    data, nome, codice, categoria
} campo_e;

void minuscola(char*);
void maiuscola(char*);
void stampaAnagrafica(atleta_t*, int, FILE*);
void reverseDate(char*);
void insetionSort(atleta_t*, int, campo_e);
int sonoOrdinati(char*, char*);
int startsWith(char*, char*); // se a comincia con b;
int ricercaDicotomica(atleta_t*, int, char*, campo_e);
int ricercaLineare(atleta_t*, int, char*, campo_e);
char *getCampo(atleta_t*, int, campo_e);
int main() {

    FILE *fp;
    atleta_t *atleti;
    atleta_temp_t tmp;
    int scelta=-1;
    int n=0, i=0;
    campo_e ordinato=-1;
    char c[LUNG_CODICE+1], p[MAX_PERCORSO_FILE+1]; // variabili riutilizzate per l'input utente

    // apretura e controllo file
    if ((fp=fopen(NOME_FILE, "r"))==NULL){
        printf("Errore! Impossibile aprire il file \"%s\"!\n", NOME_FILE);
        exit(1);
    }

    // controllo della presenza e della correttezza della prima riga del file
    if (fscanf(fp, "%d\n", &n)!=1) exit(2);

    // alloco il vettore delle strutture
    atleti=(atleta_t*)malloc(n*sizeof(atleta_t));

    i=0;
    while(i<n && fscanf(fp, "%s %s %s %s %s %d", tmp.codice, tmp.nome, tmp.cognome, tmp.categoria, tmp.data, &tmp.ore)==6) {

        //sizeof(char)==1 nella maggior parte dei sistemi, lo ometto per semplificare

        atleti[i].nome=(char*)malloc(strlen(tmp.nome)+1);
        strcpy(atleti[i].nome, tmp.nome);

        atleti[i].cognome=(char*)malloc(strlen(tmp.cognome)+1);
        strcpy(atleti[i].cognome, tmp.cognome);

        atleti[i].cognomenome=(char*)malloc(strlen(tmp.nome)+strlen(tmp.cognome)+1);
        strcpy(atleti[i].cognomenome, strcat(tmp.cognome, tmp.nome));

        atleti[i].categoria=(char*)malloc(strlen(tmp.categoria)+1);
        strcpy(atleti[i].categoria, tmp.categoria);

        // inverto la data così posso usare strcmp
        reverseDate(tmp.data);

        // campi a dimesione fissa
        strcpy(atleti[i].data, tmp.data);
        strcpy(atleti[i].codice, tmp.codice);
        atleti[i].ore=tmp.ore;

        i++;
    }
    fclose(fp);

    // menu'
    for(non_strutturato) {
        system(F_CLEAR);
        i=-1;

        puts("1. Stampa contenuto anagrafica");
        puts("2. Ordina secondo data di nascita ascendente");
        puts("3. Ordina per codice atleta");
        puts("4. Ordina per cognome");
        puts("5. Mostra gli atleti per categoria");
        puts("6. Aggiornamento monte ore settimanali");
        puts("7. Ricerca atleta per codice");
        puts("8. Ricerca atleta per cognome (anche parziale)");
        puts("0. Esci");
        puts("");
        printf("> ");
        scanf("%d", &scelta);

        switch (scelta) {
        case 0:
            return 0;
        case 1:
            printf("Stampa su file? [s/n] ");
            scanf("%s", c);
            if (tolower(c[0])=='s') {
                printf("Inserisci il nome del file: ");
				scanf("%s", p);
				// riuso fp
				if ((fp=fopen(p, "w"))==NULL) {
					printf("Errore! Impossibile aprire il file \"%s\"", p);
				} else {
					fprintf(fp, "%d\n", n);
					stampaAnagrafica(atleti, n, fp);
					fclose(fp);
					printf("Scrittura sul file \"%s\" avvenuta con successo!\n", p);
					break;
				}
			}
            stampaAnagrafica(atleti, n, stdout);
            break;
        case 2:
            insetionSort(atleti, n, data);
            ordinato=data;
            puts("Ordinamento per data eseguito!");
            break;
        case 3:
            insetionSort(atleti, n, codice);
            ordinato=codice;
            puts("Ordinamento per codice atleta eseguito!");
            break;
        case 4:
            insetionSort(atleti, n, nome);
            ordinato=nome;
            puts("Ordinamento per cognome eseguito!");
            break;
        case 5:
            insetionSort(atleti, n, categoria);
            ordinato=categoria;
			// stampo le categorie una volta sola, considero un caso a parte
			// per il primo atleta per non incorrere in problemi con il for
            printf(" -> %s\n", atleti[0].categoria);
            printf("%s %s %s %s %d\n", atleti[0].codice, atleti[0].nome, atleti[0].cognome, atleti[0].data, atleti[0].ore);
            for (i=1; i<n; i++) {
				if (_comp(atleti[i].categoria,atleti[i-1].categoria)!=0) {
					printf("\n -> %s\n", atleti[i].categoria);
				}
				printf("%s %s %s %s %d\n", atleti[i].codice, atleti[i].nome, atleti[i].cognome, atleti[i].data, atleti[i].ore);
            }
            break;
        case 6:
			printf("Inserire il codice atleta per modificarne il monte ore setimanale: ");
			scanf("%s", c);
			if (ordinato==codice) {
				puts("Ricerca dicotomica...");
				i=ricercaDicotomica(atleti, n, c, codice);
			} else {
				puts("Ricerca lineare...");
				i=ricercaLineare(atleti, n, c, codice);
			}
			if (i!=-1) {
					printf("Trovato:\n%s -> %s %s %s %s\n", atleti[i].codice, atleti[i].nome, atleti[i].cognome, atleti[i].categoria, atleti[i].data);
					printf("Monte ore attuale: %d\nNuovo monte ore: ", atleti[i].ore);
					scanf("%d", &atleti[i].ore);
					puts("Monte ore modificato correttamente!");
				} else {
					printf("Atleta con codice \"%s\" non trovato.\n", p);
				}
			break;
		case 7:
			printf("Inserire il codice atleta: ");
			scanf("%s", c);
			if (ordinato==codice) {
				puts("Ricerca dicotomica...");
				i=ricercaDicotomica(atleti, n, c, codice);
			} else {
				puts("Ricerca lineare...");
				i=ricercaLineare(atleti, n, c, codice);
			}
			if (i!=-1) {
					printf("%s -> %s %s %s %s %d\n", atleti[i].codice, atleti[i].nome, atleti[i].cognome, atleti[i].categoria, atleti[i].data, atleti[i].ore);
				} else {
					printf("Atleta con codice \"%s\" non trovato.\n", p);
				}
			break;
		case 8:
			printf("Inserire il cognome dell'atleta: ");
			scanf("%s", p);
			if (ordinato==nome) {
				puts("Ricerca dicotomica...");
				i=ricercaDicotomica(atleti, n, p, nome);
				if (i!=-1) {
					printf("%s %s %s %s %s %d\n", atleti[i].codice, atleti[i].nome, atleti[i].cognome, atleti[i].categoria, atleti[i].data, atleti[i].ore);
				} else {
					printf("Atleta \"%s\" non trovato.\n", p);
				}
			} else {
				puts("Ricerca lineare...");
				i=ricercaLineare(atleti, n, p, nome);
				if (i!=-1) {
					printf("%s %s %s %s %s %d\n", atleti[i].codice, atleti[i].nome, atleti[i].cognome, atleti[i].categoria, atleti[i].data, atleti[i].ore);
				} else {
					printf("Atleta \"%s\" non trovato.\n", p);
				}
			}
			break;
            
        default:
            puts("Comando non trovato.\n");
        }
        getc(stdin); // prendo il ritorno a capo della scanf
        printf("\nPremere invio per tornare al menu'... ");
        getc(stdin); // aspetto che l'utente prema invio
    }

    return 0;
}

int sonoOrdinati(char *a, char *b) {
	int x=_comp(a,b);
    return x<0?1:(x==0?0:-1);
}

void stampaAnagrafica(atleta_t *atleti, int n, FILE* fp) {
	int i;
	for (i=0; i<n; i++) {
		fprintf(fp, "%s %s %s %s %s %d\n", atleti[i].codice, atleti[i].nome, atleti[i].cognome, atleti[i].categoria, atleti[i].data, atleti[i].ore);
	}
}

void reverseDate(char *s) {
    int d,m,y;
    sscanf(s, "%d/%d/%d",&d,&m,&y);
    if (d>1000) return; // esco se mi accorgo che è già salvata una data invertita
    sprintf(s, "%.4d/%.2d/%.2d",y,m,d);   
}

void insetionSort(atleta_t *atleti, int n, campo_e campo) {
    int i,j;
    atleta_t x;

	for (i=1; i<n; i++) {
        x = atleti[i];
        j=i-1;

        while (j>=0 && sonoOrdinati(getCampo(&x,0,campo), getCampo(atleti,j,campo))==1) {
            // controllo di che campo si sta parlando
            atleti[j+1] = atleti[j];
            j--;
        }
        atleti[j+1] = x;
    }
}

int startsWith(char *a, char *b) { 
	// case unsensitive
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

int ricercaDicotomica(atleta_t* atleti, int n, char* s, campo_e campo) {

    int l,r,m;
    l=0; r=n-1;
    
    if (campo==codice) {
		while ((r-l)!=0) {
			m=(l+r)/2;
			if (_comp(atleti[m].codice, s)==0) {
				return m;
			} else if (_comp(atleti[m].codice, s)>0){ // proseguo nel sottovettore sx
				r=m;
			} else { // proseguo nel sottovettore dx
				l=m+1;
			}
		}
		if (_comp(atleti[l].codice, s)==0) return l;
		return -1;
		
	} else if (campo==nome) {
		while ((r-l)!=0) {
			m=(l+r)/2;
			if (startsWith(atleti[m].cognomenome, s)==1) {
				return m;
			} else if (_comp(atleti[m].cognomenome, s)>0){ // proseguo nel sottovettore sx
				r=m;
			} else { // proseguo nel sottovettore dx
				l=m+1;
			}
		}
		if (startsWith(atleti[l].cognomenome, s)==1) return l;
		return -1;
	}

    return -1;
}

int ricercaLineare(atleta_t* atleti, int n, char* s, campo_e campo) {
	int i;
	if (campo==codice) {
		for (i=0; i<n; i++) {
			if (_comp(atleti[i].codice, s)==0) return i;
		}
	} else if (campo==nome) {
		for (i=0; i<n; i++) {
			if (startsWith(atleti[i].cognomenome, s)==1) return i;
		}
	}
	return -1;
}

char* getCampo(atleta_t *atleti, int i, campo_e campo) {
    switch (campo) {
        case nome:
            return atleti[i].cognomenome;
        case codice:
            return atleti[i].codice;
        case data:
            return atleti[i].data;
        case categoria:
            return atleti[i].categoria;
        default:
            return NULL;        
    }
}

void minuscola(char *s) {for (;*s;s++) *s=tolower(*s);}
void maiuscola(char *s) {for (;*s;s++) *s=toupper(*s);}
