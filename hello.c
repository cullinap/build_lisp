#include <stdio.h>

char hello_world(int iter) {
    int x = 0;

    while (x < iter) {
        printf("%d while hello!\n", x);
        x++;
    }

    printf("\n");

    do {
        printf("%d do hello\n", x);
        x--;
    } while (x > 0);

    return 0;
}

int main(int argc, char** argv) {

    hello_world(6);

    return 0;
}
