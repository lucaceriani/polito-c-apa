#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "PQ.h"
#include "Item.h"

#define LINE(i) for (i=80; i-->0; printf("-")); printf("\n")
#define MAX_NOME 15

#ifdef _WIN32
    #define F_CLEAR "cls"
#else
    #define F_CLEAR "clear"
#endif

void printMenu();
void sfida(PQ);

int main() {
        
    PQ pq=PQnew();
    FILE *fp;
    int scelta=-1, i;
    char nome[MAX_NOME+1];
    char f[100]; // percorso del file da salvare/aprire
    int punti;
    
    // random seed
    srand((unsigned int)time(NULL));
    
    // menu'
    for (;;) {
        system(F_CLEAR); // cancello lo schermo
        printMenu();
        
        scanf("%d", &scelta);
        
        switch(scelta) {
        case 0: // esci
            exit(0);
            break;
        case 1: // sfida
            sfida(pq);
            break;
        case 2: // visualizza classifica
            LINE(i);
            PQdisplay(stdout, pq, "o") ;
            break;
        case 3: // nuovo partecipante
            puts("Inserisci nuovo partecipante...");
            printf("Nome:  "); scanf("%s", nome);
            //printf("Punti: "); scanf("%d", &punti);
            PQinsert(pq, ITEMnew(nome, 10));
            printf("Partecipante inserito con successo!");
            break;
        case 4: // elimina partecipante
            LINE(i);
            PQdisplay(stdout, pq, "o") ;
            LINE(i);
            printf("Numero del partecipante da eliminare: ");
            scanf("%d", &i);
            PQdelete(pq, i-1);
            puts("Partecipante eliminato con successo!");
            break;
        case 5: // carica dati da file
            printf("Percorso del file da dove caricare i dati: ");
            scanf("%s", f);
            if ((fp=fopen(f, "r"))==NULL) {
                printf("Impossibile aprire il file \"%s\"!\n", f);
                break;
            }
            while(fscanf(fp, "%s %d", nome, &punti)==2)
                PQinsert(pq, ITEMnew(nome,punti));
            puts("Dati caricati con successo!");
            fclose(fp);
            break;
        case 6: // salva dati su file
            printf("Percorso del file dove salvare i dati: ");
            scanf("%s", f);
            if ((fp=fopen(f, "w"))==NULL) {
                printf("Impossibile aprire il file \"%s\"!\n", f);
                break;
            }
            PQdisplay(fp, pq, "f");
            puts("File salvato con successo!");
            fclose(fp);
            break;
        default:
            puts("Comando non valido, riprovare.");
        }
        
        getc(stdin);
        getc(stdin);
    }
    
    
    return 0;
}

void sfida(PQ pq) {
    
    if (!PQisBattlePossible(pq)) { // controllo di avere effettivamente qualcosa
        puts("Errore! Controllare di avere almeno due partecipanti!");
        return;
    }
    
    Item i1=PQextractHead(pq);
    Item i2=PQextractHead(pq);
    char *n1, *n2;
    int p1, p2;
        
    n1=ITEMgetName(i1); p1=ITEMgetPoints(i1);
    n2=ITEMgetName(i2); p2=ITEMgetPoints(i2);
    
    printf("%s (%d pt) VS %s (%d pt)\n", n1, p1, n2, p2);
    
    if (rand()%2) { // se vince il primo
        p1+=(int)(.25*p2)+1;
        p2-=(int)(.25*p2)+1;
        printf("Vince %s (%d pt)\nPerde %s (%d pt)\n", n1, p1, n2, p2);
    } else { // vince il secondo
        p2+=(int)(.25*p1)+1;
        p1-=(int)(.25*p1)+1;
        printf("Vince %s (%d pt)\nPerde %s (%d pt)\n", n2, p2, n1, p1);
    }
    
    if (p1<=0) {
        //PQdelete(pq, 0); // cancella lo sfidante 1
    } else {
        PQinsert(pq, ITEMnew(n1, p1)); // altimenti lo modifica
    }
    
    if (p2<=0) {
        //PQdelete(pq, II);
    } else {
        PQinsert(pq, ITEMnew(n2, p2));
    }
    
    ITEMfree(i1); ITEMfree(i2); // cancello i vecchi item
    
    // ho dovuto fare la PQextractHead e PQinsert perché la PQchange mi
    // avrebbe modificato gli indici.
}

void printMenu() {
    int i=0;
    LINE(i);
    puts("1. Sfida!");
    puts("2. Visualizza classifica");
    puts("");
    puts("3. Inserisci nuovo partecipante");
    puts("4. Elimina partecipante");
    puts("");
    puts("5. Carica dati da file");
    puts("6. Salva dati su file");
    puts("");
    puts("0. Esci");
    LINE(i);
    puts("");
    printf("> ");
}
