/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#if !defined (WIN32) && !defined(WINCE)
#include <signal.h>
#endif

#include <uecho/util/thread.h>
#include <uecho/util/time.h>
#include <string.h>

/* Private function prototypes */
static void sig_handler(int sign);

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
#elif defined(WINCE)
static DWORD WINAPI Win32ThreadProc(LPVOID lpParam)
{
	uEchoThread *thread = (uEchoThread *)lpParam;

	//Theo Beisch: make sure we're runnable
	//thread->runnableFlag = true;
	//(moved to start() with Visa Smolander's mod)

	thread->isRunning = true;

#if defined DEBUG_LOCKS
	memdiags_tlist_addthread(thread);
#endif

	if (thread->action != NULL) {
#if defined DEBUG
		printf ("#real Thr=%X hNd=%X lpP=%X %s start\n",thread,thread->hThread,lpParam , thread->friendlyName);
#endif
		thread->action(thread);
	}
#if defined DEBUG
		printf ("** REAL ExitThread(0)*\n"); 
		printf ("*2 Thread ret4close 0x%Xh\n",GetCurrentThreadId());
		printf ("*3 Thread %X %s \n",thread, thread->friendlyName);
#endif
	thread->isRunning = false;
	if (thread->deletePending) {
		uecho_thread_delete(thread);
	}
	//proper friendly thread exit for WINCE
	uecho_thread_exit(0);
	//dummy - compiler wants a return statement
	return 0;
}

#else

static void *PosixThreadProc(void *param)
{
	sigset_t set;
	struct sigaction actions;
	uEchoThread *thread = (uEchoThread *)param;

	/* SIGQUIT is used in thread deletion routine
	 * to force accept and recvmsg to return during thread
	 * termination process. */
	sigfillset(&set);
	sigdelset(&set, SIGQUIT);
	pthread_sigmask(SIG_SETMASK, &set, NULL);
	
	memset(&actions, 0, sizeof(actions));
	sigemptyset(&actions.sa_mask);
	actions.sa_flags = 0;
	actions.sa_handler = sig_handler;
	sigaction(SIGQUIT, &actions, NULL);

	if (thread->action != NULL)
		thread->action(thread);
	
	return 0;
}

#endif

/****************************************
* uecho_thread_new
****************************************/

uEchoThread *uecho_thread_new()
{
	uEchoThread *thread;

	thread = (uEchoThread *)malloc(sizeof(uEchoThread));

	if ( NULL != thread )
	{
		uecho_list_node_init((uEchoList *)thread);
		
		thread->runnableFlag = false;
		thread->action = NULL;
		thread->userData = NULL;
	}

#if defined (WINCE)
	thread->hThread = NULL;
	//WINCE trial result: default sleep value to keep system load down
	thread->sleep = UECHO_THREAD_MIN_SLEEP;
	thread->isRunning = false;
	thread->deletePending = false;
#if defined DEBUG
	strcpy(thread->friendlyName,"-");
#endif //DEBUG
#endif //WINCE

	return thread;
}

/****************************************
* uecho_thread_delete
****************************************/

bool uecho_thread_delete(uEchoThread *thread)
{
#if defined WINCE
	bool stop = false;

	if ((thread->hThread==NULL) ||
		((thread->isRunning) && (stop = uecho_thread_stop(thread) == true)) ||	
		(thread->isRunning == false)) {

#if defined DEBUG
		printf("***** Delete entered handle=%x isRunning=%d stopResult=%d \n",thread->hThread, thread->isRunning, stop);
		printf("***** Delete and free for Thread %X %s\n",thread, thread->friendlyName);
#endif				

		if (thread->hThread!=NULL) CloseHandle(thread->hThread);
		uecho_list_remove((uEchoList *)thread);
#if defined DEBUG_MEM
		memdiags_tlist_removethread(thread);
#endif
		free(thread);
		return true;
	}

#if defined DEBUG
	printf("***** Stop failed for Thread %X %s - marking thread for selfDelete\n",thread, thread->friendlyName);
#endif
	//setting this will cause the real thread exit to call delete() again
	thread->deletePending = true;

	return false;
} //WINCE
#else //all except WINCE

	if (thread->runnableFlag == true)
		uecho_thread_stop(thread);

	uecho_thread_remove(thread);
	
	free(thread);

	return true;
}
#endif

/****************************************
* uecho_thread_start
****************************************/

bool uecho_thread_start(uEchoThread *thread)
{
	thread->runnableFlag = true;

#if defined(WIN32)
	thread->hThread = CreateThread(NULL, 0, Win32ThreadProc, (LPVOID)thread, 0, &thread->threadID);
#elif defined (WINCE)
	{

	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = true;
	saAttr.lpSecurityDescriptor = NULL;
	thread->deletePending = false;
	thread->hThread = CreateThread(&saAttr, 0, Win32ThreadProc, (LPVOID)thread, 0, &thread->threadID);
	}
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
#ifdef STACK_SIZE
	/* Optimization : not we can set STACK_SIZE attribute at compilation time */
	/* to specify thread stack size */
  if (pthread_attr_setstacksize (&thread_attr,STACK_SIZE) != 0) {
		thread->runnableFlag = false;
		pthread_attr_destroy(&thread_attr);
		return false;
  }
#endif
	/* MODIFICATION END Fabrice Fontaine Orange 24/04/2007 */
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
#if defined (WINCE)
	int i, j;
	bool result;
	DWORD dwExitCode;
#endif

	if (thread->runnableFlag == true) {
		thread->runnableFlag = false;
#if defined(WIN32)
		TerminateThread(thread->hThread, 0);
		WaitForSingleObject(thread->hThread, INFINITE);
		//tb: this will create a deadlock if the thread is on a blocking socket
#elif defined (WINCE)
		// Theo Beisch: while the above code apparently works under WIN32 (NT/XP) 
		// TerminateThread as brute force is not recommended by M$
		// (see API) and actually crashes WCE
		// WINCE provides no safe means of terminating a thread, 
		// so we can only mark the uecho_thread (context) for later deletion and 
		// do the delete(thread) cleanup on return of the Win32ThreadProc.
		// Accordingly we simulate the OK exit here as a "look ahead" (what a hack ;-) )
		for (i=0; i<UECHO_THREAD_SHUTDOWN_ATTEMPTS; ++i){
#if defined (DEBUG)
			printf("# thread stop mainloop %X %s %d. try\n",thread,thread->friendlyName,i+1);
#endif
			j=0;
			if (result = GetExitCodeThread(thread->hThread,&dwExitCode)) {
				if (dwExitCode != STILL_ACTIVE) {
#if defined (DEBUG)
					printf("Thread %X %s ended graceful: xCode=%d\n",thread,thread->friendlyName,dwExitCode);
#endif
					return true;
				} 
			}
			uecho_wait(UECHO_THREAD_MIN_SLEEP);
		}
		// ok - if everything up to here failed
#if defined DEBUG
		if (dwExitCode){
			printf ("Thread %X - %s has not yet terminated - exit code %x \n",thread,thread->friendlyName,dwExitCode);
		}
#endif

		if (dwExitCode)
			return false;

		return true;
		//end WINCE
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
* uecho_thread_sleep
****************************************/
// Theo Beisch
// Added this to improve external thread control 
// by having a finer timer tick granularity

#if defined (WINCE)
void uecho_thread_sleep(uEchoThread *thread) {
	CgTime until;
#if defined DEBUG_MEM
	printf("###### Going to sleep - elapsed since last sleep = %d\n",memdiags_getelapsedtime(thread->hThread));
#endif
	until = uecho_getcurrentsystemtime() + (thread->sleep)/1000;
	while ((uecho_getcurrentsystemtime()<until) && thread->runnableFlag){
		uecho_wait(0);
	}
}
#endif

/****************************************
* uecho_thread_exit (friendly exit) 
****************************************/
// Theo Beisch
// to be called from the thread's loop only

#if defined (WINCE)
VOID uecho_thread_exit(DWORD exitCode) {
	ExitThread(exitCode);	
}
#endif

/****************************************
* uecho_thread_restart
****************************************/

bool uecho_thread_restart(uEchoThread *thread)
{
	uecho_thread_stop(thread);
	uecho_thread_start(thread);
	return true;
}

/****************************************
* uecho_thread_isrunnable
****************************************/

bool uecho_thread_isrunnable(uEchoThread *thread)
{
#if !defined(WIN32) && !defined (WINCE)
  pthread_testcancel();
#endif
  
	return thread->runnableFlag;
}

/****************************************
* uecho_thread_setaction
****************************************/

void uecho_thread_setaction(uEchoThread *thread, uEchoThreadFunc func)
{
	thread->action = func;
}

/****************************************
* uecho_thread_setuserdata
****************************************/

void uecho_thread_setuserdata(uEchoThread *thread, void *value)
{
	thread->userData = value;
}

/****************************************
* uecho_thread_getuserdata
****************************************/

void *uecho_thread_getuserdata(uEchoThread *thread)
{
	return thread->userData;
}

/* Private helper functions */

static void sig_handler(int sign)
{
	return;
}
