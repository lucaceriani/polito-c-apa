#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define clear_funciton "cls" // "clear" sui sistemi unix

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
void selezionaDati(volo*, int, comando, char*, char*);

//funzioni aggiuntive
void maiuscola(char*);
int checkData(char*);

int main() {

	volo voli[max_n_righe];
	comando c;
	const int n = (max_comando+1)+2*(len_data+1); // lunghezza massima di una riga con il comando
	char data1[n], data2[n];
	int dim_voli;

	dim_voli=leggiTabella(nome_f_voli, voli);

	for(;;){
		c=leggiComando(data1, data2, n);
        if (c==-1) continue;
        selezionaDati(voli, dim_voli, c, data1, data2);
	}

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

comando leggiComando(char *data1, char *data2, int n) {

    char c[max_comando];
    char data[n];
    int arg_c=0;
    int i;

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

	printf("> ");
	fgets(data,n,stdin);

	// initializzazione data1, data2
    for (i=0; i<n; i++){
		data1[i]='\0';
		data2[i]='\0';
    }

	arg_c=sscanf(data,"%s%s%s",c,data1,data2);
	if (arg_c<1 && arg_c>3) {
		puts("Comando errato, riprovare!");
		getc(stdin);
		return -1;
	}

	maiuscola(c);

	// passaggio da string a enum
	if 		(strcmp(c,"VOLI")==0) 			return r_date;
	else if (strcmp(c, "ORIGINE")==0) 		return r_origine;
	else if (strcmp(c, "DESTINAZIONE")==0)	return r_destinazione;
	else if (strcmp(c, "VETTORE")==0)		return r_vettore;
	else if (strcmp(c, "FINE")==0)			return r_fine;
	else 	return -1;
}


void selezionaDati(volo *voli, int n, comando c, char *data1, char *data2) {
	int i;
    switch (c) {
	case r_date:
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
		maiuscola(data1);
		for (i=0; i<n; i++) {
			if (strcmp(data1, voli[i].partenza)==0) puts(voli[i].codiceVolo);
		}
		break;
	case r_destinazione:
		maiuscola(data1);
		for (i=0; i<n; i++) {
			if (strcmp(data1, voli[i].destinazione)==0) puts(voli[i].codiceVolo);
		}
		break;
	case r_vettore:
		maiuscola(data1);
		for (i=0; i<n; i++) {
			if (strcmp(data1, voli[i].vettore)==0) puts(voli[i].codiceVolo);
		}
		break;
	case r_fine:
		exit(0);
	default: exit(2);
    }
    puts("\nPremi invio per tornare al menu'");
    getc(stdin);
}

void maiuscola(char *s) {
	for (;*s;++s) *s=toupper(*s);
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
