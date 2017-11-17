#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define f_sost "sost.txt"
#define f_dest "dest.txt"
#define f_src "src.txt"

#define N 200

int main() {

	typedef struct {
		int len;
		char parola[N+1];
    } t_sost;

    FILE *fSrc, *fSost, *fDest;
    char s[N+1]; //parola che sto leggendo
    int n_sost=0; // numero di parole sostituibili lette nella prima riga del f_sost
    int i=0;
    t_sost sost[30];
    char special_char;
    int lun=0;

    // apertura file in letture
	fSrc=fopen(f_src, "r");
	fSost=fopen(f_sost, "r");

	// controllo sulla corretta apertura dei file
	if (fSrc==NULL) printf("Errore! Impossibile aprire il file '%s'\n", f_src), exit(1);
	if (fSost==NULL) printf("Errore! Impossibile aprire il file '%s'\n", f_sost), exit(1);

	fscanf(fSost, "%d", &n_sost);

	//leggo tutto il file a prescindere da quanto vale n_sost
    while (fscanf(fSost, "%d %s", &sost[i].len, sost[i].parola)==2 && i<30) i++;

    //controllo la correttezza dei dati
    if (i!=n_sost) printf("Errore! Il file '%s' non e' coerente!\n", f_sost), exit(2);


    fDest=fopen(f_dest, "w");
    if (fSrc==NULL) printf("Errore! Impossibile aprire il file '%s'\n", f_dest), exit(1);

    while (fscanf(fSrc, "%s", s)==1) {

		lun=strlen(s);

		//	imposto special_char a \0 come "flag zero"
		special_char='\0';

		/*
			controllo se il penultimo carattere della stringa
			letta è un carattere alfanumerico, se non lo è vuol dire
			che è un segno di punteggiature che viene fatto sparire
			per non interferire con il conteggio dei caratteri, viene
			poi riaggiunto prima di scrivere su fDest
		*/
        if(!isalpha(s[lun-1])){
			special_char=s[lun-1];
			s[lun-1]='\0';
        }

		for(i=0; i<n_sost; i++) {

			/*
			 richiamo la funzione strlen(s) perche' s potrebbe
			 aver cambiato lunghezza dopo il controllo qui sopra
			*/
            if (strlen(s)==sost[i].len) {
					strcpy(s, sost[i].parola);
					break;
            }
		}

		/*
			se avevo trovato un carattere alfanumerico
			diverso da \0 allora lo rimetto al suo posto
		*/
		if (special_char!='\0') {
			lun=strlen(s);
            s[lun]=special_char;
			s[lun+1]='\0';
		}

		fprintf(fDest, "%s ", s);
    }

    fclose(fSrc);
    fclose(fSost);
    fclose(fDest);

    return 0;
}
