/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#if !defined (WIN32)
#include <signal.h>
#endif

#include <uecho/util/thread.h>
#include <uecho/util/timer.h>
#include <string.h>

static void uecho_sig_handler(int sign);

/****************************************
* Thread Function
****************************************/

#if defined(WIN32)
static DWORD WINAPI Win32ThreadProc(LPVOID lpParam)
{
	uEchoThread *thread;

	thread = (uEchoThread *)lpParam;
	if (thread->action != NULL)
		thread->action(thread);
	
	return 0;
}
#else
static void *PosixThreadProc(void *param)
{
	sigset_t set;
	struct sigaction actions;
	uEchoThread *thread = (uEchoThread *)param;

	sigfillset(&set);
	sigdelset(&set, SIGQUIT);
	pthread_sigmask(SIG_SETMASK, &set, NULL);
	
	memset(&actions, 0, sizeof(actions));
	sigemptyset(&actions.sa_mask);
	actions.sa_flags = 0;
	actions.sa_handler = uecho_sig_handler;
	sigaction(SIGQUIT, &actions, NULL);

	if (thread->action != NULL)
		thread->action(thread);
	
	return 0;
}
#endif

/****************************************
* uecho_thread_new
****************************************/

uEchoThread *uecho_thread_new(void)
{
	uEchoThread *thread;

	thread = (uEchoThread *)malloc(sizeof(uEchoThread));

  if (!thread)
    return NULL;
  
  uecho_list_node_init((uEchoList *)thread);
		
  thread->runnableFlag = false;
  thread->action = NULL;
  thread->userData = NULL;

	return thread;
}

/****************************************
* uecho_thread_delete
****************************************/

bool uecho_thread_delete(uEchoThread *thread)
{
  if (!thread)
    return false;
  
  if (thread->runnableFlag == true) {
		uecho_thread_stop(thread);
  }

	uecho_thread_remove(thread);
	
	free(thread);

	return true;
}

/****************************************
* uecho_thread_start
****************************************/

bool uecho_thread_start(uEchoThread *thread)
{
  if (!thread)
    return false;
  
	thread->runnableFlag = true;

#if defined(WIN32)
	thread->hThread = CreateThread(NULL, 0, Win32ThreadProc, (LPVOID)thread, 0, &thread->threadID);
#else
	pthread_attr_t thread_attr;
	if (pthread_attr_init(&thread_attr) != 0) {
		thread->runnableFlag = false;
    return false;
  }

  if (pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED) !=0) {
		thread->runnableFlag = false;
		pthread_attr_destroy(&thread_attr);
		return false;
  }

  if (pthread_create(&thread->pThread, &thread_attr, PosixThreadProc, thread) != 0) {
		thread->runnableFlag = false;
		pthread_attr_destroy(&thread_attr);
		return false;
	}
	pthread_attr_destroy(&thread_attr);
#endif
	
	return true;
}

/****************************************
* uecho_thread_stop
****************************************/

bool uecho_thread_stop(uEchoThread *thread)
{
  if (!thread)
    return false;
  
  if (thread->runnableFlag == true) {
		thread->runnableFlag = false;
#if defined(WIN32)
		TerminateThread(thread->hThread, 0);
		WaitForSingleObject(thread->hThread, INFINITE);
#else
		#if  defined(TARGET_OS_MAC) || defined(TARGET_OS_IPHONE)
		pthread_kill(thread->pThread, 0);
		#else
    pthread_kill(thread->pThread, SIGQUIT);
		#endif
#endif
	}

	return true;
}

/****************************************
* uecho_thread_restart
****************************************/

bool uecho_thread_restart(uEchoThread *thread)
{
	uecho_thread_stop(thread);
	return uecho_thread_start(thread);
}

/****************************************
* uecho_thread_isrunnable
****************************************/

bool uecho_thread_isrunnable(uEchoThread *thread)
{
  if (!thread)
    return false;
  
#if !defined(WIN32)
  pthread_testcancel();
#endif
  
	return thread->runnableFlag;
}

/****************************************
 * uecho_thread_isrunning
 ****************************************/

bool uecho_thread_isrunning(uEchoThread *thread)
{
  if (!thread)
    return false;
  
  return thread->runnableFlag;
}

/****************************************
* uecho_thread_setaction
****************************************/

void uecho_thread_setaction(uEchoThread *thread, uEchoThreadFunc func)
{
  if (!thread)
    return;
  
	thread->action = func;
}

/****************************************
* uecho_thread_setuserdata
****************************************/

void uecho_thread_setuserdata(uEchoThread *thread, void *value)
{
  if (!thread)
    return;

  thread->userData = value;
}

/****************************************
* uecho_thread_getuserdata
****************************************/

void *uecho_thread_getuserdata(uEchoThread *thread)
{
  if (!thread)
    return NULL;

  return thread->userData;
}

/****************************************
 * uecho_sig_handler
 ****************************************/

static void uecho_sig_handler(int sign)
{
}
