#define _CRT_SECURE_NO_WARNINGS
#define PROGRAM_FILE "vecadd.cl"
#define KERNEL_FUNC "vecadd"
#define ARRAY_LENGTH 10

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
		return tolower(tmp);
	}

	tmp = (char)(c + offset);
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
   cout << "Press ENTER to see decryption." << endl;
   getchar();
   cout << "\n\n";
   
   text = shift_text(text, offset);
   for (int i = 0; i < text.size(); i++)
	   cout << text[i];
   cout << endl;


///////////////////////////////////////////////////////////////////////////////////////////////////////
   // SECTION 1
   // Declare data and buffers
   char *array_a = new char[size];
   char *array_result = new char[size];
   cl_mem buffer_a;				// buffer objects
   cl_mem buffer_b;				
   cl_mem buffer_c;				// c = a + b
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
   for(i=0; i < size; i++) {
      array_a[i] = text[i];					// Set the values in the array from 2 to 1001
	  cout << text[i];
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

   // Buffer for the result
   buffer_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
	   sizeof(char)*size, NULL, &err);
   if(err < 0)
      handle_error("Couldn't create result1 buffer");

///////////////////////////////////////////////////////////////////////////////////////////////////////
   // SECTION 4
   // Set buffers as arguments to the kernel
   // Set kernel arguments for c = a + b
   err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer_a);
   err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &buffer_b);
   err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &buffer_c);
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

   // Check the results
   result_check = CL_TRUE;


   if (result_check) {
	   printf("Successfully processed %d array elements.\n", size);
	   for (int i = 0; i < size; i++)
		   cout << array_result[i];
   }
///////////////////////////////////////////////////////////////////////////////////////////////////////

   /* Wait until a key is pressed before exiting */
   getchar();	

   /* Deallocate resources */
   clReleaseMemObject(buffer_a);
   clReleaseMemObject(buffer_b);
   clReleaseMemObject(buffer_c);
   clReleaseKernel(kernel);
   clReleaseCommandQueue(queue);
   clReleaseProgram(program);
   clReleaseContext(context);

   return 0;
}
