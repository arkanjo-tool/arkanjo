{

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
