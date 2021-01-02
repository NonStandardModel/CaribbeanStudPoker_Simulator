
/*
Functions count and sort inhere were taken from stackoverflow, all kudos to origianl authors.
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
// count occurences of x in array of 5 elements
// done as simple loop even if we know that arr is sorted and we could stop once
// arr[i] > x, but for 5 elements arr it probably makes no difference on average
int count5(int arr[], int x) {
	int cnt = 0;
	for (int i = 0; i < 5; i++) {
		if (arr[i] == x) {cnt++;}
	}
	return cnt;
}
// count for 2 element array
int count2(int arr[], int x) {
	int cnt = 0;
	if (arr[0] == x) {cnt++;}
	if (arr[1] == x) {cnt++;}
	return cnt;
}

// probably we could do better/faster sorting algorithm for use inhere
// note that as of now we sort only arrays of lengths 5 and 2
// EDIT: done bellow, left this in just in case
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
// sort for 5 element int array
void sort5(int arr[]) {
	int tmp;
	for (int i = 0; i < 5; i++) {
		for (int j = i + 1; j < 5; j++) {
			if (arr[j] < arr[i]) {
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}
// sort for 2 element int array is just conditional swap
void sort2(int arr[]) {
	if (arr[0] > arr[1]) {
		int tmp = arr[0];
		arr[0] = arr[1];
		arr[1] = tmp;
	}
}


















