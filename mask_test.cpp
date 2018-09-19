#define _CRT_SECURE_NO_WARNINGS
#define PROGRAM_FILE "mask_test.cl"
#define KERNEL_FUNC "mask_test"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <time.h>
using namespace std;

#ifdef MAC
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

/* Find a GPU or CPU associated with the first available platform */
cl_device_id create_device()
{

	cl_platform_id platform;
	cl_device_id dev;
	int err;

	/* Identify a platform */
	err = clGetPlatformIDs(1, &platform, NULL);
	if (err < 0)
	{
		perror("Couldn't identify a platform");
		exit(1);
	}

	/* Access a device */
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &dev, NULL);
	if (err == CL_DEVICE_NOT_FOUND)
	{
		err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &dev, NULL);
	}
	if (err < 0)
	{
		perror("Couldn't access any devices");
		exit(1);
	}

	return dev;
}

/* Create program from a file and compile it */
cl_program build_program(cl_context ctx, cl_device_id dev, const char *filename)
{

	cl_program program;
	FILE *program_handle;
	char *program_buffer, *program_log;
	size_t program_size, log_size;
	int err;

	/* Read program file and place content into buffer */
	program_handle = fopen(filename, "r");
	if (program_handle == NULL)
	{
		perror("Couldn't find the program file");
		exit(1);
	}
	fseek(program_handle, 0, SEEK_END);
	program_size = ftell(program_handle);
	rewind(program_handle);
	program_buffer = (char *)malloc(program_size + 1);
	program_buffer[program_size] = '\0';
	fread(program_buffer, sizeof(char), program_size, program_handle);
	fclose(program_handle);

	/* Create program from file */
	program = clCreateProgramWithSource(ctx, 1,
										(const char **)&program_buffer, &program_size, &err);
	if (err < 0)
	{
		perror("Couldn't create the program");
		exit(1);
	}
	free(program_buffer);

	/* Build program */
	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (err < 0)
	{

		/* Find size of log and print to std output */
		clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG,
							  0, NULL, &log_size);
		program_log = (char *)malloc(log_size + 1);
		program_log[log_size] = '\0';
		clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG,
							  log_size + 1, program_log, NULL);
		printf("%s\n", program_log);
		free(program_log);
		exit(1);
	}

	return program;
}

void loadArrays(int *array1, int *array2)
{
	srand(time(NULL));
	for (int i = 0; i < 8; i++)
	{
		array1[i] = rand() % 10;
		array2[i] = i + 1;
	}
	for (int j = 0; j < 8; j++)
		array2[j + 8] = -8 + j;
}

int main()
{

	/* Host/device data structures */
	cl_device_id device;
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	cl_kernel kernel;
	cl_int i, err;

	/* Data and buffers */
	int array1[8];
	int array2[16];
	int output[8];
	cl_mem array1_buffer, array2_buffer, output_buffer;

	loadArrays(array1, array2);

	/* Create a context */
	device = create_device();
	context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
	if (err < 0)
	{
		perror("Couldn't create a context");
		exit(1);
	}

	/* Build the program and create a kernel */
	program = build_program(context, device, PROGRAM_FILE);
	kernel = clCreateKernel(program, KERNEL_FUNC, &err);
	if (err < 0)
	{
		perror("Couldn't create a kernel");
		exit(1);
	};

	/* Create a read-only buffer to hold the input data */
	array1_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
								   sizeof(array1), array1, &err);
	if (err < 0)
	{
		perror("Couldn't create a buffer");
		exit(1);
	};
	array2_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
								   sizeof(array2), array2, &err);
	/* Create a write-only buffer to hold the output data */
	output_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
								   sizeof(output), NULL, NULL);

	/* Create kernel argument */
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &array1_buffer);
	if (err < 0)
	{
		perror("Couldn't set a kernel argument");
		exit(1);
	};
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &array2_buffer);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), &output_buffer);

	/* Create a command queue */
	queue = clCreateCommandQueue(context, device, 0, &err);
	if (err < 0)
	{
		perror("Couldn't create a command queue");
		exit(1);
	};

	/* Enqueue kernel */
	err = clEnqueueTask(queue, kernel, 0, NULL, NULL);
	if (err < 0)
	{
		perror("Couldn't enqueue the kernel");
		exit(1);
	}

	/* Read and print the result */
	err = clEnqueueReadBuffer(queue, output_buffer, CL_TRUE, 0,
							  sizeof(output), &output, 0, NULL, NULL);
	if (err < 0)
	{
		perror("Couldn't read the buffer");
		exit(1);
	}

	printf("Output:\t\t");
	for (i = 0; i < 7; i++)
	{
		cout << output[i] << ", ";
	}
	cout << output[7] << endl;

	//If any value from array1 is over five the first requirement is met
	bool use_mask1 = false;
	for (int j = 0; j < 8; j++)
	{
		if (array1[j] > 5)
			use_mask1 = true;
	}
	cout << "Output check:\t";
	if (use_mask1)
	{
		for (int i = 0; i < 8; i++)
		{
			if (array1[i] > 5)
				cout << array2[i] << ", ";
			else
				cout << array2[i + 8] << ", ";
		}
		cout << endl;
	}
	//If the first requirement failed the default mask is used
	else
	{
		for (int k = 0; k < 4; k++)
			cout << array2[k] << ", ";
		for (int l = 12; l < 16; l++)
			cout << array2[l] << ", ";
		cout << endl;
	}

	/* Wait for a key press before exiting */
	getchar();

	/* Deallocate resources */
	clReleaseMemObject(array1_buffer);
	clReleaseMemObject(output_buffer);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(queue);
	clReleaseProgram(program);
	clReleaseContext(context);
	return 0;
}