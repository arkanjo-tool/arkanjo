{

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
