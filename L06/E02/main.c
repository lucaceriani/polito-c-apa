#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NOME_FILE "atleti.txt"
#define MAX_NOME 25
#define LUNG_CODICE 5
#define non_strutturato ;;
#define MAX_INPUT 100

#ifdef _WIN32
    #define F_CLEAR "cls"
    #define _comp stricmp
#else
    #define F_CLEAR "clear"
    #define _comp(a,b) strcasecmp(a,b)
    #define _comp strcasecmp
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

typedef enum {
    data, nome, codice, categoria
} campo_e;

void minuscola(char*);
void maiuscola(char*);
void stampaAnagrafica(atleta_t*, int, FILE*);
void reverseDate(char*);
void insetionSort(atleta_t*, int, campo_e, atleta_t**);
void stampaAtleta(atleta_t*, FILE*);
int sonoOrdinati(char*, char*);
int startsWith(char*, char*); // se a comincia con b;
// è più facile se le ricerche ritornano i puntatori all'atleta trovato
atleta_t *ricercaDicotomica(atleta_t**, int, char*, campo_e);
atleta_t *ricercaLineare(atleta_t*, int, char*, campo_e);
char *getCampo(atleta_t*,  campo_e);


int main() {

    FILE *fp;
    atleta_t *atleti, *ap, tmp;
    int scelta=-1;
    int n=0, i=0;
    int ordinato[]={-1, -1, -1, -1};
    atleta_t **ordData, **ordCodice, **ordNome, **ordCategoria;
    char uInput[MAX_INPUT];

    // apertura e controllo file
    if ((fp=fopen(NOME_FILE, "r"))==NULL){
        printf("Errore! Impossibile aprire il file \"%s\"!\n", NOME_FILE);
        exit(1);
    }

    // controllo della presenza e della correttezza della prima riga del file
    if (fscanf(fp, "%d\n", &n)!=1) exit(2);

    // alloco il vettore delle strutture
    atleti=(atleta_t*)malloc(n*sizeof(atleta_t));

    // alloco i vettori degli ordinamenti
    ordData=(atleta_t**)malloc(n*sizeof(atleta_t*));
    ordCodice=(atleta_t**)malloc(n*sizeof(atleta_t*));
    ordNome=(atleta_t**)malloc(n*sizeof(atleta_t*));
    ordCategoria=(atleta_t**)malloc(n*sizeof(atleta_t*));

    // alloco i campi della struttura temporanea tmp
    tmp.nome=(char*)malloc(MAX_NOME+1);
    tmp.cognome=(char*)malloc(MAX_NOME+1);
    tmp.categoria=(char*)malloc(MAX_NOME+1);

    i=0;
    while(i<n && fscanf(fp, "%s %s %s %s %s %d",
                        tmp.codice, tmp.cognome, tmp.nome, tmp.categoria,
                        tmp.data, &tmp.ore)==6) {

        // sizeof(char) vale 1 nella maggior parte dei sistemi
        // lo ometto per semplificare

        atleti[i].nome=strdup(tmp.nome);
        atleti[i].cognome=strdup(tmp.cognome);
        atleti[i].categoria=strdup(tmp.categoria);

        atleti[i].cognomenome=malloc(strlen(tmp.nome)+strlen(tmp.cognome)+1);
        strcpy(atleti[i].cognomenome, "");
        strcat(atleti[i].cognomenome, tmp.cognome);
        strcat(atleti[i].cognomenome, tmp.nome);

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
            scanf("%s", uInput);
            if (tolower(uInput[0])=='s') {
                printf("Inserisci il nome del file: ");
                scanf("%s", uInput);
                // riuso fp
                if ((fp=fopen(uInput, "w"))==NULL) {
                    printf("Errore! Impossibile aprire il file \"%s\"", uInput);
                } else {
                    fprintf(fp, "%d\n", n);
                    stampaAnagrafica(atleti, n, fp);
                    fclose(fp);
                    printf("Salvato il file \"%s\"!\n", uInput);
                    break;
                }
            }
            stampaAnagrafica(atleti, n, stdout);
            break;
        case 2:
            insetionSort(atleti, n, data, ordData);
            ordinato[data]=1;
            puts("Ordinamento per data eseguito!");
            break;
        case 3:
            insetionSort(atleti, n, codice, ordCodice);
            ordinato[codice]=1;
            puts("Ordinamento per codice atleta eseguito!");
            break;
        case 4:
            insetionSort(atleti, n, nome, ordNome);
            ordinato[nome]=1;
            puts("Ordinamento per cognome eseguito!");
            break;
        case 5:
            insetionSort(atleti, n, categoria, ordCategoria);
            ordinato[categoria]=1;
            // stampo le categorie una volta sola, considero un caso a parte
            // per il primo atleta per non incorrere in problemi con il for
            printf(" -> %s\n", ordCategoria[0]->categoria);
            stampaAtleta(ordCategoria[0], stdout);
            for (i=1; i<n; i++) {
                if (_comp(ordCategoria[i]->categoria, ordCategoria[i-1]->categoria)!=0) {
                    printf("\n -> %s\n", ordCategoria[i]->categoria);
                }
                stampaAtleta(ordCategoria[i], stdout);
            }
            break;
        case 6:
            printf("Codice atleta per modificarne il monte ore setimanale: ");
            scanf("%s", uInput);
            if (ordinato[codice]==1) {
                puts("Ricerca dicotomica...");
                ap=ricercaDicotomica(ordCodice, n, uInput, codice);
            } else {
                puts("Ricerca lineare...");
                ap=ricercaLineare(atleti, n, uInput, codice);
            }
            // controllo che cosa mi ha ritornato la ricerca
            if (ap!=NULL) {
                    puts("Trovato:");
                    stampaAtleta(ap, stdout);
                    printf("Monte ore attuale: %d\n", ap->ore);
                    printf("Nuovo monte ore:");
                    scanf("%d", &(ap->ore));
                    puts("Monte ore modificato correttamente!");
            } else {
                printf("Atleta con codice \"%s\" non trovato.\n", uInput);
            }

            break;
        case 7:
            printf("Inserire il codice atleta: ");
            scanf("%s", uInput);
            if (ordinato[codice]==1) {
                puts("Ricerca dicotomica...");
                ap=ricercaDicotomica(ordCodice, n, uInput, codice);
            } else {
                puts("Ricerca lineare...");
                ap=ricercaLineare(atleti, n, uInput, codice);
            }
            // controllo che cosa mi ha ritornato la ricerca
            if (ap!=NULL) {
                    stampaAtleta(ap, stdout);
            } else {
                printf("Atleta con codice \"%s\" non trovato.\n", uInput);
            }
            break;
        case 8:
            printf("Inserire il cognome dell'atleta: ");
            scanf("%s", uInput);
            if (ordinato[nome]==1) {
                puts("Ricerca dicotomica...");
                ap=ricercaDicotomica(ordNome, n, uInput, nome);
            } else {
                puts("Ricerca lineare...");
                ap=ricercaLineare(atleti, n, uInput, nome);
            }
            // controllo che cosa mi ha ritornato la ricerca
            if (ap!=NULL) {
                    stampaAtleta(ap, stdout);
            } else {
                printf("Atleta con cognome \"%s\" non trovato.\n", uInput);
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

void stampaAtleta(atleta_t *atleti, FILE *fp) {
    fprintf(fp, "%s %s %s %s %s %d\n",
            atleti->codice, atleti->nome, atleti->cognome,
            atleti->categoria, atleti->data, atleti->ore);
}

int sonoOrdinati(char *a, char *b) {
    int x=_comp(a,b);
    return x<0?1:(x==0?0:-1);
}

void stampaAnagrafica(atleta_t *atleti, int n, FILE* fp) {
    int i;
    for (i=0; i<n; i++) {
        stampaAtleta(&atleti[i], fp);
    }
}

void reverseDate(char *s) {
    int d,m,y;
    sscanf(s, "%d/%d/%d",&d,&m,&y);
    if (d>1000) return; // esco se mi accorgo che la data è invertita
    sprintf(s, "%.4d/%.2d/%.2d",y,m,d);
}

void insetionSort(atleta_t *atleti, int n, campo_e campo, atleta_t **ordinato) {
    int i,j;

    ordinato[0]=&atleti[0];

    for (i=1; i<n; i++) {
        j=i-1;
        while (j>=0 &&
               sonoOrdinati(getCampo(&atleti[i],campo),
                            getCampo(ordinato[j],campo))==1
               ) {
            ordinato[j+1] = ordinato[j];
            j--;
        }
        ordinato[j+1] = &atleti[i];
        //printf("ordinato.data: %s\n", getCampo(&atleti[i],campo));
    }
}

int startsWith(char *a, char *b) {
    // ritorna 0 se a comincia con b

    // case unsensitive
    int i, n=0;
    // voglio proseguire il confronto fino all'ultima lettera
    // di b (che dorevbbe essere la più corta)

    n=strlen(b);
    if (strlen(a)<n) return -1;

    for (i=0; i<n; i++) {
        if (tolower(a[i])!=tolower(b[i])) {
            return -1;
        }
    }
    return 0;
}

atleta_t *ricercaDicotomica(atleta_t **ordinato, int n, char *s, campo_e campo){
    int l,r,m;
    l=0; r=n-1;

    int (*fpComp)(char*, char*); // vedi ricerca lineare
    fpComp=(campo==codice?&_comp:&startsWith);

    while ((r-l)!=0) {
        m=(l+r)/2;
        // se ho azzeccato il valore di mezzo lo ritorno subito
        if (fpComp(getCampo(ordinato[m],campo), s)==0) return ordinato[m];
        // proseguo a sinistra
        else if (_comp(getCampo(ordinato[m],campo), s)>0) r=m;
        // proseguo a destra
        else l=m+1;
    }

    if (fpComp(getCampo(ordinato[l],campo), s)==0) return ordinato[l];
    return NULL;
}

atleta_t *ricercaLineare(atleta_t *atleti, int n, char *s, campo_e campo) {

    int i;
    // fp_Comp è un puntatore a funzione che a seconda che si tratti del
    // campo codice diventa _comp(), altrimenti per il cognome uso startsWith()
    int (*fpComp)(char*, char*);

    fpComp=(campo==codice?&_comp:&startsWith);

    for (i=0; i<n; i++)
        if (fpComp(getCampo(&atleti[i], campo), s)==0) return &atleti[i];

    return NULL;
}

char* getCampo(atleta_t *atleti, campo_e campo) {
    switch (campo) {
        case nome:
            return atleti->cognomenome;
        case codice:
            return atleti->codice;
        case data:
            return atleti->data;
        case categoria:
            return atleti->categoria;
        default:
            return NULL;
    }
}

void minuscola(char *s) {for (;*s;s++) *s=tolower(*s);}
void maiuscola(char *s) {for (;*s;s++) *s=toupper(*s);}
