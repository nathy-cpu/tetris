#include "tetris.h"
#ifdef _WIN32

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>

DWORD WINAPI thread_func_adapter(LPVOID arg)
{
    void** args = (void**)arg;
    void* (*func)(void*) = args[0];
    void* real_arg = args[1];
    free(args);
    func(real_arg);
    return 0;
}

void Thread_Init(Thread* thread, void* (*func)(void*), void* arg)
{
    void** args = malloc(2 * sizeof(void*));
    args[0] = func;
    args[1] = arg;
    *thread = CreateThread(NULL, 0, thread_func_adapter, args, 0, NULL);
}

void Thread_Join(Thread thread)
{
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
}

void Mutex_Init(Mutex* mutex)
{
    InitializeCriticalSection(mutex);
}

void Mutex_Destroy(Mutex* mutex)
{
    DeleteCriticalSection(mutex);
}

void Mutex_Lock(Mutex* mutex)
{
    EnterCriticalSection(mutex);
}

void Mutex_Unlock(Mutex* mutex)
{
    LeaveCriticalSection(mutex);
}

void SleepFor(int millisecond)
{
    Sleep(millisecond);
}

bool Thread_TimedJoin(Thread thread, unsigned int milliseconds)
{
    DWORD result = WaitForSingleObject(thread, milliseconds);
    if (result == WAIT_OBJECT_0) {
        CloseHandle(thread);
        return true;
    }
    // Handle timeout or failure
    return false;
}

void Thread_Cancel(Thread* thread)
{
    TerminateThread(thread.handle, 0);
    CloseHandle(thread.handle);
}

#else // POSIX

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void Thread_Init(Thread* thread, void* (*function)(void*), void* arg)
{
    pthread_create(thread, NULL, function, arg);
}

void Thread_Join(const Thread* thread)
{
    pthread_join(*thread, NULL);
}

void Mutex_Init(Mutex* mutex)
{
    pthread_mutex_init(mutex, NULL);
}

void Mutex_Destroy(Mutex* mutex)
{
    pthread_mutex_destroy(mutex);
}

void Mutex_Lock(Mutex* mutex)
{
    pthread_mutex_lock(mutex);
}

void Mutex_Unlock(Mutex* mutex)
{
    pthread_mutex_unlock(mutex);
}

void SleepFor(const int millisecond)
{
    usleep(millisecond * 1000);
}

bool Thread_TimedJoin(const Thread* thread, unsigned int milliseconds)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += milliseconds / 1000;
    ts.tv_nsec += (milliseconds % 1000) * 1000000;
    // Normalize nanoseconds
    if (ts.tv_nsec >= 1000000000) {
        ts.tv_nsec -= 1000000000;
        ts.tv_sec += 1;
    }
    const int result = pthread_timedjoin_np(*thread, NULL, &ts);
    if (result == 0)
        return true;
    return false;
}

void Thread_Cancel(Thread* thread)
{
    pthread_cancel(*thread);
}

#endif