__kernel void password_check(__global char *output) { 

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
   char f = (char)((global_id_0 * 10 + global_id_1 * 1)+'A');


   output[index] = f;
}
