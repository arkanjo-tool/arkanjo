#include <stdio.h>
#include <stdbool.h>

bool check_sorted(int v[], int n){
	bool ok = true;
	for(int i = 0; i < n-1; i++){
		ok &= v[i] <= v[i+1];
	}
	return ok;
}

int find_max(int v[], int n){
	int mx = v[0];
	for(int i = 1; i < n; i++){
		if(mx > v[i]){
			mx = i;
		}
	}
	return mx;
}

int find_sum(int v[], int n){
	int sum = 0;
	for(int i = 0; i < n; i++){
		sum += v[i];
	}
	return sum;
}

int find_min(int v[], int n){
	int mn = v[0];
	for(int i = 1; i < n; i++){
		if(mn > v[i]){
			mn = i;
		}
	}
	return mn;
}

int main(int argc, char *argv[]){

	int v[5];
	v[0] = 1;
	v[1] = 2;
	v[2] = 3;
	v[3] = 4;
	v[4] = 5;
	int n = 5;
	int C = find_min(v,n);
	int D = find_max(v,n);
	int E = find_sum(v,n);
	bool is_sorted = check_sorted(v,n);
	return 0;
}
