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

/* Just a wrapper fot the mutex type and it's associated functions */
typedef struct mutex_agn
{
    #ifdef _WIN32
    HANDLE mutex;
    #else
    pthread_mutex_t mutex;
    #endif
}mutex_agn;

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

/**
 * @brief Locks the given mutex, this function will wait and undetermined amount of time
 * 
 * @param m The mutex to lock
*/
void _mutex_lock_agn(mutex_agn* m)
{
    #ifdef _WIN32
    WaitForSingleObject(m->mutex, INFINITE);
    #else
    pthread_mutex_lock(&m->mutex);
    #endif
}

/**
 * @brief Locks the given mutex, this function will wait for the given amount of time
 * 
 * @param m The mutex to lock
 * @param miliseconds The amount of time to wait, in miliseconds
*/
void _mutex_lock_fixed_time_agn(mutex_agn* m, int miliseconds)
{
    #ifdef _WIN32
    WaitForSingleObject(m->mutex, miliseconds);
    #else
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += miliseconds / 1000;
    ts.tv_nsec += (miliseconds % 1000) * 1000;
    pthread_mutex_timedlock(&m->mutex, &ts);
    #endif
}

/**
 * @brief Unlock the given mutex
 * 
 * @param m The mutex to unlock
*/
void _mutex_unlock_agn(mutex_agn* m)
{
    #ifdef _WIN32
    ReleaseMutex(m);
    #else
    pthread_mutex_unlock(m);
    #endif   
}
