__kernel void vecadd(__global char *a,
					 __global int *b,
					 __global char *c, __global char *d, __global char *e) {

   int i = get_global_id(0);

	int num1 = a[i];
	num1 += *b;
	while(num1 < 'a')
		num1 += 26;
	while(num1 > 'z')
		num1 -= 26;
	num1 -= ('a' - 'A');
	c[i] = (char)(num1);

	int num2 = d[i];
	num2 -= *b;
	while(num2 < 'A')
		num2 += 26;
	while(num2 > 'Z')
		num2 -= 26;
	num2 += ('a' - 'A');
	e[i] = (char)(num2);


   
   
}
