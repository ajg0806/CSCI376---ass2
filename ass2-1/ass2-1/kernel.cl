__kernel void shuffle_test(__global int8 *s0, __global int8 *s1) {

   /* Execute the first example */
   int8 input = *s0; 
   int8 v1 = (int8)(25, 5, 75, 1, 125, 150, 175, 20);
   int8 v2 = (int8)(25, 5, 75, 1, 125, 150, 175, 20);
  // input = vload8(0, &s0[get_global_id(0)]);

   //input = v1+v2;

   *s1 = input;
}
