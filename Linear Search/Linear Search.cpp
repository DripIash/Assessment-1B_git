#include <iostream>
#include <assert.h>
using namespace std;

// searches through input array for a target int, returns the index it is first found at, otherwise returns -1
int LinearSearch(int target, int arr[], const int arrSize);

int main() {
	const int arrSize = 20;
	int array[arrSize] = { 67, 13, 3, 89, 43, 2, 19, 71, 5, 61, 97, 7, 37, 31, 17, 11, 83, 53, 23, 29 };

	assert(LinearSearch(3, array, arrSize) == 2);
	assert(LinearSearch(7, array, arrSize) == 11);
	assert(LinearSearch(67, array, arrSize) == 0);
	assert(LinearSearch(88, array, arrSize) == -1);

	int input;
	int output;
	cout << "Enter a value to find in array:" << endl;
	cin >> input;

	output = LinearSearch(input, array, arrSize);
	if (output == -1)
		cout << "Value is not in the array";
	else
		cout << "Value " << input << " found at index " << output << " in the array";

	cout << "\n\n======END======\n";
}

int LinearSearch(int target, int arr[], const int arrSize) {
	for (int i = 0; i < arrSize; i++)
		if (arr[i] == target)
			return i;
	return -1;
}