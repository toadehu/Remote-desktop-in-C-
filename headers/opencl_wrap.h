#pragma once

#ifndef _OPENCL_WRAPP_CRAZY /*Now this shouldn't conflict with anything */
#define _OPENCL_WRAPP_CRAZY 1

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

#ifdef __cplusplus
extern "C"{
#endif

#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    int platform, device;
}CLDevicePair;

/**
 * @brief Get the fastest parallel device including platform and device number;
 *
 * @return CLDevicePair with the platform and device to use when calling createOpenCLWrapperStruct, otherwise NULL on error, call clW_get_last_error().
 */
CLDevicePair get_fastest_parallel_device_full(void);

#define MAX_SOURCE_SIZE (1024 * 1024 * 30)

static int last_error = 0;
static cl_int last_err = 0;

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
#define BAD_PLATFORM_NUM 21
#define BAD_DEVICE_NUM 22
#define NO_OPENCL 23
#define BAD_WRAPPER_REFERENCE 24
#define INVALID_PTR 25

#define seterr(code, __clerr) do{ \
last_error = code; \
 last_err = __clerr; \
}while(0);

void clW_get_last_error(void);

const char *clGetErrorString(cl_int error);

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
    size_t local_size;
    int local_size_set;
} OpenCLFunctionWrapper;

#define evalnull(ptr) if (ptr == NULL) \
{ \
    seterr(BAD_POINTER, -1);\
    goto ABORT;\
}

int get_number_of_platforms(void);
int get_platform_no_devices(int platform_num);


#define KERNEL_FROM_STRING 1
#define KERNEL_FROM_FILE 2

#define OUT_OF_ORDER_EXEC  CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE
#define ENABLE_PROFILING CL_QUEUE_PROFILING_ENABLE

/**
 * @brief creates the OpenCLWrapper struct, and optionally writes the build log to the build_log variable. no data is loaded
 *
 * @param arg1 the data argument, either a filename to load the code from or the kernel code itself
 * @param type what is arg1, is the kernel loaded from the string (KERNEL_FROM_STRING) or loaded from a file (KERNEL_FROM_FILE)
 * @param kernel_name the name of the kernel, it must be found within the program (look up clCreateKernel for more info)
 * @param build_log if not NULL, the build log will be written here, make sure it's BOTH large enough and null-terminated otherwise it will cause undefined behaviour and/or a crash
 * @param platform the platform you want to use (if -1 then uses the fastest device NOT platform)
 * @param device_number number of the device to run the kernel on. You can find the number of compatible devices using get_number_of_gpus (if -1 then it uses the fastest device)
*/
struct OpenCLFunctionWrapper* createOpenCLWrapperStruct(char *arg1, int type, char *kernel_name, char *build_log, int platform, int device_number,
            int queue_flags);

struct OpenCLFunctionWrapper* createSharedContextOpenCLWrapper(struct OpenCLFunctionWrapper *original, const char *arg1, int type, char *kernel_name, char *build_log, int queue_flags);

/**
 * @brief sets the number of data buffers that are going to be used. It does NOT create any data buffers.
 *
 * @param wrapper pointer to the OpenCLFunctionWrapper struct
 * @param no_buf the number of buffers
 * @return nothing, but nobuf will be set to -1 on failure of the malloc call
*/
void set_no_buffers(struct OpenCLFunctionWrapper* wrapper, int no_buf);
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
int set_kernel_arg(struct OpenCLFunctionWrapper* wrapper, int argIndex, size_t argSize, ...);

#define READONLY CL_MEM_READ_ONLY
#define WRITEONLY CL_MEM_WRITE_ONLY
#define READWRITE CL_MEM_READ_WRITE

/**
 * @brief creates a buffer and sets it as an argument for the function
 *
 * @param wrapper pointer to the OpenCLFunctionWrapper struct
 * @param bufsize the buffersize in bytes
 * @param argsize Size of datatype, should be sizeof(void*)
 * @param argIndex the index of the argument that will be set -1 means it is not an argument
 * @param mem_access_flags memory access flags (READONLY, WRITEONLY, READWRITE or the real opencl names CL_MEM_READ_ONLY, CL_MEM_WRITE_ONLY, CL_MEM_READ_WRITE)
 * @param write_flags tells whether the data is to be read from or written to main memory (FROM_MEMORY, TO_MEMORY or TO_AND_FROM_MEMORY) MUST BE SET
 *
 * @return position in the buffer array on succes -1 on failure
*/
int create_and_set_buf(struct OpenCLFunctionWrapper *wrapper, void* data_source, uint64_t bufsize,
                                 size_t argSize, int argIndex, int mem_access_flags, uint32_t write_flags);

int copy_buf_from_other(struct OpenCLFunctionWrapper *source, struct OpenCLFunctionWrapper* wrapper,
                                void* data_source, size_t argSize, int argIndex, uint32_t write_flags);
/**
 * @brief updates a buffer and its arguments for the function
 *
 * @param source pointer to the OpenCLFunctionWrapper struct
 * @param ptr the pointer to the memory location in host memory
 * @param write_flags tells whether the data is to be read from or written to main memory (FROM_MEMORY, TO_MEMORY or TO_AND_FROM_MEMORY) MUST BE SET
 *
 * @return 0 on succes -1 on failure, failure only possible for invalid pointer
*/
int change_buf_flags(struct OpenCLFunctionWrapper *source, void* ptr, uint32_t new_flags);

/**
 * @brief Sets the global size of work items (var1 * max(1, var2) * max(1, var3) in general) needs to be called before call_function, or you will get an error for sure
 *
 * @param wrapper pointer to the OpenCLFunctionWrapper struct
 * @param dimension the number of dimensions for kernel execution (1, 2 or 3 numbers bigger than 3 are treated as 3, numbers smaller than 1 do nothing)
 * @param var1 the first size variable  (should be nonzero)
 * @param var2 the second size variable (should be set for dimension > 1)
 * @param var3 the third size variable  (should be set if dimension is 3)
*/
void set_dimension_and_values(struct OpenCLFunctionWrapper* wrapper, int dimension, int var1, int var2, int var3);

/**
 * @brief Sets the local work-item size (if needed, one can just not set it)
 *
 * @param wrapper pointer to the OpenCLFunctionWrapper struct
 * @param size the size of the local work-items
 *
*/
void set_local_size(struct OpenCLFunctionWrapper* wrapper, int size);


/* No data that is specified to be read from main memory will be read,
  if you want to have finer control set manually the datatypes field individually,
  either manually or using the change_datatype_field method (recommended)*/
#define NO_READ 1
#define NO_WRITE 2 /* No data that is specified to be written back to main memory, will be written to main memory */

#define HIGH_PERF_CLOCK 4 /* call_function() now returns the exection time of the kernel if the kernel supports it */

/**
 * @brief executes an iteration of the function stored in this instance
 *
 * @param wrapper pointer to the OpenCLFunctionWrapper struct
 * @param flags optional flags (NO_WRITE if the data doesn't need to be written again to VRAM, similarly there is a NO_WRITE)
 * @return positive number on succes -1 on failure, if flags is set to HIGH_PERF_CLOCK return value is the execution time of the kernel
*/
int64_t call_function(struct OpenCLFunctionWrapper* wrapper, int flags);

/**
 * @brief Changes the datatype field associated with the specified pointer in main memory
 *
 * @param wrapper pointer to the OpenCLFunctionWrapper struct
 * @param ptr the pointer in main memory
 * @param newFlag the new memory access flag (FROM_MEMORY, TO_MEMORY or TO_AND_FROM_MEMORY)
 *
 * @return 0 if ptr is found and the change is successful, -1 upon failure (ptr is not found)
*/
int change_datatype_field(struct OpenCLFunctionWrapper* wrapper, void* ptr, uint32_t newFlag);

/** @brief Updates the pointer to main memory of the VRAM buffer, assuming the VRAM buffer's size is unchanged
 *
 * @param wrapper Pointer to the OpenCLFunctionWrapper struct
 * @param new_ptr new pointer
 * @param new_size the new size of the pointer has to be the same or less than the old one (the VRAM buffer is unchanged)
 * @param ind the position in the buffer array
 *
 * @return 0 on succes -1 on failure use clW_get_last_error for more info
*/
int update_buffer_pointer(struct OpenCLFunctionWrapper* wrapper, void* new_ptr, uint64_t new_size, int ind);

/** @brief Releases the cl_mem object found at position ind, without updating the kernel arguments
 *
 * @param wrapper Pointer to the OpenCLFunctionWrapper struct
 * @param ind the position in the buffers' array
 *
 * @return 0 on succes, -1 on failure, use clW_get_last_error for more info
*/
int release_buf(struct OpenCLFunctionWrapper* wrapper, int ind);

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
int change_vram_buf(struct OpenCLFunctionWrapper* wrapper, void* new_ptr, uint64_t new_size, int ind, int mem_access_flags, int write_flags);

/**
 * @brief Cleans up and deallocates the OpenCLFunctionWrapper struct and its associated resources.
 *
 * @param wrapper The OpenCLFunctionWrapper struct to be cleaned up.
 *
 * @return 0 on succes, -1 on failure, call clW_get_last_error for more info
 */
int cleanupOpenCLWrapper(OpenCLFunctionWrapper* wrapper);

/**
 * @brief gets the number of gpus
 *
 * @return the number of gpu-like devices found by opencl, -1 on error or if there are no gpu-like devices
*/
int get_number_of_gpus(void);

/**
 * @brief gets the number of platforms
 *
 * @return the number of gpu-like devices found by opencl, -1 on error or if there are no gpu-like devices
*/
int get_number_of_platforms(void);

/**
 * @brief Get the number of devices on a given platform
 *
 * @param platform_num the platform you want to query
 *
 * @return the number of devices on the specified platform -1 if something went wrong. call clW_get_last_error()
 */
int get_platform_no_devices(int platform_num);

#ifdef __cplusplus
}
#endif

#endif
