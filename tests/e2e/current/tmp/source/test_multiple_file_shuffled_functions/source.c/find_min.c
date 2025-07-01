{
	int mn = v[0];
	for(int i = 1; i < n; i++){
		if(mn > v[i]){
			mn = i;
		}
	}
	return mn;
}
