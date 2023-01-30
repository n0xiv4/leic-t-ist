#include <stdio.h>
#define SPACE 2

void escreveCaracteres(char c, int n) {
	while(n--) {
		putchar(c);
	}
}

int main() {
	char c;
	int i;
	printf("Escreve o caracter: ");
	scanf("%c", &c);
	printf("Escreve o numero: ");
	scanf("%i", &i);
	escreveCaracteres(c, i);
	printf("%*d", SPACE, i++);
}