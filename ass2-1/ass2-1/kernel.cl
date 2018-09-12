__kernel void shuffle_test(__global int8 *array1, __global int8 *output) {
   /* Execute the first example */
   int8 v = *array1;
   v *= 0;
   int8 v1 = (int8)(5, 5, 5, 5, 5, 5, 5, 5);
   int8 v2 = (int8)(2, 2, 2, 2, 2, 2, 2, 2);
   int8 mask1 = (int8)(v > 5);
   int8 mask2 = (int8)(-1, -1, -1, -1, 0, 0, 0, 0);

	if(any(mask1 != 0))
		*output = select(v2, v1, mask1);
	else
		*output = select(v2, v1, mask2);
}
