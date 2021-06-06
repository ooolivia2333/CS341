#include <string>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

//void print(int *print, int size) {
//	for (int i = 0; i < size; i++) {
//		cout << print[i] << " ";
//	}
//	cout << endl;
//}

long long* mult(long long first[], long long second[], int firstDegree, int secondDegree) {
	long long* result = new long long[firstDegree + secondDegree + 1]();

	for (int i = 0; i < firstDegree + 1; i++) {
		for (int j = 0; j < secondDegree + 1; j++) {
			result[i + j] += first[i] * second[j];
		}
	}

	return result;
}

long long* polynomial(long long* first, long long* second, int firstDegree, int secondDegree) {
	if (firstDegree <= 1 or secondDegree <= 1) {
		return mult(first, second, firstDegree, secondDegree);
	}

	int midDegree = firstDegree / 2;
	int leftDegree = midDegree;
	int rightDegree = firstDegree - midDegree - 1;

	long long* a0 = new long long[leftDegree + 1];
	long long* a1 = new long long[rightDegree + 1];
	long long* b0 = new long long[leftDegree + 1];
	long long* b1 = new long long[rightDegree + 1];

	int resultDegree = firstDegree + secondDegree;
	long long* result = new long long[resultDegree + 1]();

	for (int i = 0; i < leftDegree + 1; i++) {
		a0[i] = first[i];
		b0[i] = second[i];
	}
	for (int i = 0; i < rightDegree + 1; i++) {
		a1[i] = first[i + leftDegree + 1];
		b1[i] = second[i + leftDegree + 1];
	}

	long long* p1 = polynomial(a0, b0, leftDegree, leftDegree);
	int p1Degree = 2 * leftDegree;
	for (int i = 0; i <= p1Degree; i++) {
		result[i] += p1[i];
	}

	//cout << endl;
	//print(b1, rightSize);
	//cout << rightDegree << endl;
	long long* p3 = polynomial(a1, b1, rightDegree, rightDegree);
	int p3Degree = 2 * rightDegree;
	for (int i = 0; i <= p3Degree; i++) {
		result[resultDegree - i] += p3[p3Degree - i];
	}

	int a0a1Degree = max(leftDegree, rightDegree);
	long long* a0a1 = new long long[a0a1Degree + 1];
	long long* b0b1 = new long long[a0a1Degree + 1];
	for (int i = 0; i <= rightDegree; i++) {
		a0a1[i] = a0[i] + a1[i];
		b0b1[i] = b0[i] + b1[i];
	}
	if (leftDegree > rightDegree) {
		a0a1[leftDegree] = a0[leftDegree];
		b0b1[leftDegree] = b0[leftDegree];
	}

	long long* mult = polynomial(a0a1, b0b1, a0a1Degree, a0a1Degree);

	int multDegree = a0a1Degree + a0a1Degree;

	for (int i = 0; i <= multDegree; i++) {
		result[i + leftDegree + 1] += mult[i];
		if (i <= p1Degree) {
			result[i + leftDegree + 1] -= p1[i];
		}
		if (i <= p3Degree) {
			result[i + leftDegree + 1] -= p3[i];
		}
	}

	delete[] a0;
	delete[] a1;
	delete[] b0;
	delete[] b1;
	delete[] p1;
	delete[] p3;
	delete[] mult;
	delete[] a0a1;
	delete[] b0b1;

	return result;
}


int main() {
	string line;
	int degree = 0;
	int coef = 0;

	cin >> degree;
	long long* first = new long long[degree + 1];
	long long* second = new long long[degree + 1];

	for (int i = 0; i <= degree; i++) {
		cin >> coef;
		first[i] = coef;
	}

	for (int i = 0; i <= degree; i++) {
		cin >> coef;
		second[i] = coef;
	}

	long long* result = polynomial(first, second, degree, degree);
	int resultSize = degree * 2 + 1;

	for (int i = 0; i < resultSize; i++) {
		cout << result[i] << " ";
	}
	cout << endl;

	delete[] first;
	delete[] second;
	delete[] result;

}