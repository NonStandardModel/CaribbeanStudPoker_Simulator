
/*
Functions inhere were taken from stackoverflow, all kudos to origianl authors.
*/

/* if x is present in arr[] then returns the index of FIRST occurrence of x in arr[0..n-1], otherwise returns -1 */
int first(int arr[], int low, int high, int x, int n)  { 
	if(high >= low) { 
		int mid = (low + high)/2;  
		if( ( mid == 0 || x > arr[mid-1]) && arr[mid] == x) {return mid;} 
		else if(x > arr[mid]) {return first(arr, (mid + 1), high, x, n);} 
		else {return first(arr, low, (mid -1), x, n);} 
	} 
	return -1; 
}

/* if x is present in arr[] then returns the index of LAST occurrence of x in arr[0..n-1], otherwise returns -1 */ 
int last(int arr[], int low, int high, int x, int n) { 
	if (high >= low) { 
		int mid = (low + high)/2;  
		if( ( mid == n-1 || x < arr[mid+1]) && arr[mid] == x ) {return mid;} 
		else if(x < arr[mid]) {return last(arr, low, (mid -1), x, n);} 
		else {return last(arr, (mid + 1), high, x, n);}       
	} 
	return -1; 
}

/* if x is present in arr[] then returns the count of occurrences of x, otherwise returns -1. */
int count(int arr[], int x, int n) { 
	int i; // index of first occurrence of x in arr[0..n-1] 
	int j; // index of last occurrence of x in arr[0..n-1] 
	// get the index of first occurrence of x 
	i = first(arr, 0, n-1, x, n); 
	/* If x doesn't exist in arr[] then return -1 */
	if(i == -1) {return i;}
	/* Else get the index of last occurrence of x.  
	Note that we are only looking in the subarray after first occurrence  */    
	j = last(arr, i, n-1, x, n);       
	return j-i+1; 
}

// TODO: probably we could do better/faster sorting algorithm for use inhere
// note that as of now we sort only arrays of lengths 5 and 2
void sort(int arr[], int size) {
	int tmp;
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			if (arr[j] < arr[i]) {
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}



















