#include "opencl_wrap.h"

#define set_kernel_args(kernel) set_kernel_arg(kernel, 0, sizeof(int), N); \
            set_kernel_arg(kernel, 1, sizeof(iter), iter); \
            set_no_buffers(kernel, 3); \
            create_and_set_buf(kernel, a, sizeof(float) * arr_size, sizeof(void*), 2, READONLY, FROM_MEMORY); \
            create_and_set_buf(kernel, b, sizeof(float) * arr_size, sizeof(void*), 3, READONLY, FROM_MEMORY); \
            create_and_set_buf(kernel, c, sizeof(float) * arr_size, sizeof(void*), 4, WRITEONLY, TO_MEMORY); \
            set_dimension_and_values(kernel, 3, N, N, iter);

static char dummy_kernel[] = 
"__kernel void dummy_name(const int N, const int iter, __global float* A, __global float* B, __global float* C) {\n"
"    // Get the index of the current element\n"
"    int i = get_global_id(0);\n"
"    int j = get_global_id(1);\n"
"    int k = get_global_id(2); //this is the number of iterations of the same calculations\n\n"
"    // Perform the operation only if within the bounds of the matrices\n"
"    if (i < N && j < N && k < iter) {\n"
"        int index = i * N + j;\n\n"
"        // Perform complex arithmetic operations\n"
"        float valueA = A[index];\n"
"        float valueB = B[index];\n"
"        float result = valueA + valueB; // Addition\n"
"        result *= sin(valueA) + cos(valueB); // Trigonometric functions\n"
"        result /= 1.0f + fabs(valueA - valueB); // Division and absolute difference\n"
"        result = pow(result, 2.0f); // Squaring the result\n"
"        result = sqrt(result + 0.1f); // Square root with a small constant to avoid domain error\n\n"
"        // Store the result\n"
"        C[index] = result;\n"
"    }\n"
"}";

void set_arrays(float *a, float *b, float*c, int arr_size)
{
	a[0] = (float)rand();
    b[0] = (float)rand();
    a[1] = (float)rand();
    b[1] = (float)rand();
	int i;
    for (i = 2; i < arr_size; ++i)
    {
        a[i] = a[i-1] + a[i-2];
        b[i] = b[i-1] - b[i-2];
    }
}

CLDevicePair get_fastest_parallel_device_full(void)
{
    float *a = NULL, *b = NULL, *c = NULL;
    int *gpus = NULL;

    CLDevicePair ret;
	ret.platform = -1;
	ret.device = -1;
    cl_uint no_platforms = get_number_of_platforms();
	cl_int clStatus;
	if (no_platforms == -1)
	{
		return ret;
	}
	cl_platform_id *platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id) * no_platforms);
	if (platforms == NULL)
	{
		last_err = BAD_MALLOC;
		goto ABORT;
	}
    gpus = malloc(sizeof(int) * (no_platforms+1));
    if (gpus == NULL){ /* This is kinda bad */
        free(platforms);
		last_error = BAD_MALLOC;
        goto ABORT;
    }
	clStatus = clGetPlatformIDs(no_platforms, platforms, NULL);
	
	
    long long int best_result = -1;
    struct OpenCLFunctionWrapper *kernel;
	

    int N = 1024 * 4, iter = 2; /* In total this should be ~100 GFLOP or close*/
    int arr_size = N * N;
    a = malloc(sizeof(float) * arr_size);
    b = malloc(sizeof(float) * arr_size);
    c = malloc(sizeof(float) * arr_size);
    if (a == NULL || b == NULL || c == NULL){
        last_error = BAD_MALLOC;
        goto ABORT;
    }
	set_arrays(a, b, c, arr_size);
	
    char *log = malloc(100000);
    memset(log, '\0', 99999);
	
	char platformName[1024];
	memset(platformName, '\0', 1024);
	
    int i,j;
    for (i = 0; i < no_platforms; ++i)
    {
        gpus[i] = get_platform_no_devices(i);
		
		// Get the current platform ID
		cl_platform_info param_name = CL_PLATFORM_NAME;
		size_t platform_name_len;
		
		clStatus = clGetPlatformInfo(platforms[i], param_name, 0, NULL, &platform_name_len);
		
		if (clStatus != CL_SUCCESS)
			continue;
		
		if (platform_name_len > 1024)
		{
			printf("Upsies\n");
			continue;
		}
		
		clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, platform_name_len, platformName, NULL);
		
		//printf("Platfrom name is: %s\n", platformName);
		
		if (strstr(platformName, "OpenCLOn12") != NULL)
		{
			//printf("Skipping OpenCLOn12\n");
			continue;
		}
		
        for (j = 0; j < gpus[i]; ++j)
        {
            kernel = createOpenCLWrapperStruct(dummy_kernel, KERNEL_FROM_STRING, "dummy_name", log, i, j, ENABLE_PROFILING);
            memset(log, 1, 99999);
            log[99999]='\0';
            if (kernel == NULL)
            {
                clW_get_last_error();
                printf("%s\n", clGetErrorString(last_err));
                continue;
            }
            set_kernel_arg(kernel, 0, sizeof(int), N);
            set_kernel_arg(kernel, 1, sizeof(iter), iter);
            set_no_buffers(kernel, 3);
            create_and_set_buf(kernel, a, sizeof(float) * arr_size, sizeof(void*), 2, READONLY, FROM_MEMORY);
            create_and_set_buf(kernel, b, sizeof(float) * arr_size, sizeof(void*), 3, READONLY, FROM_MEMORY);
            create_and_set_buf(kernel, c, sizeof(float) * arr_size, sizeof(void*), 4, WRITEONLY, TO_MEMORY);
            set_dimension_and_values(kernel, 3, N, N, iter);
            long long int time = call_function(kernel, HIGH_PERF_CLOCK);
            
			//printf("Time on current device: %d, device is: %d %d\n", time, i, j);
			//clW_get_last_error();
            //printf("%s\n", clGetErrorString(last_err));
			
            if (best_result > time && time > 0)
            {
                best_result = time;
                ret.platform = i;
                ret.device = j; 
            }
			else if (best_result == -1)
			{
				best_result = time;
				ret.platform = i;
				ret.device = j;
			}
			//printf("ret is now: %d and %d\n", ret.platform, ret.device);
            cleanupOpenCLWrapper(kernel);
        }
    }
	free(gpus);
    if (a != NULL)
	    free(a);
	if (b != NULL)
        free(b);
    if (c != NULL)
	    free(c);
	//printf("ret parameters: %d and %d\n", ret.platform, ret.device);
    return ret;

ABORT:
    if (gpus != NULL)
	    free(gpus);
    if (a != NULL)
	    free(a);
	if (b != NULL)
        free(b);
    if (c != NULL)
	    free(c);
	//printf("ABORTED\n");
    ret.platform = -1;
    ret.device = -1;
    return ret;
}

/**
 * @brief Get the fastest device parallel computing, does not return the platform of it
 * 
 * @return int the number of the device (i.e. the first, second and so on)
 */
int get_fastest_parallel_device(void)
{
    return get_fastest_parallel_device_full().device;
}
