 __kernel void mask_test(__global *array1, __global *array2, __global int *output) {
   /* Execute the first example */

   int8 v = vload8(0, array1);
   int8 v1 = vload8(0, array2);
   int8 v2 = vload8(1, array2);

   //Creates a mask of where v is greater than 5
   int8 mask1 = (int8)(v > 5);
   
   //Backup mask if no elements in v are greater than 5 
   int8 mask2 = (int8)(-1, -1, -1, -1, 0, 0, 0, 0);
   
   //Result vector
   int8 results;

	//Comment here
	if(any(mask1 != 0))
		results = select(v2, v1, mask1);
	
	//Comment here
	else
		results = select(v2, v1, mask2);

	//Comment here
	vstore8(results, 0, output);
}
