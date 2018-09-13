__kernel void vecadd(__global char *a,
					 __global int *b,
					 __global char *c) {

   int i = get_global_id(0);

   int num = a[i] + *b;
	if(num > 'Z')
		num -= 26;
	if(num < 'A')
		num += 26;
   c[i] = (char)(num);
}
