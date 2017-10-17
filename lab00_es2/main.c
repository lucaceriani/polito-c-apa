#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define N 20+1
#define M 10+1

int numeroConsonanti(char*);
int wordLen(char*);

int main() {

	FILE *fp1, *fp2, *fp3;

	char p[N];
	char s[N];

	int numeroNomi=0;
	int i;

	char file1[M];
	char file2[M];
	char file3[M];

	printf("Nome file di input ");
    gets(file1);
    printf("Nome primo file di output ");
    gets(file2);
    printf("Nome secondo file di output ");
    gets(file3);


	fp1=fopen(file1, "r");
	fp2=fopen(file2, "w");
	fp3=fopen(file3, "w");

	if (fp1==NULL) {
		printf("Errore nell'apertura di un file!");
		return -1;
	}

	fscanf(fp1,"%d %s", &numeroNomi, p);

    for (i=0; i<numeroNomi; i++) {
		fscanf(fp1,"%s", s);
        if (strcmp(p,s)==-1) {
            fprintf(fp2,"%s\n", s);
        }
        if (numeroConsonanti(s)>numeroConsonanti(p)) {
			fprintf(fp3, "%s\n" ,s);
        }
    }

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);

    return 0;
}

int numeroConsonanti(char* s) {

	int i;
	int consonanti=0;
	int len=wordLen(s);

	for (i=0; i<len; i++) {
        switch (tolower(s[i])) {
			case 'a':
			case 'e':
			case 'i':
			case 'o':
			case 'u':
				break;
			default:
				consonanti++;
				break;
        }
	}

}

int wordLen(char *s) {
	int i=0;
    while (s[i]!='\0') i++;
	return i;
}
