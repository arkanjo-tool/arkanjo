{
	int *dist_to_median = (int*)malloc(n*sizeof(int));

	for(int i = 0; i < n; i++){
		dist_to_median[i] = v[i]-median;
		if(dist_to_median[i] < 0){
			dist_to_median[i] *= -1;
		}
	}
	return dist_to_median;
}
