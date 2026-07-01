{
	int *p = (int*)malloc(n*sizeof(int));

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
	return p;
}
