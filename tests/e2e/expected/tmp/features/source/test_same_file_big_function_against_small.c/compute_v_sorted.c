{
	int *v_sorted = (int*)malloc(n*sizeof(int));

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
	return v_sorted;
}
