#include <stdio.h>
#include <stdlib.h>

//the two functions below will compute the vector permutation p
//which returns a list of the index of elements sorted by distance of median
//I do not care about ties.


int *sorted_by_distance_to_median(int v[], int n){

	int *v_sorted = (int*)malloc(n*sizeof(int));
	int *dist_to_median = (int*)malloc(n*sizeof(int));
	int *p = (int*)malloc(n*sizeof(int));
	int median;

	for(int i = 0; i < n; i++){
		v_sorted[i] = i;
	}

	for(int i = 0; i < n; i++){
		for(int j = 0; j < n-1; j++){
			if(v[v_sorted[j]] > v[v_sorted[j+1]]){
				int aux = v_sorted[j];
				v_sorted[j] = v_sorted[j+1];
				v_sorted[j+1] = aux;
			}
		}
	}

	median = v[v_sorted[n/2]];
		
	for(int i = 0; i < n; i++){
		dist_to_median[i] = v[i]-median;
		if(dist_to_median[i] < 0){
			dist_to_median[i] *= -1;
		}
	}

	for(int i = 0; i < n; i++){
		p[i] = i;
	}

	for(int i = 0; i < n; i++){
		for(int j = 0; j < n-1; j++){
			if(dist_to_median[p[j]] > dist_to_median[p[j+1]]){
				int aux = p[j];
				p[j] = p[j+1];
				p[j+1] = aux;
			}
		}
	}

	free(v_sorted);
	free(dist_to_median);
	return p;
}

int *sorted_by_distance_of_median(int v[], int n){

	int median;
	int *p = (int*)malloc(n*sizeof(int));
	int *v_sorted= (int*)malloc(n*sizeof(int));
	int *dist_to_median = (int*)malloc(n*sizeof(int));


	for(int i = 0; i < n; i++){
		v_sorted[i] = i;
	}

	for(int i = 0; i < n; i++){
		for(int j = 0; j < n-1; j++){
			if(v[v_sorted[j]] > v[v_sorted[j+1]]){
				int aux = v_sorted[j];
				v_sorted[j] = v_sorted[j+1];
				v_sorted[j+1] = aux;
			}
		}
	}

	median = v[v_sorted[n/2]];

	for(int i = 0; i < n; i++){
		dist_to_median[i] = v[i]-median;
		if(dist_to_median[i] < 0)
			dist_to_median[i] = -dist_to_median[i];
	}

	for(int i = 0; i < n; i++)
		p[i] = i;

	for(int i = 0; i < n; i++){
		for(int j = 0; j < n-1; j++){
			if(dist_to_median[p[j]] > dist_to_median[p[j+1]]){
				int aux = p[j];
				p[j] = p[j+1];
				p[j+1] = aux;
			}
		}
	}

	free(dist_to_median);
	free(v_sorted);
	return p;
}

int main(int argc, char *argv[]){

	int v[6];
	v[0] = 5;
	v[1] = 3;
	v[2] = 4;
	v[3] = 1;
	v[4] = 2;
	v[5] = 1;
	int n = 6;

	int *p1 = sorted_by_distance_to_median(v,n);
	int *p2 = sorted_by_distance_of_median(v,n);
	

	for(int i = 0; i < n; i++){
		printf("%d ",p1[i]);
	}
	printf("\n");
	
	for(int i = 0; i < n; i++){
		printf("%d ",p2[i]);
	}
	printf("\n");
	
	free(p1);
	free(p2);
	return 0;
}
