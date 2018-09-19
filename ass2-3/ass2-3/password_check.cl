int4 get_ints(char4 chars)
{
	int4 ints = (int4)(0,0,0,0);

	ints.s0 = (int)(chars.s0 - 'A');
	ints.s1 = (int)(chars.s1 - 'A');
	ints.s2 = (int)(chars.s2 - 'A');

	return ints;
}

bool hit(int4 v, global const char* password){

	//char4 pv = (char4)('D', 'B', 'H', 'A');
	char4 pv = vload4(0, password);
	int4 p_int = get_ints(pv);
	int4 result = p_int == v;

	if(all(result != 0))
		return true;
	return false;
}



__kernel void password_check(__global int *output, __global const char *password) { 

   /* Access work-item/work-group information */
   size_t global_id_0 = get_global_id(0);
   size_t global_id_1 = get_global_id(1);
   size_t global_id_2 = get_global_id(2);
   
   /* Set float data */
   int a = (int)(global_id_0);
   int b = (int)(global_id_1);
   int c = (int)(global_id_2);

   int4 v = (int4)(a, b, c, 0);

   if (hit(v, password))
		vstore4(v, 0, output);
}
