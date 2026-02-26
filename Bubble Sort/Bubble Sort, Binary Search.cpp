#include <iostream>
#include <assert.h>
using namespace std;

// sorts an array from smallest to largest
void BubbleSort(int arr[], const int arrSize);
// returns true if bubble sort worked properly
bool PairsAreCorrect(int arr[], const int arrSize);
// returns the index of chosen element, returns -1 if not found
int BinarySearch(int arr[], const int arrSize, int target);

int main() {
	const int arrSize = 20;
	int array[arrSize] = { 67,13,3,89,43,2,19,71,5,61,97,7,37,31,17,11,83,53,23,29 };

	BubbleSort(array, arrSize);
	assert(PairsAreCorrect(array, arrSize));

	assert(BinarySearch(array, arrSize, 11) == 4);
	assert(BinarySearch(array, arrSize, 23) == 8);
	assert(BinarySearch(array, arrSize, 97) == 19);
	assert(BinarySearch(array, arrSize, 88) == -1);

	int input;
	int output;
	cout << "Enter a value to find in array:" << endl;
	cin >> input;

	output = BinarySearch(array, arrSize, input);
	if (output == -1)
		cout << "Value is not in the array";
	else
		cout << "Value " << input << " found at index " << output << " in the array";

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

int BinarySearch(int arr[], const int arrSize, int target) {
	int high = arrSize - 1, low = 0, mid;
	bool stop = false;
	while (true) {
		// set the midpoint
		mid = low + (high - low) / 2;
		if (target == arr[mid])
			return mid;
		// if the value is higher than the midpoint, set lower bound to one above the midpoint
		else if (target > arr[mid])
			low = mid + 1;
		// if the value is lower than the midpoint, set upper bound to one below the midpoint
		else if (target < arr[mid])
			high = mid - 1;
		
		if (stop)
			return -1;
		// after the break so it doesn't break early
		if (high == low || high < low)
			stop = true;
	}
}