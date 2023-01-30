#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void quadrado (int N) {
	int i1;
	int i2;
	srand(time(NULL));
	for (i1=0; i1<N; i1++) {
		for (i2=0; i2<N; i2++) {
			int random = rand() % 100;
			printf("%i ", random);
		}
		printf("\n");
	}
}

int main() {
	printf("What size should the square be? ");
	int sizeSqr;
	scanf("%i", &sizeSqr);
	quadrado(sizeSqr);
	return 0;
}