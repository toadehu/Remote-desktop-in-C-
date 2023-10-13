#ifndef _OPENCL_WRAPP_CRAZY /*Now this shouldn't conflict with anything */
#define _OPENCL_WRAPP_CRAZY 1

#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>

#define MAX_SOURCE_SIZE (0x100000)

int last_error = -1;
cl_int last_err = -1;

#define BAD_WRAPPER_MALLOC 1
#define BAD_FILE_PATH 2
#define BAD_MALLOC 3
#define BAD_PLATFORMID 4
#define BAD_DEVICEID 5
#define BAD_CONTEXT 6
#define BAD_COMMANDQUEUE 7
#define BAD_PROGRAM 8
#define BAD_BUILD 9
#define BAD_KERNEL 10
#define BAD_KERNEL_ARG 11
#define NO_PLACE_FOR_BUFF 12
#define NULL_PTR_ERROR 13 /* This error should honestly cause an exit */
#define BAD_ENQUEUE_WRITE 14
#define BAD_ENQUEUE 15
#define BAD_ENQUEUE_READ 16
#define BAD_VRAM 17
#define BAD_IND 18
#define BAD_POINTER 19
#define INVALID_BUF 20

#define seterr(code, clerr) do{ \
last_error = code; \
 last_err = clerr; \
}while(0);

void clW_get_last_error(void)
{
    switch (last_error)
    {
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
        case BAD_VRAM: printf("Couldn't allocate VRAM, opencl error code: %d\n", (int)last_err); break;
        case BAD_IND: printf("Inidice outside of array encountered when trying to update the buffer \n"); break;
        case BAD_POINTER: printf("Tried to access null memory or default error caused by malloc returning NULL\n"); break;
        case INVALID_BUF: printf("Encountered an error freeing a cl_mem object, opencl error code: %d\n", (int)last_err);
        default: printf("Unkown error: %d (-1 means no error), opencl error code: %d\n", last_error, (int)last_err); break;
    }
}

#define FROM_MEMORY 4
#define TO_MEMORY   8
#define TO_AND_FROM_MEMORY TO_MEMORY | FROM_MEMORY

typedef struct OpenCLFunctionWrapper{
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_mem *buffers;
    void **original_data;
    uint64_t *sizes;
    uint32_t *datatypes;        /* Is the data to be copied from main memory or is it to be written to main memory */
    int* argind;
    int nobuf;
    size_t dims[3];
    int dim;
} OpenCLFunctionWrapper;

#define evalnull(ptr) if (ptr == NULL) \
{ \
    seterr(BAD_POINTER, -1);\
    goto ABORT;\
} 

#define KERNEL_FROM_STRING 1
#define KERNEL_FROM_FILE 2

/**
 * @brief creates the OpenCLWrapper struct, and optionally writes the build log to the build_log variable. no data is loaded
 * 
 * @param arg1 the data argument, either a filename to load the code from or the kernel code itself
 * @param type what is arg1, is the kernel loaded from the string (KERNEL_FROM_STRING) or loaded from a file (KERNEL_FROM_FILE)
 * @param kernel_name the name of the kernel, it must be found within the program (look up clCreateKernel for more info)
 * @param build_log if not NULL, the build log will be written here, make sure it's BOTH large enough and null-terminated otherwise it will cause undefined behaviour and/or a crash
*/
struct OpenCLFunctionWrapper* createOpenCLWrapperStruct(char* arg1, int type, char* kernel_name, char* build_log)
{
    OpenCLFunctionWrapper* wrapper;
    cl_int err;

    wrapper = (OpenCLFunctionWrapper*)malloc(sizeof(OpenCLFunctionWrapper));
    if (!wrapper) {
        seterr(BAD_WRAPPER_MALLOC, -1);
        return NULL;
    }

    /* Setting dimension to 0 for safety, incase calle forgets to give it an actual value */
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
        seterr(BAD_FILE_PATH, -1);
        evalnull(fp);

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

    err = clGetPlatformIDs(1, &wrapper->platform, NULL);
    if (err != CL_SUCCESS) {
        seterr(BAD_PLATFORMID, err);
        /* Handle error: Unable to get platform */
        goto ABORT;
    }

    err = clGetDeviceIDs(wrapper->platform, CL_DEVICE_TYPE_GPU, 1, &wrapper->device, NULL);
    if (err != CL_SUCCESS) {
        seterr(BAD_DEVICEID, err);
        /* Handle error: Unable to get device */
        goto ABORT;
    }

    wrapper->context = clCreateContext(NULL, 1, &wrapper->device, NULL, NULL, &err);
    if (err != CL_SUCCESS) {
        seterr(BAD_CONTEXT, err);
        /* Handle error: Unable to create context */
        goto ABORT;
    }

    wrapper->queue = clCreateCommandQueue(wrapper->context, wrapper->device, 0, &err);
    if (err != CL_SUCCESS) {
        seterr(BAD_COMMANDQUEUE, err);
        /* Handle error: Unable to create command queue */
        clReleaseContext(wrapper->context);
        goto ABORT;
    }

    wrapper->program = clCreateProgramWithSource(wrapper->context, 1, &kernel_source, NULL, &err);
    if (err != CL_SUCCESS) {
        seterr(BAD_PROGRAM, err);
        /* Handle error: Unable to create program */
        clReleaseCommandQueue(wrapper->queue);
        clReleaseContext(wrapper->context);
        goto ABORT;
    }

    err = clBuildProgram(wrapper->program, 1, &wrapper->device, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        seterr(BAD_BUILD, err);
        /* Handle error: Unable to build program */
        clReleaseProgram(wrapper->program);
        clReleaseCommandQueue(wrapper->queue);
        clReleaseContext(wrapper->context);
        goto ABORT;
    }

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

    return wrapper;

ABORT:
    free(wrapper);
    return NULL;

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
    if (wrapper == NULL)
    {
        seterr(NULL_PTR_ERROR, -1); 
           return -1;
    }
    
    va_list args;
    va_start(args, argSize);
    cl_int err;

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

#define READONLY CL_MEM_READ_ONLY
#define WRITEONLY CL_MEM_WRITE_ONLY
#define READWRITE CL_MEM_READ_WRITE

/**
 * @brief creates a buffer and sets it as an argument for the function
 * 
 * @param wrapper pointer to the OpenCLFunctionWrapper struct
 * @param bufsize the buffersize in bytes
 * @param argsize Size of datatype, should be sizeof(void*)
 * @param argIndex the index of the argument that will be set
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

/* No data that is specified to be read from main memory will be read,
  if you want to have finer control set manually the datatypes field individually,
  either manually or using the change_datatype_field method (recommended)*/
#define NO_READ 1
#define NO_WRITE 2 /* No data that is specified to be written back to main memory, will be written to main memory */

/**
 * @brief executes an iteration of the function stored in this instance
 * 
 * @param wrapper pointer to the OpenCLFunctionWrapper struct
 * @param flags optional flags (NO_WRITE if the data doesn't need to be written again to VRAM, similarly there is a NO_WRITE)
 * @return 0 on succes -1 on failure
*/
int call_function(struct OpenCLFunctionWrapper* wrapper, int flags)
{
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
    err = clEnqueueNDRangeKernel(wrapper->queue, wrapper->kernel, wrapper->dim, NULL, wrapper->dims, NULL, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        seterr(BAD_ENQUEUE, err);
        goto ABORT;
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
    return 0;
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
 * @return 0 on succes -1 on failure
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

#endif
