#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define F_CLEAR "clear" // "clear" per sistemi unix

#define NOME_FILE "atleti.txt"
#define MAX_NOME 25
#define LUNG_CODICE 5
#define non_strutturato ;;

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

typedef enum {
    lineare=1, dicotomica
} ricerca_e;

void minuscola(char*);
void stampaAnagrafica(atleta_t*, int, FILE*);
void reverseDate(char*);
void insetionSort(atleta_t*, int, campo_e);
int sonoOrdinati(char*, char*);

int main() {

    FILE *fp;
    atleta_t *atleti;
    atleta_temp_t tmp;
    int scelta=-1;
    int n=0, i=0;
    int ordinato[4];

    // inizializzazione del vettore a non ordinato
    for (i=0; i<4; i++) {
        ordinato[i]=0;
    }

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

        puts("1. Stampa contenuto anagrafica");
        puts("2. Ordina secondo data di nascita ascendente");
        puts("3. Ordina per codice atleta");
        puts("4. Ordina per cognome");
        puts("5. Mostra gli atleti per categoria");
        puts("6. Aggiornamento monte ore settimanali");
        puts("7. Ricerca atleta per codice");
        puts("8. Ricerca atleta per cognome (anche parziale)");
        puts("");
        printf("> ");
        scanf("%d", &scelta);

        switch (scelta) {
        case 0:
            return 0;
        case 1:
            printf("Stampa su file? [s/N] \n");
            stampaAnagrafica(atleti, n, stdout);
            break;
        case 2:
            insetionSort(atleti, n, data);
            ordinato[data]=1;
            break;
        case 3:
            insetionSort(atleti, n, codice);
            ordinato[codice]=1;
            break;
        case 4:
            insetionSort(atleti, n, nome);
            ordinato[nome]=1;
            break;
        case 5:
            insetionSort(atleti, n, categoria);
            ordinato[categoria]=1;
            for (i=0; i<n; i++) {
                puts(atleti[i].categoria);

            }
            break;
        default:
            puts("Comando non trovato");
        }
        getc(stdin); // prendo il ritorno a capo della scanf
        printf("\nPremere invio per tornare al menu'... ");
        getc(stdin); // aspetto che l'utente prema invio
    }

    return 0;
}

int sonoOrdinati(char *a, char *b) {
	minuscola(a);
	minuscola(b);
    return strcmp(a,b)<0?1:0;
}

void stampaAnagrafica(atleta_t *atleti, int n, FILE* fp) {
    int i;
    for (i=0; i<n; i++) {
        fprintf(fp, "%s %s %s %s %s %d\n", atleti[i].codice, atleti[i].cognomenome, atleti[i].cognome, atleti[i].categoria, atleti[i].data, atleti[i].ore);
    }
}

void reverseDate(char *s) {
    char d[11];
    d[10]='\0';
    d[4]='/';
    d[7]='/';
    sscanf(s, "%c%c/%c%c/%c%c%c%c", &d[8],&d[9], &d[5],&d[6], &d[0],&d[1],&d[2],&d[3]);
    strcpy(s,d);
}

void insetionSort(atleta_t *atleti, int n, campo_e campo) {
    int i,j;
    atleta_t x;
    char *a, *b;

    for (i=1; i<n; i++) {
        x = atleti[i];
        j=i-1;

        switch (campo) {
            case data:
                a=x.data; b=atleti[j].data; break;
            case nome:
                a=x.cognomenome; b=atleti[j].cognomenome; break;
            case codice:
                a=x.codice; b=atleti[j].codice; break;
            case categoria:
                a=x.categoria; b=atleti[j].categoria; break;
            default:
                return;
            }

        while (j>=0 && sonoOrdinati(a, b)==1) {
            // controllo di che campo si sta parlando
            switch (campo) {
            case data:
                b=atleti[j].data; break;
            case nome:
                b=atleti[j].cognomenome; break;
            case codice:
                b=atleti[j].codice; break;
            case categoria:
                b=atleti[j].categoria; break;
            default:
                return;
            }
            atleti[j+1] = atleti[j];
            j--;
        }
        atleti[j+1] = x;
    }
}

void minuscola(char *s) {
    for (;*s;s++) *s=tolower(*s);
}
