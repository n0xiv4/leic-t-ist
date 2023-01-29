#include <stdio.h>

int multiplos(int n, int k) {
	int i;
	for (i=1; i<=k; i++) {
		int m = n*i;
		printf("Multiplo %i: %d\n", i, m);
	}
}

int main() {
	int n, k;
	printf("Escreve o teu n: ");
	scanf("%d", &n);
	printf("Escreve o teu k: ");
	scanf("%d", &k);
	multiplos(n, k);
	return 0;
}
