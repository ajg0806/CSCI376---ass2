#define _CRT_SECURE_NO_WARNINGS
#define PROGRAM_FILE "vecadd.cl"
#define KERNEL_FUNC "vecadd"

#ifdef MAC
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <fstream>
#include <ctype.h>

using namespace std;

vector<char> getPlaintext() {
	vector<char> str;
	ifstream ifs;
	ifs.open("plaintext");
	char c;
	while (!ifs.eof())
	{
		ifs.get(c);

		if (!ifs.eof())
			str.push_back(c);
	}
	return str;
}

vector<char> formatPlainText(vector<char> str) {
	vector<char> fmtted;

	for (int i = 0; i < str.size(); i++) {
		char c = str[i];
		if ((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A'))
			fmtted.push_back(tolower(c));
	}
	return fmtted;
}

void loadArrays(char* array1) {
	srand(time(NULL));
	for (int i = 0; i < 8; i++)
		array1[i] = (char)((rand() % 26) + 97);
}

char shift_character(char c, int offset) {
	char tmp;
	if (isupper(c)) {
		tmp = (char)(c - offset);
		while (tmp < 'A')
			tmp = (char)(tmp+26);
		while (tmp > 'Z')
			tmp = (char)(tmp - 26);

		return tolower(tmp);
	}

	tmp = (char)(c + offset);

	while (tmp < 'a')
		tmp = (char)(tmp + 26);
	while (tmp > 'z')
		tmp = (char)(tmp - 26);

	return toupper(tmp);

}

vector<char> shift_text(vector<char> text, int offset) {
	vector<char> shifted_text;
	for (int i = 0; i < text.size(); i++)
		shifted_text.push_back(shift_character(text[i], offset));

	return shifted_text;
}

/* Prints error message and exits */
void handle_error(const char* msg)
{
   perror(msg);

   /* Wait until a key is pressed before exiting */
   getchar();	
	
   exit(1);   
}

/* Find a GPU or CPU associated with the first available platform */
cl_device_id create_device() {

   cl_platform_id platform;
   cl_device_id dev;
   int err;

   /* Identify a platform */
   err = clGetPlatformIDs(1, &platform, NULL);
   if(err < 0)
      handle_error("Couldn't identify a platform");

   /* Access a device */
   err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &dev, NULL);
   if(err == CL_DEVICE_NOT_FOUND) {
      err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &dev, NULL);
   }
   if(err < 0)
      handle_error("Couldn't access any devices");

   return dev;
}

/* Create program from a file and compile it */
cl_program build_program(cl_context ctx, cl_device_id dev, const char* filename) {

   cl_program program;
   FILE *program_handle;
   char *program_buffer, *program_log;
   size_t program_size, log_size;
   int err;

   /* Read program file and place content into buffer */
   program_handle = fopen(filename, "r");
   if(program_handle == NULL)
      handle_error("Couldn't find the program file");

   fseek(program_handle, 0, SEEK_END);
   program_size = ftell(program_handle);
   rewind(program_handle);
   program_buffer = (char*)malloc(program_size + 1);
   program_buffer[program_size] = '\0';
   fread(program_buffer, sizeof(char), program_size, program_handle);
   fclose(program_handle);

   /* Create program from file */
   program = clCreateProgramWithSource(ctx, 1, 
      (const char**)&program_buffer, &program_size, &err);
   if(err < 0)
      handle_error("Couldn't create the program");

   free(program_buffer);

   /* Build program */
   err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
   if(err < 0) {

      /* Find size of log and print to std output */
      clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG, 
            0, NULL, &log_size);
      program_log = (char*) malloc(log_size + 1);
      program_log[log_size] = '\0';
      clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG, 
            log_size + 1, program_log, NULL);
      printf("%s\n", program_log);
      free(program_log);
      exit(1);
   }

   return program;
}

char *lookup_generate_01()
{
	char *l1 = new char[26];

	l1[0] = 'C';
	l1[1] = 'I';
	l1[2] = 'S';
	l1[3] = 'Q';
	l1[4] = 'V';
	l1[5] = 'N';
	l1[6] = 'F';
	l1[7] = 'O';
	l1[8] = 'W';
	l1[9] = 'A';
	l1[10] = 'X';
	l1[11] = 'M';
	l1[12] = 'T';
	l1[13] = 'G';
	l1[14] = 'U';
	l1[15] = 'H';
	l1[16] = 'P';
	l1[17] = 'B';
	l1[18] = 'K';
	l1[19] = 'L';
	l1[20] = 'R';
	l1[21] = 'E';
	l1[22] = 'Y';
	l1[23] = 'D';
	l1[24] = 'Z';
	l1[25] = 'J';

	return l1;
}

char *lookup_generate_02()
{
	char *l2 = new char[26];
	l2[0] = 'j';
	l2[1] = 'r';
	l2[2] = 'a';
	l2[3] = 'x';
	l2[4] = 'v';
	l2[5] = 'g';
	l2[6] = 'n';
	l2[7] = 'p';
	l2[8] = 'b';
	l2[9] = 'z';
	l2[10] = 's';
	l2[11] = 't';
	l2[12] = 'l';
	l2[13] = 'f';
	l2[14] = 'h';
	l2[15] = 'q';
	l2[16] = 'd';
	l2[17] = 'u';
	l2[18] = 'c';
	l2[19] = 'm';
	l2[20] = 'o';
	l2[21] = 'e';
	l2[22] = 'i';
	l2[23] = 'k';
	l2[24] = 'w';
	l2[25] = 'y';

	return l2;
}

int main() {

   /* OpenCL data structures */
   cl_device_id device;
   cl_context context;
   cl_command_queue queue;
   cl_program program;
   cl_kernel kernel;
   cl_int i, j, err;

   int offset = 3;

   vector<char> text = getPlaintext();
   text = formatPlainText(text);
   int size = text.size();
   for (int i = 0; i < text.size(); i++)
	   cout << text[i];
   cout << endl;

   cout << "Press ENTER to see encryption." << endl;
   getchar();
   cout << "\n\n";
   text = shift_text(text, offset);
   for (int i = 0; i < text.size(); i++)
	   cout << text[i];
   cout << endl;

   vector<char> other = text;

   cout << "Press ENTER to see decryption." << endl;
   getchar();
   cout << "\n\n";
   
   text = shift_text(text, offset);
   for (int i = 0; i < text.size(); i++)
	   cout << text[i];
   cout << endl;

   char *l1 = lookup_generate_01();
   char *l2 = lookup_generate_02();

///////////////////////////////////////////////////////////////////////////////////////////////////////
   // SECTION 1
   // Declare data and buffers
   char *array_a = new char[size];
   char *array_result = new char[size];
   char *array_d = new char[size];
   char *array_e = new char[size];
   char *array_f = new char[size];
   char *array_g = new char[size];
   cl_mem buffer_a;				// buffer objects
   cl_mem buffer_b;				
   cl_mem buffer_c;				// c = a + b
   cl_mem buffer_d;
   cl_mem buffer_e;
   cl_mem buffer_f;
   cl_mem buffer_g;
   cl_mem buffer_l1;
   cl_mem buffer_l2;
   size_t num_of_work_items = size;
   cl_bool result_check = CL_TRUE;

///////////////////////////////////////////////////////////////////////////////////////////////////////

   /* Create a device and context */
   device = create_device();
   context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
   if(err < 0)
      handle_error("Couldn't create a context");

   /* Build the program and create the kernel */
   program = build_program(context, device, PROGRAM_FILE);
   kernel = clCreateKernel(program, KERNEL_FUNC, &err);
   if(err < 0)
      handle_error("Couldn't create a kernel");

   /* Create a command queue */
   queue = clCreateCommandQueue(context, device, 0, &err);
   if(err < 0)
      handle_error("Couldn't create a command queue");

///////////////////////////////////////////////////////////////////////////////////////////////////////
   // SECTION 2
   // Initialise arrays
   cout << "Press ENTER to see parallel encryption." << endl;
   getchar();
   cout << "\n\n";

   for(i=0; i < size; i++) {
      array_a[i] = text[i];					// Set the values in the array from 2 to 1001
	  array_d[i] = other[i];
   }
   memset(array_result, '0', sizeof(char)*size);	// Set all values in the array to 0

///////////////////////////////////////////////////////////////////////////////////////////////////////
   // SECTION 3
   // Create buffers
   // Buffers for the input, read-only
   buffer_a = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
	   sizeof(char)*size, array_a, &err);
   if(err < 0)
      handle_error("Couldn't create buffer a");

   buffer_b = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
	   sizeof(int), &offset, &err);
   if(err < 0)
      handle_error("Couldn't create buffer b");


   buffer_l1 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
	   sizeof(char)*size, l1, &err);
   if (err < 0)
	   handle_error("Couldn't create buffer d");


   buffer_l2 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
	   sizeof(char)*size, l2, &err);
   if (err < 0)
	   handle_error("Couldn't create buffer d");



   buffer_d = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
	   sizeof(char)*size, array_d, &err);
   if (err < 0)
	   handle_error("Couldn't create buffer d");

   // Buffer for the result
   buffer_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
	   sizeof(char)*size, NULL, &err);
   if(err < 0)
      handle_error("Couldn't create result1 buffer");

   buffer_e = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
	   sizeof(char)*size, NULL, &err);
   if (err < 0)
	   handle_error("Couldn't create result1 buffer");

   buffer_f = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
	   sizeof(char)*size, NULL, &err);
   if (err < 0)
	   handle_error("Couldn't create result1 buffer");

   buffer_g = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
	   sizeof(char)*size, NULL, &err);
   if (err < 0)
	   handle_error("Couldn't create result1 buffer");

///////////////////////////////////////////////////////////////////////////////////////////////////////
   // SECTION 4
   // Set buffers as arguments to the kernel
   // Set kernel arguments for c = a + b
   err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer_a);
   err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &buffer_b);
   err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &buffer_c);
   err |= clSetKernelArg(kernel, 3, sizeof(cl_mem), &buffer_d);
   err |= clSetKernelArg(kernel, 4, sizeof(cl_mem), &buffer_e);
   err |= clSetKernelArg(kernel, 5, sizeof(cl_mem), &buffer_f);
   err |= clSetKernelArg(kernel, 6, sizeof(cl_mem), &buffer_g);
   err |= clSetKernelArg(kernel, 7, sizeof(cl_mem), &buffer_l1);
   err |= clSetKernelArg(kernel, 8, sizeof(cl_mem), &buffer_l2);
   if(err < 0)
      handle_error("Couldn't set kernel argument");

   // Enqueue kernel
//   err = clEnqueueTask(queue, kernel, 0, NULL, NULL);
   err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &num_of_work_items, NULL, 0, NULL, NULL);
   if(err < 0)
      handle_error("Couldn't enqueue the kernel");

///////////////////////////////////////////////////////////////////////////////////////////////////////
   // SECTION 5
   // Enqueue command to read from buffer c
   err = clEnqueueReadBuffer(queue, buffer_c, CL_TRUE, 
		 0, sizeof(char)*size, array_result, 0, NULL, NULL);
   if(err < 0)
      handle_error("Couldn't read from buffer result");

   err = clEnqueueReadBuffer(queue, buffer_e, CL_TRUE,
	   0, sizeof(char)*size, array_e, 0, NULL, NULL);
   if (err < 0)
	   handle_error("Couldn't read from buffer result");


   err = clEnqueueReadBuffer(queue, buffer_f, CL_TRUE,
	   0, sizeof(char)*size, array_f, 0, NULL, NULL);
   if (err < 0)
	   handle_error("Couldn't read from buffer result");


   err = clEnqueueReadBuffer(queue, buffer_g, CL_TRUE,
	   0, sizeof(char)*size, array_g, 0, NULL, NULL);
   if (err < 0)
	   handle_error("Couldn't read from buffer result");


   // Check the results
   result_check = CL_TRUE;


   if (result_check) {
	   for (int i = 0; i < size; i++)
		   cout << array_result[i];

	   cout << endl;

	   cout << "Press ENTER to see parallel decryption." << endl;
	   cout << "\n\n";
	   getchar();

	   for (int i = 0; i < size; i++)
		   cout << array_e[i];
	   cout << endl;

	   cout << "Press ENTER to see lookup table encryption." << endl;
	   cout << "\n\n";
	   getchar();

	   cout << "\n\n" << endl;
	   for (int i = 0; i < size; i++)
		   cout << array_f[i];
	   cout << endl;

	   cout << "Press ENTER to see lookup table decryption." << endl;
	   cout << "\n\n";
	   getchar();

	   cout << "\n\n" << endl;
	   for (int i = 0; i < size; i++)
		   cout << array_g[i];

   }
///////////////////////////////////////////////////////////////////////////////////////////////////////
   /* Wait until a key is pressed before exiting */
   getchar();	

   /* Deallocate resources */
   free(array_a);
   free(array_d);
   free(array_e);
   free(array_result);
   clReleaseMemObject(buffer_a);
   clReleaseMemObject(buffer_b);
   clReleaseMemObject(buffer_c);
   clReleaseMemObject(buffer_d);
   clReleaseMemObject(buffer_e);
   clReleaseKernel(kernel);
   clReleaseCommandQueue(queue);
   clReleaseProgram(program);
   clReleaseContext(context);

   return 0;
}
