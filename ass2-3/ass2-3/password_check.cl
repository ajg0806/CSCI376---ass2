__kernel void password_check(__global int *output, __global int *output2) { 

   /* Access work-item/work-group information */
   size_t global_id_0 = get_global_id(0);
   size_t global_id_1 = get_global_id(1);
   size_t global_size_0 = get_global_size(0);
   size_t offset_0 = get_global_offset(0);
   size_t offset_1 = get_global_offset(1);

   /* Determine array index */
   int index_0 = global_id_0 - offset_0;
   int index_1 = global_id_1 - offset_1;
   int index = index_1 * global_size_0 + index_0;
   
   /* Set float data */
   int a = global_id_0;
   int b = global_id_1;
   output[index] = a;
   output2[index] = b;
}
