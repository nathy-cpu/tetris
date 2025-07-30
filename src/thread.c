#include "tetris.h"

#ifdef _WIN32

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

#else // POSIX

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
    sleep(millisecond * 1000);
}

#endif