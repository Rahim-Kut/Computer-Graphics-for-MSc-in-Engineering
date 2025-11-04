#include <stdlib.h>
#include <stdio.h>

#define N 10000000

void vector_add(float *out, float *a, float *b, int n) {
	for(int i = 0; i < n; i++){
		out[i] = a[i] + b[i];
	}
}

int main(){
	float *a, *b, *out; 

	// Allocate memory
	a   = (float*)malloc(sizeof(float) * N);
	b   = (float*)malloc(sizeof(float) * N);
	out = (float*)malloc(sizeof(float) * N);

	// Initialize array
	for(int i = 0; i < N; i++){
		a[i] = rand()%10; b[i] = rand()%10;
	}

	// Add the vectors
	vector_add(out, a, b, N);

	// Check the result
	printf( "result[0], %.0f + %.0f = %.0f\n", a[0],b[0],out[0] );
	printf( "result[N-1], %.0f + %.0f = %.0f\n", a[N-1],b[N-1],out[N-1] );
}

