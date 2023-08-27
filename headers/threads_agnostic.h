#pragma once

#ifdef _WIN32

#include <processthreadsapi.h>
#include <handleapi.h>


typedef HANDLE thread_handle;
typedef struct
{
    LPSECURITY_ATTRIBUTES lpThreadAttributes;
    SIZE_T dwStackSize;
    DWORD dwCreationFlags;
    LPDWORD lpThreadId;
}attr_t;

#else

#include <pthread.h>

typedef pthread_t thread_handle;
typedef pthread_attr_t attr_t;

#endif

typedef struct thread
{
    thread_handle handle;
    void* (*func)(void*);
    void* arg;
    int is_running;
    attr_t attributes;
}thread;

thread* thread_create(void* (*func)(void*), void* arg)
{
    thread* t = (thread*)malloc(sizeof(thread));
    t->func = func;
    t->arg = arg;
    t->is_running = 1;
    return t;
}

/*
 * @brief Initializes a thread
 *
 * @param t The thread to start
 * @return 0 on success, -1 on failure
 */
int thread_init(thread* t, void* (*func)(void*), void* arg)
{
    if (!t) 
        return 1;
    if (t->is_running)
        return 2;
    t->func = func;
    t->arg = arg;
    return 0;
}

/*
 * @brief Creates a thread
 *
 * @param t The thread to join
 * @param attach Whether to attach(1) the thread or not(0)
 * @return 0 on success, -1 on failure
 */
void thread_start(thread* t, int attach)
{
    if (!t)
        return;
    if (t->is_running)
        return;
#ifdef _WIN32
    t->handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)t->func, (LPVOID)t->arg, 0, NULL);
#else
    pthread_create(&t->handle, NULL, t->func, t->arg);
#endif
    t->is_running = 1;
    if (attach == 0)
    {
#ifdef _WIN32
        /* Do nothing I guess */
#else
        pthread_detach(t->handle);
#endif
    }
}


/*
 * @brief Closes a thread forcefully
 *
 * @param t The thread to close
 * @param ExitCode The exit code to return
 * @return whatever the underlying thread closing function returns
*/
int thread_close(thread* t, int ExitCode)
{
#ifdef _WIN32
    int ret = (int)TerminateThread(t->handle, (DWORD)ExitCode);
    CloseHandle(t->handle);
    return ret;
#else
    int ret = (int)pthread_cancel(t->handle, NULL);
    pthread_join(t->handle, NULL);
    return ret;
#endif
}

/* Executes the given function, with the given arguments, and nothing more*/
void execute_async(void (*foo)(void*), void* arg)
{
#ifdef _WIN32
    HANDLE threadHandle;
    threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)foo, arg, 0, NULL);
    CloseHandle(threadHandle);
#else
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, foo, arg);
    pthread_detach(thread_id);
#endif
}