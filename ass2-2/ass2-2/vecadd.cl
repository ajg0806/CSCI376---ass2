__kernel void vecadd(__global char *a,
					 __global int *b,
					 __global char *c) {

   int i = get_global_id(0);

   int num = a[i];

   if(num >= 'a'){
		num += *b;

		while(num < 'a')
			num += 26;

		while(num > 'z')
			num -= 26;

		num -= ('a' - 'A');
	}

	else{
		num -= *b;

		while(num < 'A')
			num += 26;

		while(num > 'Z')
			num -= 26;
	}


   c[i] = (char)(num);
}
