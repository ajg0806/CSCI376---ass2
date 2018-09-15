__kernel void shiftcipher(__global char *a,
					 __global int *b,
					 __global char *c, __global char *d, __global char *e,  __global char *f,
					 __global char *g,  __global char *l1, __global char *l2) {

   int i = get_global_id(0);


   /********************TASK2********************/

   //Encryption of text using offset
	int num1 = a[i];
	num1 += *b;
	//Simplified modulo (works with kernels)
	while(num1 < 'a')
		num1 += 26;
	while(num1 > 'z')
		num1 -= 26;
	num1 -= ('a' - 'A');
	c[i] = (char)(num1);

	//Decryption of text using offset
	//Almost identical to encryption
	int num2 = d[i];
	num2 -= *b;
	while(num2 < 'A')
		num2 += 26;
	while(num2 > 'Z')
		num2 -= 26;
	num2 += ('a' - 'A');
	e[i] = (char)(num2);


   /******************TASK3***********************/
   //Lookup to encrypt
   f[i] = l1[(int)(a[i]-'a')];
   //Lookup to decrypt
   g[i] = l2[(int)(f[i]-'A')];
}
