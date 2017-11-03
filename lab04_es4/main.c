#include <stdio.h>
#include <stdlib.h>

int somma(int);

int main() {

    int x;
    puts("Somma ricorsiva delle cifre di un numero intero");
    printf("> ");
    scanf("%d", &x);
    x=abs(x);
    printf("La somma dell cifre di %d e' %d\n", x, somma(x));

    return 0;
}

int somma(int x) {
    if (x<10) {
        return x;
    }
    return (x%10)+somma(x/10);
}
