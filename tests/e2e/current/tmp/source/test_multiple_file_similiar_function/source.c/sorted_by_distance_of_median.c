{

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
