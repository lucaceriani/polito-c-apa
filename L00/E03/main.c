#include <stdio.h>
#include <stdlib.h>

#define gradoMax 10

float valutaHorner(float*, float);
float ass(float);

int main() {

    int grado=0;
    int i=0;
    float dimInt=0;
    float c[gradoMax+1]={0};
    float a,b;
    float integrale=0;
    int M;

    printf("Grado del polinomio? ");
    scanf("%d", &grado);
    (grado<=10 && grado>0)?1:(exit(-1));

	for (i=0; i<grado+1; i++) {
		printf("Coefficiente termine di grado %d:", i);
		scanf("%f",&c[i]);
	}

	printf("Estremo a: ");
	scanf("%f",&a);

	printf("Estremo b: ");
	scanf("%f",&b);

	printf("Sottointervalli: ");
	scanf("%d",&M);

	dimInt=ass(a-b)/M;

	for (i=0;i<M;i++){
		integrale+=valutaHorner(c,(a+dimInt/2+i*dimInt))*dimInt;
	}

	printf("L'integrale vale: %.4f",integrale);

	return 0;

}

float valutaHorner(float c[gradoMax+1], float x) {
    float res=0;
    int i;

    // soluzione poco elegante, da rivedere
    for (i=gradoMax; i>=0; i--) {
        res+=c[i];
        i!=0?res=res*x:0;
    }
    return res;
}

float ass(float x) {return x<0?-x:x;}
