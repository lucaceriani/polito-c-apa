#include <stdio.h>
#include <stdlib.h>

int somma(int);

int main() {

    int x;
    puts("Somma ricorsiva delle cifre di un numero intero");
    printf("> ");
    scanf("%d", &x);
    x=abs(x);
    printf("La somma delle cifre di %d e' %d\n", x, somma(x));

    return 0;
}

int somma(int x) {
    if (x<10) {
        return x;
    }
    return (x%10)+somma(x/10);
}
/* esemplificazione soluzione:

    x=123;

    somma(123)
     |
     +-> 3 + somma(12)
              |
              +-> 2 + somma(1)
                       |
                       +-> 1    = 3 + 2 + 1 = 6
*/
