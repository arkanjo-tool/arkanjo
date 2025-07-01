#include <stdio.h>

//three equal sum functions with different names
int sum_of_two_elements(int left_hand, int right_hand){
	return left_hand + right_hand;
}

int sum(int a, int b){
	return a+b;
}

int sum_of_pair(int left, int right){
	return left+right;
}

int main(int argc, char *argv[]){
	int A = 5;
	int B = 4;

	int C = sum_of_two_elements(A,B);
	int D = sum(A,B);
	int E = sum_of_pair(A,B);
	return 0;
}
