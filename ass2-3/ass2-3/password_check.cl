int8 get_ints(char8 chars)
{
	int8 ints = (int8)(0,0,0,0,0,0,0,0);

	ints.s0 = (int)(chars.s0 - 'A');
	ints.s1 = (int)(chars.s1 - 'A');
	ints.s2 = (int)(chars.s2 - 'A');
	ints.s3 = (int)(chars.s3 - 'A');
	ints.s4 = (int)(chars.s4 - 'A');
	ints.s5 = (int)(chars.s5 - 'A');

	return ints;
}

bool hit(int8 v){

	char8 password = (char8)('D', 'B', 'A', 'A', 'A', 'A', 'A', 'A');
	int8 password_int = get_ints(password);
	int8 result = password_int == v;


	if(all(result != 0))
		return true;
	return false;
}



__kernel void password_check(__global int *output) { 

   /* Access work-item/work-group information */
   size_t global_id_0 = get_global_id(0);
   size_t global_id_1 = get_global_id(1);
   
   /* Set float data */
   int a = (int)(global_id_0);
   int b = (int)(global_id_1);

   int8 v = (int8)(a,b, 0, 0, 0, 0, 0, 0);

   if (hit(v))
		vstore8(v, 0, output);
}
