#define _CRT_SECURE_NO_WARNINGS
#define PROGRAM_FILE "password_check.cl"
#define KERNEL_FUNC "password_check"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cmath>
using namespace std;

#ifdef MAC
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

/* Find a GPU or CPU associated with the first available platform */
cl_device_id create_device() {

	cl_platform_id platform;
	cl_device_id dev;
	int err;

	/* Identify a platform */
	err = clGetPlatformIDs(1, &platform, NULL);
	if (err < 0) {
		perror("Couldn't identify a platform");
		getchar();
		exit(1);
	}

	/* Access a device */
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &dev, NULL);
	if (err == CL_DEVICE_NOT_FOUND) {
		err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &dev, NULL);
	}
	if (err < 0) {
		perror("Couldn't access any devices");
		getchar();
		exit(1);
	}

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
	if (program_handle == NULL) {
		perror("Couldn't find the program file");
		getchar();
		exit(1);
	}
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
	if (err < 0) {
		perror("Couldn't create the program");
		getchar();
		exit(1);
	}
	free(program_buffer);

	/* Build program */
	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (err < 0) {

		/* Find size of log and print to std output */
		clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG,
			0, NULL, &log_size);
		program_log = (char*)malloc(log_size + 1);
		program_log[log_size] = '\0';
		clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG,
			log_size + 1, program_log, NULL);
		printf("%s\n", program_log);
		free(program_log);
		getchar();
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
	cl_int i, err;

	/* Data and buffers */
	size_t dim = 3;
	size_t global_offset[] = { 0, 0, 0 };
	size_t global_size[] = { 26, 26, 26 };
	size_t local_size[] = { 2, 2, 2};

	char password[3];
	int test[4];
	cl_mem test_buffer, password_buffer;

	char c;
	bool valid = false;
	do {
		cout << "Please enter a 3 letter password: ";
		for (int i = 0; i < 3; i++) {
			cin >> c;

			if (c >= 'a' && c <= 'z') {
				c = c - ('a' - 'A');
				valid = true;
				password[i] = c;
			}
			else if (c >= 'A' && c <= 'Z')
			{
				valid = true;
				password[i] = c;
			}
			else {
				valid = false;
				i += 50;
			}
		}
		cin.ignore(100, '\n');
	} while (!valid);
	/* Create a device and context */
	device = create_device();
	context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
	if (err < 0) {
		perror("Couldn't create a context");
		getchar();
		exit(1);
	}

	/* Build the program and create a kernel */
	program = build_program(context, device, PROGRAM_FILE);
	kernel = clCreateKernel(program, KERNEL_FUNC, &err);
	if (err < 0) {
		perror("Couldn't create a kernel");
		getchar();
		exit(1);
	};

	/* Create a write-only buffer to hold the output data */
	test_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
		sizeof(test), NULL, &err);
	if (err < 0) {
		perror("Couldn't create a buffer");
		getchar();
		exit(1);
	};
	password_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		sizeof(password), password, &err);

	/* Create kernel argument */
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &test_buffer);
	if (err < 0) {
		perror("Couldn't set a kernel argument");
		getchar();
		exit(1);
	};
	err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &password_buffer);
	if (err < 0) {
		perror("Couldn't set a kernel argument");
		getchar();
		exit(1);
	};

	/* Create a command queue */
	queue = clCreateCommandQueue(context, device, 0, &err);
	if (err < 0) {
		perror("Couldn't create a command queue");
		getchar();
		exit(1);
	};

	/* Enqueue kernel */
	err = clEnqueueNDRangeKernel(queue, kernel, dim, global_offset,
		global_size, local_size, 0, NULL, NULL);
	if (err < 0) {
		perror("Couldn't enqueue the kernel");
		getchar();
		exit(1);
	}

	/* Read and print the result */
	err = clEnqueueReadBuffer(queue, test_buffer, CL_TRUE, 0,
		sizeof(test), &test, 0, NULL, NULL);
	if (err < 0) {
		perror("Couldn't read the buffer");
		getchar();
		exit(1);
	}

	cout << "The password you entered was: ";
	for (int j = 0; j < 3; j++) {
		cout << (char)(test[j]+'A');
	}
	cout << endl;

	/* Wait for a key press before exiting */

	getchar();

	/* Deallocate resources */
	clReleaseMemObject(test_buffer);
	clReleaseMemObject(password_buffer);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(queue);
	clReleaseProgram(program);
	clReleaseContext(context);
	return 0;
}