#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int randnum(int lim) {
	return 0;
}

void quadrado (int N) {
	srand(time(NULL));
	int i1;
	int i2;
	for (i1=0; i1<N; i1++) {
		for (i2=0; i2<N; i2++) {
			int random = 1 + rand()/((RAND_MAX + 1u)/9);
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