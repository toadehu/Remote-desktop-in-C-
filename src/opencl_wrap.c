#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

#ifndef CL_TARGET_OPENCL_VERSION
#define CL_TARGET_OPENCL_VERSION 300
#endif

#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include "opencl_wrap.h"


void clW_get_last_error(void)
{
    switch (last_error)
    {
	      case 0: printf("No error yet\n");break;
        case BAD_WRAPPER_MALLOC: printf("Couldn't allocate memory for the wrapper\n"); break;
        case BAD_FILE_PATH: printf("Couldn't open given file or file is empty\n"); break;
        case BAD_MALLOC: printf("Malloc failed, but not for allocating the wrapper\n"); break;
        case BAD_PLATFORMID: printf("OpenCL couldn't get platform id, opencl error code: %d\n", (int)last_err); break;
        case BAD_DEVICEID: printf("Couldn't get specified device id, default is 1, opencl error code: %d\n", (int)last_err); break;
        case BAD_CONTEXT: printf("Couldn't create OpenCL context, opencl error code: %d\n", (int)last_err); break;
        case BAD_COMMANDQUEUE: printf("Couldn't create command queue, opencl error code: %d\n", (int)last_err); break;
        case BAD_PROGRAM: printf("Couldn't create program, opencl error code: %d\n", (int)last_err); break;
        case BAD_BUILD: printf("Couldn't build program, opencl error code: %d\n", (int)last_err); break;
        case BAD_KERNEL: printf("Couldn't create kernel, opencl error code: %d\n", (int)last_err); break;
        case BAD_KERNEL_ARG: printf("Couldn't set kernel argument, opencl error code: %d\n", (int)last_err); break;
        case NO_PLACE_FOR_BUFF: printf("There is no place for the new buffer in the wrapper\n"); break;
        case BAD_ENQUEUE_WRITE: printf("Couldn't do enqueue write operation to VRAM, opencl error code: %d\n", (int)last_err); break;
        case NULL_PTR_ERROR: printf("The wrapper passed is NULL, and you also lost your last relevant error... you really shouldn't be seeing this\n"); break;
        case BAD_ENQUEUE: printf("Couldn't enqueue function, opencl error code: %d\n", (int)last_err); break;
        case BAD_ENQUEUE_READ: printf("Couldn't enqueue reading VRAM, opencl error code: %d\n", (int)last_err); break;
        case BAD_VRAM: printf("Couldn't allocate resources (VRAM for example), opencl error code: %d\n", (int)last_err); break;
        case BAD_IND: printf("Inidice outside of array encountered when trying to update the buffer \n"); break;
        case BAD_POINTER: printf("Tried to access null memory or default error caused by malloc returning NULL\n"); break;
        case INVALID_BUF: printf("Encountered an error freeing a cl_mem object, opencl error code: %d\n", (int)last_err);
        case BAD_PLATFORM_NUM: printf("Platform specified doesn't exist, either you used a negative number or too big\n"); break;
        case BAD_DEVICE_NUM: printf("Specified device number is too large for the given platform\n"); break;
	      case NO_OPENCL: printf("Something went wrong when benchmarking the hardware, verify GPU drivers and try again\n");break;
        case INVALID_PTR: printf("Invalid pointer passed\n");
        default: printf("Unkown error: %d (-1 means no error), opencl error code: %d\n", last_error, (int)last_err); break;
    }
}

const char *clGetErrorString(cl_int error)
{
    switch(error)
    {
        // run-time and JIT compiler errors
        case 0: return "CL_SUCCESS";
        case (-1): return "CL_DEVICE_NOT_FOUND";
        case (-2): return "CL_DEVICE_NOT_AVAILABLE";
        case (-3): return "CL_COMPILER_NOT_AVAILABLE";
        case (-4): return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
        case (-5): return "CL_OUT_OF_RESOURCES";
        case (-6): return "CL_OUT_OF_HOST_MEMORY";
        case (-7): return "CL_PROFILING_INFO_NOT_AVAILABLE";
        case (-8): return "CL_MEM_COPY_OVERLAP";
        case (-9): return "CL_IMAGE_FORMAT_MISMATCH";
        case (-10): return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
        case (-11): return "CL_BUILD_PROGRAM_FAILURE";
        case (-12): return "CL_MAP_FAILURE";
        case (-13): return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
        case (-14): return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
        case (-15): return "CL_COMPILE_PROGRAM_FAILURE";
        case (-16): return "CL_LINKER_NOT_AVAILABLE";
        case (-17): return "CL_LINK_PROGRAM_FAILURE";
        case (-18): return "CL_DEVICE_PARTITION_FAILED";
        case (-19): return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

        // compile-time errors
        case (-30): return "CL_INVALID_VALUE";
        case (-31): return "CL_INVALID_DEVICE_TYPE";
        case (-32): return "CL_INVALID_PLATFORM";
        case (-33): return "CL_INVALID_DEVICE";
        case (-34): return "CL_INVALID_CONTEXT";
        case (-35): return "CL_INVALID_QUEUE_PROPERTIES";
        case (-36): return "CL_INVALID_COMMAND_QUEUE";
        case (-37): return "CL_INVALID_HOST_PTR";
        case (-38): return "CL_INVALID_MEM_OBJECT";
        case (-39): return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
        case (-40): return "CL_INVALID_IMAGE_SIZE";
        case (-41): return "CL_INVALID_SAMPLER";
        case (-42): return "CL_INVALID_BINARY";
        case (-43): return "CL_INVALID_BUILD_OPTIONS";
        case (-44): return "CL_INVALID_PROGRAM";
        case (-45): return "CL_INVALID_PROGRAM_EXECUTABLE";
        case (-46): return "CL_INVALID_KERNEL_NAME";
        case (-47): return "CL_INVALID_KERNEL_DEFINITION";
        case (-48): return "CL_INVALID_KERNEL";
        case (-49): return "CL_INVALID_ARG_INDEX";
        case (-50): return "CL_INVALID_ARG_VALUE";
        case (-51): return "CL_INVALID_ARG_SIZE";
        case (-52): return "CL_INVALID_KERNEL_ARGS";
        case (-53): return "CL_INVALID_WORK_DIMENSION";
        case (-54): return "CL_INVALID_WORK_GROUP_SIZE";
        case (-55): return "CL_INVALID_WORK_ITEM_SIZE";
        case (-56): return "CL_INVALID_GLOBAL_OFFSET";
        case (-57): return "CL_INVALID_EVENT_WAIT_LIST";
        case (-58): return "CL_INVALID_EVENT";
        case (-59): return "CL_INVALID_OPERATION";
        case (-60): return "CL_INVALID_GL_OBJECT";
        case (-61): return "CL_INVALID_BUFFER_SIZE";
        case (-62): return "CL_INVALID_MIP_LEVEL";
        case (-63): return "CL_INVALID_GLOBAL_WORK_SIZE";
        case (-64): return "CL_INVALID_PROPERTY";
        case (-65): return "CL_INVALID_IMAGE_DESCRIPTOR";
        case (-66): return "CL_INVALID_COMPILER_OPTIONS";
        case (-67): return "CL_INVALID_LINKER_OPTIONS";
        case (-68): return "CL_INVALID_DEVICE_PARTITION_COUNT";

        // extension errors
        case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
        case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
        case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
        case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
        case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
        case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
        default: return "Unknown OpenCL error";
    }
}

/**
 * @brief creates the OpenCLWrapper struct, and optionally writes the build log to the build_log variable. no data is loaded
 *
 * @param arg1 the data argument, either a filename to load the code from or the kernel code itself
 * @param type what is arg1, is the kernel loaded from the string (KERNEL_FROM_STRING) or loaded from a file (KERNEL_FROM_FILE)
 * @param kernel_name the name of the kernel, it must be found within the program (look up clCreateKernel for more info)
 * @param build_log if not NULL, the build log will be written here, make sure it's BOTH large enough and null-terminated otherwise it will cause undefined behaviour and/or a crash
 * @param platform the platform you want to use (if -1 then uses the fastest device NOT platform)
 * @param device_number number of the device to run the kernel on. You can find the number of compatible devices using get_number_of_gpus (if -1 then it uses the fastest device)
 * @param queue_flags flags to pass to the clCreateCommandQueue function (OUT_OF_ORDER_EXEC and ENABLE_PROFILING are already defined for you)
*/
struct OpenCLFunctionWrapper* createOpenCLWrapperStruct(char* arg1, int type, char* kernel_name, char* build_log, int platform, int device_number,
            int queue_flags)
{
    cl_int err;
    int no_plat = get_number_of_platforms();
    if (platform >= no_plat || platform < 0)
    {
        platform = -1;
    }
    else
    {
        if (device_number < 0 || device_number >= get_platform_no_devices(platform))
        {
            device_number = -1;
        }
    }
    if (platform == -1 || device_number == -1)
    {
        CLDevicePair temp = get_fastest_parallel_device_full();
		if (temp.platform == -1 || temp.device == -1)
		{
			last_error = NO_OPENCL;
			return NULL;
		}
        platform = temp.platform;
        device_number = temp.device;
    }
    OpenCLFunctionWrapper* wrapper;

    wrapper = (OpenCLFunctionWrapper*)malloc(sizeof(OpenCLFunctionWrapper));
    if (!wrapper) {
        seterr(BAD_WRAPPER_MALLOC, -1);
        return NULL;
    }

    /* Setting dimension to 0 for safety, incase callee forgets to give it an actual value */
    wrapper->dim = 0;
    char* kernel_source;

    if (type == KERNEL_FROM_STRING){
        kernel_source = arg1;
    }
    else if (type == KERNEL_FROM_FILE)
    {
        FILE *fp;
        char *source_str;
        size_t source_size;
        fp = fopen(arg1, "r");
        /* Could not open file */
        if (fp == NULL)
		{
			seterr(BAD_FILE_PATH, -1);
        	evalnull(fp);
		}

        source_str = (char*)malloc(MAX_SOURCE_SIZE);
        evalnull(source_str);

        source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
        fclose(fp);
        if (source_size == 0)
        {
            seterr(BAD_FILE_PATH, -1);
            free(source_str);
            goto ABORT;
        }

        kernel_source = (char*)malloc(source_size + 1);
        /* Malloc failed*/
        if (kernel_source == NULL)
        {
            seterr(BAD_MALLOC, -1);
            free(source_str);
            goto ABORT;
        }

        memcpy(kernel_source, source_str, source_size);
        kernel_source[source_size] = '\0'; /* Just to keep it safe */
        free(source_str);
    }

    cl_platform_id *platforms = (cl_platform_id *)malloc(sizeof(cl_platform_id) * no_plat);
    if (platforms == NULL) {
        seterr(BAD_MALLOC, -1);
        goto ABORT;
    }

    err = clGetPlatformIDs(no_plat, platforms, NULL);
    if (err != CL_SUCCESS) {
        seterr(BAD_PLATFORMID, err);
        goto FREE_PLATFORMS_ABORT;
    }

    if (platform >= no_plat) {
        seterr(BAD_PLATFORM_NUM, -1);
        goto FREE_PLATFORMS_ABORT;
    }

    cl_platform_id selected_platform = platforms[platform];

    cl_uint num_devices;
    err = clGetDeviceIDs(selected_platform, CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
    if (err != CL_SUCCESS) {
        seterr(BAD_DEVICEID, err);
        goto FREE_PLATFORMS_ABORT;
    }

    if (device_number >= num_devices) {
        seterr(BAD_DEVICE_NUM, -1);
        goto FREE_PLATFORMS_ABORT;
    }

    cl_device_id *devices = (cl_device_id *)malloc(sizeof(cl_device_id) * num_devices);
    if (devices == NULL) {
        seterr(BAD_MALLOC, -1);
        goto FREE_PLATFORMS_ABORT;
    }

    err = clGetDeviceIDs(selected_platform, CL_DEVICE_TYPE_GPU, num_devices, devices, NULL);
    if (err != CL_SUCCESS) {
        seterr(BAD_DEVICEID, err);
        goto FREE_DEVICES_ABORT;
    }

    wrapper->device = devices[device_number];
    wrapper->platform = selected_platform;

    wrapper->context = clCreateContext(NULL, 1, &wrapper->device, NULL, NULL, &err);
    if (err != CL_SUCCESS) {
        seterr(BAD_CONTEXT, err);
        /* Handle error: Unable to create context */
        goto ABORT;
    }

    wrapper->queue = clCreateCommandQueue(wrapper->context, wrapper->device, queue_flags, &err);
    if (err != CL_SUCCESS) {
        seterr(BAD_COMMANDQUEUE, err);
        /* Handle error: Unable to create command queue */
        clReleaseContext(wrapper->context);
        goto ABORT;
    }

    wrapper->program = clCreateProgramWithSource(wrapper->context, 1, (const char**)&kernel_source, NULL, &err);
    if (err != CL_SUCCESS) {
        seterr(BAD_PROGRAM, err);
        /* Handle error: Unable to create program */
        clReleaseCommandQueue(wrapper->queue);
        clReleaseContext(wrapper->context);
        goto ABORT;
    }
    //printf("Program create ok\n");

    err = clBuildProgram(wrapper->program, 1, &wrapper->device, NULL, NULL, NULL);

    /* OPTIONAL: Getting the build log */
    if (build_log != NULL)
    {
        size_t log_size;
        clGetProgramBuildInfo(wrapper->program, wrapper->device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        /* Safety check */
        if (strlen(build_log) >= log_size)
        {
	        clGetProgramBuildInfo(wrapper->program, wrapper->device, CL_PROGRAM_BUILD_LOG, log_size, build_log, NULL);
        }
    }
    //printf("build log ok\n");

    if (err != CL_SUCCESS) {
        seterr(BAD_BUILD, err);
        /* Handle error: Unable to build program */
        clReleaseProgram(wrapper->program);
        clReleaseCommandQueue(wrapper->queue);
        clReleaseContext(wrapper->context);
        goto ABORT;
    }

    wrapper->kernel = clCreateKernel(wrapper->program, kernel_name, &err);
    if (err != CL_SUCCESS) {
        seterr(BAD_KERNEL, err);
        /* Handle error: Unable to create kernel */
        clReleaseProgram(wrapper->program);
        clReleaseCommandQueue(wrapper->queue);
        clReleaseContext(wrapper->context);
        goto ABORT;
    }

    /* Freeing additional resources */
    if (type == KERNEL_FROM_FILE)
    {
        free(kernel_source);
    }
	/*printf("NOrmal flow\n"); */
	wrapper -> local_size_set = -1;
    return wrapper;

FREE_DEVICES_ABORT:
    free(devices);

FREE_PLATFORMS_ABORT:
    free(platforms);
ABORT:
    free(wrapper);
	printf("ABORTED....\n");
    return NULL;

}

struct OpenCLFunctionWrapper* createSharedContextOpenCLWrapper (struct OpenCLFunctionWrapper *original, const char *arg1, int type, char *kernel_name, char *build_log, int queue_flags)
{
    if (!original)
    {
        seterr(BAD_WRAPPER_REFERENCE, -1);
        return NULL;
    }

    OpenCLFunctionWrapper *wrapper;
    cl_int err;

    wrapper = (OpenCLFunctionWrapper*)malloc(sizeof(OpenCLFunctionWrapper));
    if (!wrapper)
    {
        seterr(BAD_WRAPPER_MALLOC, -1);
        return NULL;
    }

    wrapper->context  = original->context;
    wrapper->device   = original->device;
    wrapper->platform = original->platform;

    char *kernel_source;

    if (type == KERNEL_FROM_STRING)
    {
        kernel_source = (char*)arg1;
    }
    else if (type == KERNEL_FROM_FILE)
    {
        FILE *fp = fopen(arg1, "r");
        if (!fp)
        {
            seterr(BAD_FILE_PATH, -1);
            free(wrapper);
            return NULL;
        }
        char *source_str = (char*)malloc(MAX_SOURCE_SIZE);
        size_t source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
        fclose(fp);

        if (source_size == 0)
        {
            free(source_str);
            free(wrapper);
            seterr(BAD_FILE_PATH, -1);
            return NULL;
        }

        kernel_source = (char*)malloc(source_size + 1);
        if (!kernel_source)
        {
            free(source_str);
            free(wrapper);
            seterr(BAD_MALLOC, -1);
            return NULL;
        }

        memcpy(kernel_source, source_str, source_size);
        kernel_source[source_size] = '\0'; /* Null terminate the string */
        free(source_str);
    }

    wrapper->queue = clCreateCommandQueue(wrapper->context, wrapper->device, queue_flags, &err);
    if (err != CL_SUCCESS)
    {
        seterr(BAD_COMMANDQUEUE, err);
        free(wrapper);
        if (type == KERNEL_FROM_FILE)
        {
            free(kernel_source);
        }
        return NULL;
    }

    wrapper->program = clCreateProgramWithSource(wrapper->context, 1, (const char**)&kernel_source, NULL, &err);
    if (err != CL_SUCCESS)
    {
        clReleaseCommandQueue(wrapper->queue);
        seterr(BAD_PROGRAM, err);
        free(wrapper);
        if (type == KERNEL_FROM_FILE)
        {
            free(kernel_source);
        }
        return NULL;
    }

    err = clBuildProgram(wrapper->program, 1, &wrapper->device, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        clReleaseProgram(wrapper->program);
        clReleaseCommandQueue(wrapper->queue);
        seterr(BAD_BUILD, err);
        free(wrapper);
        if (type == KERNEL_FROM_FILE)
        {
            free(kernel_source);
        }
        return NULL;
    }

    if (build_log != NULL)
    {
        size_t log_size;
        clGetProgramBuildInfo(wrapper->program, wrapper->device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        if (strlen(build_log) > log_size)
        {
            clGetProgramBuildInfo(wrapper->program, wrapper->device, CL_PROGRAM_BUILD_LOG, log_size, build_log, NULL);
        }
        build_log[log_size] = '\0';
    }

    wrapper->kernel = clCreateKernel(wrapper->program, kernel_name, &err);
    if (err != CL_SUCCESS)
    {
        clReleaseProgram(wrapper->program);
        clReleaseCommandQueue(wrapper->queue);
        seterr(BAD_KERNEL, err);
        free(wrapper);
        if (type == KERNEL_FROM_FILE)
        {
            free(kernel_source);
        }
        return NULL;
    }

    if (type == KERNEL_FROM_FILE)
    {
        free(kernel_source);
    }

	wrapper -> local_size_set = -1;
    return wrapper; /*success*/
}

/**
 * @brief sets the number of data buffers that are going to be used. It does NOT create any data buffers.
 *
 * @param wrapper pointer to the OpenCLFunctionWrapper struct
 * @param no_buf the number of buffers
 * @return nothing, but nobuf will be set to -1 on failure of the malloc call
*/
void set_no_buffers(struct OpenCLFunctionWrapper* wrapper, int no_buf)
{
    evalnull(wrapper);
    wrapper->buffers = calloc(no_buf, sizeof(cl_mem));
    evalnull(wrapper->buffers);
    wrapper->original_data = (void**)malloc(no_buf * sizeof(void*));
    evalnull(wrapper->original_data);
    wrapper->sizes = (uint64_t*)malloc(no_buf * sizeof(uint64_t));
    evalnull(wrapper->sizes);
    wrapper->datatypes = (uint32_t*)malloc(no_buf * sizeof(uint32_t));
    evalnull(wrapper->datatypes);
    wrapper->argind = (int*)malloc(no_buf * sizeof(int));
    evalnull(wrapper->argind);
    wrapper->nobuf = no_buf;
    return;
ABORT:
    seterr(BAD_MALLOC, -1);
    wrapper->nobuf = -1;
    return;
}

/**
 * @brief sets an argument for the function kernel
 *
 * @param wrapper pointer to the OpenCLFunctionWrapper struct
 * @param argIndex the index of the argument to be set
 * @param argsize the size of the datatype for the argument
 * @param ... the param, use a SINGLE variable which is either a data primitive or a pointer, structs passed by value should be set manually
 *
 * @return 0 on success, -1 on failure
*/
int set_kernel_arg(struct OpenCLFunctionWrapper* wrapper, int argIndex, size_t argSize, ...)
{
    cl_int err;
    if (wrapper == NULL)
    {
        seterr(NULL_PTR_ERROR, -1);
           return -1;
    }

    va_list args;
    va_start(args, argSize);

    if (argSize == sizeof(int)) {
        int intValue = va_arg(args, int);
        err = clSetKernelArg(wrapper->kernel, argIndex, argSize, (void *)&intValue);
    } else if (argSize == sizeof(float)) {
        float floatValue = va_arg(args, double); // Note the cast to double
        err = clSetKernelArg(wrapper->kernel, argIndex, argSize, (void *)&floatValue);
    } else if (argSize == sizeof(double)) {
        double doubleValue = va_arg(args, double);
        err = clSetKernelArg(wrapper->kernel, argIndex, argSize, (void *)&doubleValue);
    } else if (argSize == sizeof(char)) {
        char charValue = va_arg(args, int); // Note the cast to int
        err = clSetKernelArg(wrapper->kernel, argIndex, argSize, (void *)&charValue);
    } else if (argSize == sizeof(short)) {
        short shortValue = va_arg(args, int); // Note the cast to int
        err = clSetKernelArg(wrapper->kernel, argIndex, argSize, (void *)&shortValue);
    } else if (argSize == sizeof(long)) {
        long longValue = va_arg(args, long);
        err = clSetKernelArg(wrapper->kernel, argIndex, argSize, (void *)&longValue);
    } else if (argSize == sizeof(long long)) {
        long long longLongValue = va_arg(args, long long);
        err = clSetKernelArg(wrapper->kernel, argIndex, argSize, (void *)&longLongValue);
    } else if (argSize == sizeof(unsigned int)) {
        unsigned int uintValue = va_arg(args, unsigned int);
        err = clSetKernelArg(wrapper->kernel, argIndex, argSize, (void *)&uintValue);
    } else if (argSize == sizeof(unsigned char)) {
        unsigned char ucharValue = va_arg(args, unsigned int); // Note the cast to unsigned int
        err = clSetKernelArg(wrapper->kernel, argIndex, argSize, (void *)&ucharValue);
    } else if (argSize == sizeof(unsigned long long)) {
        unsigned long long ullValue = va_arg(args, unsigned long long);
        err = clSetKernelArg(wrapper->kernel, argIndex, argSize, (void *)&ullValue);
    } else if (argSize == sizeof(void*)) {
        void *ptrval = va_arg(args, void*);
        err = clSetKernelArg(wrapper->kernel, argIndex, argSize, ptrval);
    }

    va_end(args);

    if (err != CL_SUCCESS)
    {
        seterr(BAD_KERNEL_ARG, err);
        return -1;
    }
    return 0;
}


/**
 * @brief creates a buffer and sets it as an argument for the function
 *
 * @param wrapper pointer to the OpenCLFunctionWrapper struct
 * @param bufsize the buffersize in bytes
 * @param argsize Size of datatype, should be sizeof(void*)
 * @param argIndex the index of the argument that will be set if -1 it is not an argument
 * @param mem_access_flags memory access flags (READONLY, WRITEONLY, READWRITE or the real opencl names CL_MEM_READ_ONLY, CL_MEM_WRITE_ONLY, CL_MEM_READ_WRITE)
 * @param write_flags tells whether the data is to be read from or written to main memory (FROM_MEMORY, TO_MEMORY or TO_AND_FROM_MEMORY) MUST BE SET
 *
 * @return position in the buffer array on succes -1 on failure
*/
int create_and_set_buf(struct OpenCLFunctionWrapper* wrapper, void* data_source, uint64_t bufsize,
                                 size_t argSize, int argIndex, int mem_access_flags, uint32_t write_flags)
{
    if (wrapper == NULL)
    {
        seterr(NULL_PTR_ERROR, -1);
        goto ABORT;
    }
    cl_int err;
    cl_mem tmp = clCreateBuffer(wrapper->context, mem_access_flags, bufsize, NULL, &err);
    /* Unlikely to happen but possible */
    if (tmp == NULL)
    {
        seterr(BAD_VRAM, err);
        goto ABORT;
    }
    int i = 0, ok = -1;
    for (i = 0; i < wrapper->nobuf; i +=1)
    {
        if (wrapper->buffers[i] == 0)
        {
            ok = i;
            wrapper->buffers[i] = tmp;
            wrapper->original_data[i] = data_source;
            wrapper->sizes[i] = bufsize;
            wrapper->datatypes[i] = write_flags;
            wrapper->argind[i] = argIndex;
            break;
        }
    }
    /* There is no more room for this buffer */
    if (ok == -1)
    {
        /* No point in checking status*/
        clReleaseMemObject(tmp);
        seterr(NO_PLACE_FOR_BUFF, -1);
        goto ABORT;
    }
    if (argIndex != -1)
        /* It finally makes sense to set it as an argument*/
        err = clSetKernelArg(wrapper->kernel, argIndex, argSize, (void *)&tmp);
    if (err != CL_SUCCESS)
    {
        /* No point in checking status*/
        clReleaseMemObject(tmp);
        seterr(BAD_KERNEL_ARG, err);
        goto ABORT;
    }
    return ok;
ABORT:
    return -1;
}

/**
 @brief finds the index of the cl_mem handle associated to ptr
 @param wrapper the OpenCLFunctionWrapper to search
 @param ptr the pointer to look for
 @return -1 on failure non-negative number on success
*/
int find_cl_mem_from_ptr(struct OpenCLFunctionWrapper *wrapper, void *ptr)
{
    int i = 0;
    for (; i < wrapper->nobuf; ++i)
    {
        if (wrapper->original_data[i] == ptr)
        {
            return i;
        }
    }
    return -1;
}

int copy_buf_from_other(struct OpenCLFunctionWrapper *source, struct OpenCLFunctionWrapper *wrapper, void* data_source,
                                size_t argSize, int argIndex, uint32_t write_flags)
{
    if (wrapper == NULL)
    {
        seterr(NULL_PTR_ERROR, -1);
        goto ABORT;
    }
    int buf_index = find_cl_mem_from_ptr(source, data_source);
    if (buf_index == -1)
    {
        seterr(INVALID_PTR, -1);
        goto ABORT;
    }
    int i = 0, ok = -1;
    for (i = 0; i < wrapper->nobuf; i +=1)
    {
        if (wrapper->buffers[i] == 0)
        {
            ok = i;
            wrapper->buffers[i] = source->buffers[buf_index];
            wrapper->original_data[i] = data_source;
            wrapper->sizes[i] = source->sizes[buf_index];
            wrapper->datatypes[i] = write_flags;
            wrapper->argind[i] = argIndex;
            break;
        }
    }
    /* There is no more room for this buffer */
    if (ok == -1)
    {
        /* No point in checking status*/
        seterr(NO_PLACE_FOR_BUFF, -1);
        goto ABORT;
    }
    /* It finally makes sense to set it as an argument*/
    cl_int err = clSetKernelArg(wrapper->kernel, argIndex, argSize, (void *)&wrapper->buffers[i]);
    if (err != CL_SUCCESS)
    {
        /* No point in checking status*/
        seterr(BAD_KERNEL_ARG, err);
        goto ABORT;
    }
    return ok;
ABORT:
    return -1;
}

int change_buf_flags(struct OpenCLFunctionWrapper *source, void *ptr, uint32_t write_flags)
{
    if (source == NULL)
    {
        seterr(NULL_PTR_ERROR, -1);
        return -1;
    }
    int buf_index = find_cl_mem_from_ptr(source, ptr);
    if (buf_index == -1)
    {
        seterr(INVALID_PTR, -1);
        return -1;
    }
    source->datatypes[buf_index] = write_flags;
    return 0;
}

/**
 * @brief Sets the global size of work items (var1 * max(1, var2) * max(1, var3) in general) needs to be called before call_function, or you will get an error for sure
 *
 * @param wrapper pointer to the OpenCLFunctionWrapper struct
 * @param dimension the number of dimensions for kernel execution (1, 2 or 3 numbers bigger than 3 are treated as 3, numbers smaller than 1 do nothing)
 * @param var1 the first size variable  (should be nonzero)
 * @param var2 the second size variable (should be set for dimension > 1)
 * @param var3 the third size variable  (should be set if dimension is 3)
*/
void set_dimension_and_values(struct OpenCLFunctionWrapper* wrapper, int dimension, int var1, int var2, int var3)
{
    if (wrapper == NULL)
    {
        seterr(NULL_PTR_ERROR, -1);
        return;
    }
    wrapper->dim = dimension;
    if (dimension >= 1)
        wrapper -> dims[0] = var1;
    if (dimension >= 2)
        wrapper -> dims[1] = var2;
    if (dimension >= 3)
        wrapper -> dims[2] = var3;
}

void set_local_size(struct OpenCLFunctionWrapper* wrapper, int size)
{
    wrapper->local_size_set = 1;
    wrapper->local_size = size;
}


int64_t call_function(struct OpenCLFunctionWrapper* wrapper, int flags)
{
    int ret = 0;
    cl_event kernel_event;
    if (wrapper == NULL)
    {
        seterr(NULL_PTR_ERROR, -1);
        goto ABORT;
    }
    cl_int err;
    int i;

    /* Copying the data to VRAM if needed */
    if ((flags & NO_READ) == 0)
    {
        for (i = 0; i < wrapper->nobuf; i+=1)
        {
            if ((wrapper->datatypes[i] & FROM_MEMORY) != 0)
            {
                err = clEnqueueWriteBuffer(wrapper->queue, wrapper->buffers[i], CL_TRUE, 0, wrapper->sizes[i], wrapper->original_data[i], 0, NULL, NULL);
                if (err != CL_SUCCESS)
                {
                    seterr(BAD_ENQUEUE_WRITE, err);
                    goto ABORT;
                }
            }
        }
    }

    /* Arguments should already be set at this point */
    if (wrapper->local_size_set == -1)
        err = clEnqueueNDRangeKernel(wrapper->queue, wrapper->kernel, wrapper->dim, NULL, wrapper->dims, NULL, 0, NULL, &kernel_event);
    else
        err = clEnqueueNDRangeKernel(wrapper->queue, wrapper->kernel, wrapper->dim, NULL, wrapper->dims, &wrapper->local_size, 0, NULL, &kernel_event);
    if (err != CL_SUCCESS)
    {
        seterr(BAD_ENQUEUE, err);
        goto ABORT;
    }
    clWaitForEvents(1, &kernel_event);

    if (flags & HIGH_PERF_CLOCK)
    {
        cl_ulong start, end;
        clGetEventProfilingInfo(kernel_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, NULL);
        clGetEventProfilingInfo(kernel_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
        ret = end - start;
    }

    /* Copying data from vram if needed */
    if ((flags & NO_WRITE) == 0)
    {
        for (i = 0; i < wrapper->nobuf; i+=1)
        {
            if ((wrapper->datatypes[i] & TO_MEMORY) != 0)
            {
                err = clEnqueueReadBuffer(wrapper->queue, wrapper->buffers[i], CL_TRUE, 0, wrapper->sizes[i], wrapper->original_data[i], 0, NULL, NULL);
                if (err != CL_SUCCESS)
                {
                    seterr(BAD_ENQUEUE_READ, err);
                    goto ABORT;
                }
            }
        }
    }
    return ret;
ABORT:
    return -1;
}

/**
 * @brief Changes the datatype field associated with the specified pointer in main memory
 *
 * @param wrapper pointer to the OpenCLFunctionWrapper struct
 * @param ptr the pointer in main memory
 * @param newFlag the new memory access flag (FROM_MEMORY, TO_MEMORY or TO_AND_FROM_MEMORY)
 *
 * @return 0 if ptr is found and the change is successful, -1 upon failure (ptr is not found)
*/
int change_datatype_field(struct OpenCLFunctionWrapper* wrapper, void* ptr, uint32_t newFlag)
{
    if (wrapper == NULL)
    {
        seterr(NULL_PTR_ERROR, -1);
        goto ABORT;
    }
    int i = 0;
    for (; i < wrapper->nobuf; i+=1)
    {
        if (wrapper->original_data[i] == ptr)
        {
            wrapper->datatypes[i] = newFlag;
            return 0;
        }
    }
ABORT:
    return -1;
}

/** @brief Updates the pointer to main memory of the VRAM buffer, assuming the VRAM buffer's size is unchanged
 *
 * @param wrapper Pointer to the OpenCLFunctionWrapper struct
 * @param new_ptr new pointer
 * @param new_size the new size of the pointer has to be the same or less than the old one (the VRAM buffer is unchanged)
 * @param ind the position in the buffer array
 *
 * @return 0 on succes -1 on failure use clW_get_last_error for more info
*/
int update_buffer_pointer(struct OpenCLFunctionWrapper* wrapper, void* new_ptr, uint64_t new_size, int ind)
{
    if (wrapper == NULL)
    {
        seterr(NULL_PTR_ERROR, -1);
        goto ABORT;
    }
    if (ind >= wrapper->nobuf)
    {
        seterr(BAD_IND, -1);
        goto ABORT;
    }
    evalnull(wrapper->buffers[ind])
    wrapper->sizes[ind] = new_size;
    wrapper->original_data[ind] = new_ptr;
    return 0;
ABORT:
    return -1;
}

/** @brief Releases the cl_mem object found at position ind, without updating the kernel arguments
 *
 * @param wrapper Pointer to the OpenCLFunctionWrapper struct
 * @param ind the position in the buffers' array
 *
 * @return 0 on succes, -1 on failure, use clW_get_last_error for more info
*/
int release_buf(struct OpenCLFunctionWrapper* wrapper, int ind)
{
    if (wrapper == NULL)
    {
        seterr(NULL_PTR_ERROR, -1);
        goto ABORT;
    }
    if (ind >= wrapper->nobuf)
    {
        seterr(BAD_IND, -1);
        goto ABORT;
    }
    cl_int err = clReleaseMemObject(wrapper->buffers[ind]);
    if (err != CL_SUCCESS)
    {
        seterr(INVALID_BUF, -1);
        goto ABORT;
    }
    wrapper->datatypes[ind] = 0;
    wrapper->original_data[ind] = NULL;
    wrapper->sizes[ind] = 0;
    return 0;
ABORT:
    return -1;
}

/**
 * @brief updates an existing buffer and sets it as an argument for the function
 *
 * @param wrapper pointer to the OpenCLFunctionWrapper struct
 * @param new_ptr the new pointer in the main memory
 * @param new_size new size of the memory at new_ptr
 * @param ind position in buffers array
 * @param mem_access_flags memory access flags (READONLY, WRITEONLY, READWRITE or the real opencl names CL_MEM_READ_ONLY, CL_MEM_WRITE_ONLY, CL_MEM_READ_WRITE)
 * @param write_flags tells whether the data is to be read from or written to main memory (FROM_MEMORY, TO_MEMORY or TO_AND_FROM_MEMORY) MUST BE SET
 *
 * @return 0 on succes -1 on failure
*/
int change_vram_buf(struct OpenCLFunctionWrapper* wrapper, void* new_ptr, uint64_t new_size, int ind, int mem_access_flags, int write_flags)
{
    if (wrapper == NULL)
    {
        seterr(NULL_PTR_ERROR, -1);
        goto ABORT;
    }
    if (ind >= wrapper->nobuf)
    {
        seterr(BAD_IND, -1);
        goto ABORT;
    }
    int ret = release_buf(wrapper, ind);
    if (ret == -1)
    {
        /* The error is already set */
        goto ABORT;
    }
    cl_int err;
    cl_mem tmp = clCreateBuffer(wrapper->context, mem_access_flags, new_size, NULL, &err);
    /* Unlikely to happen but possible */
    if (tmp == NULL)
    {
        seterr(BAD_VRAM, err);
        goto ABORT;
    }
    wrapper->buffers[ind] = tmp;
    wrapper->original_data[ind] = new_ptr;
    wrapper->sizes[ind] = new_size;
    wrapper->datatypes[ind] = write_flags;
    err = clSetKernelArg(wrapper->kernel, wrapper->argind[ind], sizeof(void*), (void *)&tmp);
    if (err != CL_SUCCESS)
    {
        /* No point in checking status*/
        clReleaseMemObject(tmp);
        seterr(BAD_KERNEL_ARG, err);
        goto ABORT;
    }
    return 0;
ABORT:
    return -1;
}

/**
 * @brief Cleans up and deallocates the OpenCLFunctionWrapper struct and its associated resources.
 *
 * @param wrapper The OpenCLFunctionWrapper struct to be cleaned up.
 *
 * @return 0 on succes, -1 on failure, call clW_get_last_error for more info
 */
int cleanupOpenCLWrapper(OpenCLFunctionWrapper* wrapper) {
    if (wrapper == NULL) {
        last_error = NULL_PTR_ERROR;
        return -1;
    }

    // Release OpenCL resources
    if (wrapper->kernel != NULL) {
        clReleaseKernel(wrapper->kernel);
    }
    if (wrapper->program != NULL) {
        clReleaseProgram(wrapper->program);
    }
    int i =0;
    for (; i < wrapper->nobuf; ++i)
    {
        if (release_buf(wrapper, i) == -1)
            return -1;
    }
    if (wrapper->queue != NULL) {
        clReleaseCommandQueue(wrapper->queue);
    }
    if (wrapper->context != NULL) {
        clReleaseContext(wrapper->context);
    }

    // Free the struct itself
    free(wrapper);
    return 0;
}


/**
 * @brief gets the number of gpus
 *
 * @return the number of gpu-like devices found by opencl, -1 on error or if there are no gpu-like devices
*/
int get_number_of_gpus(void)
{
    cl_platform_id *platforms;
    cl_uint num_platforms;
    cl_uint total_gpus = 0;
    cl_int err;

    // Get number of platforms
    err = clGetPlatformIDs(0, NULL, &num_platforms);
    if (err != CL_SUCCESS) {
        /* Handle error: Unable to get number of platforms */
        seterr(BAD_PLATFORMID, err);
        goto ABORT;
    }

    // Allocate memory for platforms
    platforms = (cl_platform_id *)malloc(num_platforms * sizeof(cl_platform_id));
    if(platforms == NULL) {
        /* Handle error: Unable to allocate memory for platforms */
        seterr(BAD_MALLOC, err);
        goto ABORT;
    }

    // Get platforms
    err = clGetPlatformIDs(num_platforms, platforms, NULL);
    if (err != CL_SUCCESS) {
        /* Handle error: Unable to get platforms */
        seterr(BAD_PLATFORMID, err);
        goto FREE_MEM_ABORT;
    }

    // Loop over platforms and count GPU devices
    for (cl_uint i = 0; i < num_platforms; i++) {
        cl_uint num_gpus = 0;

        err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, NULL, &num_gpus);
        if (err != CL_SUCCESS && err != CL_DEVICE_NOT_FOUND) {
            /* Handle error: Unable to get number of GPU devices */
            seterr(BAD_DEVICEID, err);
            goto FREE_MEM_ABORT;
        }

        total_gpus += num_gpus;
    }

    free(platforms);
    return total_gpus;

FREE_MEM_ABORT:
    free(platforms);
ABORT:
    return -1;
}

/**
 * @brief gets the number of platforms
 *
 * @return the number of gpu-like devices found by opencl, -1 on error or if there are no gpu-like devices
*/
int get_number_of_platforms(void)
{
    cl_uint num_platforms;
    cl_int err;

    // Get number of platforms
    err = clGetPlatformIDs(0, NULL, &num_platforms);
    if (err != CL_SUCCESS) {
        /* Handle error: Unable to get number of platforms */
        seterr(BAD_PLATFORMID, err);
        goto ABORT;
    }
    return (int)num_platforms;
ABORT:
    return -1;
}

/**
 * @brief Get the number of devices on a given platform
 *
 * @param platform_num the platform you want to query
 *
 * @return the number of devices on the specified platform -1 if something went wrong. call clW_get_last_error()
 */
int get_platform_no_devices(int platform_num)
{
    cl_int err;
    int no_plat = get_number_of_platforms();
    if (platform_num < 0 || platform_num >= no_plat) /* Zero-based indexing */
    {
        last_error = BAD_PLATFORM_NUM;
        goto ABORT;
    }

    cl_platform_id *platforms = malloc(sizeof(cl_platform_id) * no_plat);
    if (platforms == NULL) {
        seterr(BAD_MALLOC, -1); /* err is not set yet */
        goto ABORT;
    }

    // Get the list of platform IDs
    err = clGetPlatformIDs(no_plat, platforms, NULL);
    if (err != CL_SUCCESS) {
        seterr(BAD_PLATFORMID, err);
        goto FREE_MEM_ABORT;
    }

    cl_uint num_gpus = 0;
    err = clGetDeviceIDs(platforms[platform_num], CL_DEVICE_TYPE_GPU, 0, NULL, &num_gpus);
    if (err != CL_SUCCESS && err != CL_DEVICE_NOT_FOUND) {
        seterr(BAD_DEVICEID, err);
        goto FREE_MEM_ABORT;
    }

    free(platforms); // Clean up the allocated memory
    return (int)num_gpus;

FREE_MEM_ABORT:
    free(platforms);
ABORT:
    return -1;
}
