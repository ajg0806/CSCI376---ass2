__kernel void shuffle_test(__global char *array1, __global char *output) {
   /* Execute the first example */

   char8 v = vload8(0, array1);
	vstore8(v, 0, output);
}
