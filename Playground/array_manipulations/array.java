class array{
	int[] my_arr;
  	array(int[] arr){
  		my_arr = arr;
  	}
  	array(int i){
  		my_arr = int_to_arr(i);
  	}
  	min_max find_min_max(){
  		if(my_arr == null){ return null; }

  		int l = my_arr.length;
		int min = my_arr[0];
		int max = my_arr[0];	
  		for(int i = 0; i<l; i++){
  			max = max(my_arr[i],max);
  			min = min(my_arr[i],min);
  		}
  		return new min_max(min,max);
  	}
  	int max(int a, int b){
  		if (b>=a){
  			return b;
  		}
  		return a;
  	}
  	int min(int a, int b){
  		if (b>=a){
  			return a;
  		}
  		return b;
  	}
  	int[] int_to_arr(int i){
		int temp = i;
		if(i == 0){
			return new int[] {0};
		}
		int size = 0;
		while (temp > 0){
			temp /=10;
			size++;
		}
  	}

}
class min_max{
	int min;
	int max;
	min_max(int min1, int max1){
		min = min1;
		max = max1;
	}
}