{
	int *v_sorted;
	int *dist_to_median;
	int *p;
	int median;

	v_sorted = compute_v_sorted(v,n);
	median = v[v_sorted[n/2]];
	dist_to_median = compute_dist_to_median(v,n,median);
	p = compute_p(dist_to_median,n);

	free(v_sorted);
	free(dist_to_median);
	return p;
}
