#include <iostream>
#include <assert.h>
using namespace std;

// sorts an array from smallest to largest
void BubbleSort(int arr[], const int arrSize);
// returns true if bubble sort worked properly
bool PairsAreCorrect(int arr[], const int arrSize);

int main() {
	const int arrSize = 20;
	int array[arrSize] = { 67,13,3,89,43,2,19,71,5,61,97,7,37,31,17,11,83,53,23,29 };

	BubbleSort(array, arrSize);
	assert(PairsAreCorrect(array, arrSize));

	cout << "\n\n======END======\n";
}

void BubbleSort(int arr[], const int arrSize) {
	bool swapped = false;
	for (int i = 0; i < (arrSize - 1); i++) {
		for (int j = 0; j < (arrSize - (i + 1)); j++)
			if (arr[j] > arr[j + 1]) {
				// swap without an extra variable
				arr[j] -= arr[j + 1];
				arr[j + 1] += arr[j];
				arr[j] = arr[j + 1] - arr[j];
				swapped = true;
			}
		// break if all the elements are in the correct order
		if (!swapped)
			break;
		swapped = false;
	}
}

bool PairsAreCorrect(int arr[], const int arrSize) {
	for (int i = 0; i < (arrSize - 1); i++)
		if (arr[i] > arr[i + 1])
			return false;
	return true;
}