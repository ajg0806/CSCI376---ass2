__kernel void vecadd(__global char *a,
					 __global float *b,
					 __global char *c) {

   int i = get_global_id(0);
   c[i] = a[i];
}
