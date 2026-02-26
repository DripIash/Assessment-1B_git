#include <iostream>
#include <assert.h>
using namespace std;

// swaps 2 integers using pointers
void SwapInts(int* int1, int* int2);

int main() {
	int a = 10, b = 3;
	int* A = &a;
	int* B = &b;
	SwapInts(A, B);

	assert(a == 3 && b == 10);

	cout << "\n\n======END======\n";
}

void SwapInts(int* int1, int* int2) {
	int temp = *int1;
	*int1 = *int2;
	*int2 = temp;
}