
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operation.h"

int main() {
    char input[100];
    int num1, num2;
    char op;

    while (1) {
        printf("Entrez le calcul (format: num1 op num2, ou 'exit' pour quitter): ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        if (strncmp(input, "exit", 4) == 0) {
            break;
        }

        if (sscanf(input, "%d %c %d", &num1, &op, &num2) != 3) {
            printf("Format invalide\n");
            continue;
        }

        if (op == '+') {
            printf("%d + %d = %d\n", num1, num2, add(num1, num2));
        } else if (op == '-') {
            printf("%d - %d = %d\n", num1, num2, subtract(num1, num2));
        } else if (op == '*') {
            printf("%d * %d = %d\n", num1, num2, multiply(num1, num2));
        } else {
            printf("Operation invalide\n");
        }
    }

    return 0;
}
