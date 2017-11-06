#include <stdio.h>
#include <stdlib.h>

#define N 10000

int values[N]={0};

int H(int);

int main() {
    int n=0, i;
    int *res;
    puts("Sequenza di Hofstadter fino all'elemento n.");
    printf("n = ");
    scanf("%d", &n);
    n=abs(n);
    for (i=0; i<n; i++) {
        values[i]=H(i);
        printf("%d, ", values[i]);

    }
    return 0;
}

int H(int n) {
    if (values[n]!=0) {
        return values[n];
    }
    if (n<=0) {
        return 0;
    }
    return n-H(H(H(n-1)));
}
