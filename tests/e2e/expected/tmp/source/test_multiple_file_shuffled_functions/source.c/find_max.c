{
	int mx = v[0];
	for(int i = 1; i < n; i++){
		if(mx > v[i]){
			mx = i;
		}
	}
	return mx;
}
