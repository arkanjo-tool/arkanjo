{
	bool ok = true;
	for(int i = 0; i < n-1; i++){
		ok &= v[i] <= v[i+1];
	}
	return ok;
}
