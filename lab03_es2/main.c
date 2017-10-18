#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define clear_funciton "cls"

#define nome_f_voli "voli.txt"
#define max_riga 30
#define max_n_righe 1000

#define len_codice_volo 5
#define len_partenza 3
#define len_destinazione 3
#define len_data 19
#define len_vettore 4

#define max_comando 13

// definizioni tipi di dato
typedef enum {
	r_date, r_origine, r_destinazione, r_vettore, r_fine
} comando;

typedef struct {
    char codiceVolo[len_codice_volo+1];
    char partenza[len_partenza+1];
    char destinazione[len_destinazione+1];
    char dataOra[len_data+1];
    char vettore[len_vettore+1];
} volo;

// definizioni prototipi funzioni
int leggiTabella(char*, volo*);
comando leggiComando();
void selezionaDati(volo*, int, comando);

//funzioni aggiuntive
void maiuscola(char*);
void input(char*,int);
int checkData(char*);

int main() {

	volo voli[max_n_righe];
	int dim_voli;

	dim_voli=leggiTabella(nome_f_voli, voli);

	for(;;)
		selezionaDati(voli, dim_voli, leggiComando());

    return 0;
}

int leggiTabella(char *nome_file, volo *voli) {

	FILE *fp;
	int i=0, dim_voli=0;

	fp=fopen(nome_file, "r");
	if (fp==NULL) printf("Errore! Impossibile aprire il file \"%s\"\n", nome_file), exit(1);

	fscanf(fp, "%d", &dim_voli);

	// controllo sulla correttezza del primo dato del file (dim_voli)
    while(fscanf(fp,"%s%s%s%s%s",voli[i].codiceVolo,voli[i].partenza,voli[i].destinazione,
				voli[i].dataOra,voli[i].vettore)!=EOF && i<dim_voli && i<max_n_righe) i++;

	fclose(fp);
    dim_voli=i;
    return dim_voli;
}

comando leggiComando() {

    char c[max_comando];

	system(clear_funciton);
    puts("+----------------------------------------------------------------------+");
    puts("|                          COMANDI DISPONIBILI                         |");
    puts("+-------------+----------------------+---------------------------------+");
    puts("|   COMANDO   |      PARAMETRI       |           DESCRIZIONE           |");
    puts("+-------------+----------------------+---------------------------------+");
    puts("|voli         | <data_1> <data_2>    | voli compresi tra le due date   |");
    puts("|origine      | <codice_aereoporto>  | voli che decollano da           |");
    puts("|destinazione | <codice_aereoporto>  | voli che atterrano a            |");
    puts("|vettore      | <codice_vettore>     | voli effettuati dal vettore     |");
    puts("|fine         |                      | esci dal programma              |");
    puts("+-------------+----------------------+---------------------------------+");
	puts("");

	input(c, max_comando);
	maiuscola(c);

	// passaggio da string a enum
	if (strcmp(c,"VOLI")==0) return r_date;
	else if (strcmp(c, "ORIGINE")==0) return r_origine;
	else if (strcmp(c, "DESTINAZIONE")==0) return r_destinazione;
	else if (strcmp(c, "VETTORE")==0) return r_vettore;
	else if (strcmp(c, "FINE")==0) return r_fine;
	else leggiComando();
}

void selezionaDati(volo *voli, int n, comando c) {
	char data1[len_data]; // len_data è il massimo che posso acquisire
	char data2[len_data];
	int i;
    switch (c) {
	case r_date:
		puts("Inserire le date nel formato YYYY/MM/DD");
		input(data1, len_data);
		input(data2, len_data);
        if ((checkData(data1)+checkData(data2))!=2) {
            puts("Data inserita nel formato sbagliato, riprovare");
            break;
        }
        for (i=0; i<n; i++) {
			if (strcmp(data1, voli[i].dataOra)==-1 && strcmp(data2, voli[i].dataOra)==1) {
				puts(voli[i].codiceVolo);
			}
        }
		break;
	case r_origine:
		puts("Inserire il codice dell'aereoporto d'origine nel formato XYZ");
		input(data1, len_partenza);
		maiuscola(data1);
		for (i=0; i<n; i++) {
			if (strcmp(data1, voli[i].partenza)==0) puts(voli[i].codiceVolo);
		}
		break;
	case r_destinazione:
		puts("Inserire il codice dell'aereoporto d'arrivo nel formato XYZ");
		input(data1, len_partenza);
		maiuscola(data1);
		for (i=0; i<n; i++) {
			if (strcmp(data1, voli[i].destinazione)==0) puts(voli[i].codiceVolo);
		}
		break;
	case r_vettore:
		puts("Inserire il codice del vettore nel formato AXYZ");
		input(data1, len_vettore);
		maiuscola(data1);
		for (i=0; i<n; i++) {
			if (strcmp(data1, voli[i].vettore)==0) puts(voli[i].codiceVolo);
		}
		break;
	case r_fine:
		exit(0);
	default: exit(2);
    }
    puts("\nPremi un tasto per tornare al menu'");
    getc(stdin);
    getc(stdin);
}

void maiuscola(char *s) {
	for (;*s;++s) *s=toupper(*s);
}

// funzione che fa l'input di una stringa su data di lunghezza al max l
void input(char *data,int l) {
	printf("> ");
	fgets(data, l+1, stdin);
	sscanf(data, "%s", data);
}

int checkData(char *data) {
	int i;
    for (i=0; i<4; i++) {
		if (!isdigit(data[i]))
			 return 0;
    }
    for (i=5; i<6; i++) {
		if (!isdigit(data[i]))
			return 0;
    }
    for (i=8; i<9; i++) {
		if (!isdigit(data[i]))
			return 0;
    }
    return 1;
}
