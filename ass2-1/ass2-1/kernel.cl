__kernel void shuffle_test(__global *array1, __global int *output) {
   /* Execute the first example */
   int8 v = *array1;
   v *= 0;
   int8 v1 = (int8)(5, 5, 5, 5, 5, 5, 5, 5);
   int8 v2 = (int8)(2, 2, 2, 2, 2, 2, 2, 2);
   int8 mask1 = (int8)(v > 5);
   int8 mask2 = (int8)(-1, -1, -1, -1, 0, 0, 0, 0);
   int8 results;

	if(any(mask1 != 0))
		results = select(v2, v1, mask1);

	else
		results = select(v2, v1, mask2);

	vstore8(results, 0, output);
}
