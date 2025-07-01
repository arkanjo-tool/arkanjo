#include <stdio.h>

//two equal sum functions with different logic
int sum_of_two_elements(int left_hand, int right_hand){
	return left_hand + right_hand;
}

int sum(int a, int b){
	int ret = a+b;
	return ret;
}

//tree functions that find the sum of elements of an array
int sum_of_array(int array[], int n){
	int ret = 0;
	for(int i = 0; i < n; i++){
		ret += array[i];
	}
	return ret;
}

int sum_of_array2(int n, int array[]){
	int ret = 0;
	for(int i = 0; i < n; i++){
		ret += array[i];
	}
	return ret;
}

int sum_of_vector(int vector[], int size_of_vector){
	int ret = 0;
	for(int i = size_of_vector-1; i >= 0; i--){
		ret += vector[i];
	}
	return ret;
}


int main(int argc, char *argv[]){

	int v[5];
	v[0] = 1;
	v[1] = 2;
	v[2] = 3;
	v[3] = 4;
	v[4] = 5;
	int n = 5;
	int C = sum_of_array(v,5);
	int D = sum_of_array2(5,v);
	int E = sum_of_vector(v,5);
	return 0;
}
