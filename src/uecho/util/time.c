/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/util/time.h>

#include <limits.h>

#if defined(WIN32) && !defined(ITRON) && !defined (WINCE)
#include <windows.h>
#include <time.h>
#elif defined(WIN32) && defined (WINCE)
#include <windows.h>
#include <time.h>
//#include <altcecrt.h>
#elif defined(BTRON) 
#include <btron/proctask.h>
#include <btron/clk.h>
#elif defined(ITRON)
#include <kernel.h>
#elif defined(TENGINE) && !defined(PROCESS_BASE)
#include <tk/tkernel.h>
#include <time.h>
#elif defined(TENGINE) && defined(PROCESS_BASE)
#include <tk/tkernel.h>
#include <btron/proctask.h>
#include <time.h>
#else
#include <unistd.h>
#include <time.h>
#endif

/****************************************
* uecho_time_wait
****************************************/

void uecho_wait(size_t mtime)
{
#if defined(WIN32) && !defined(ITRON)
	Sleep(mtime);
#elif defined(BTRON)
	slp_tsk(mtime);
#elif defined(ITRON)
	tslp_tsk(mtime);
#elif defined(TENGINE) && !defined(PROCESS_BASE)
	tk_slp_tsk(mtime);
#elif defined(TENGINE) && defined(PROCESS_BASE)
	b_slp_tsk(mtime);
#else
	usleep(((useconds_t)(mtime * 1000)));
#endif
}

/****************************************
* uecho_time_wait
****************************************/

void uecho_waitrandom(size_t mtime)
{
	double factor;
	long waitTime;

	factor = (double)rand() / (double)RAND_MAX;
	waitTime = (long)((double)mtime * factor);
	uecho_wait(waitTime);
}

/****************************************
* uecho_time_wait
****************************************/

size_t uecho_getcurrentsystemtime()
{
#if defined(BTRON)
	STIME clock_t;
	TIMEZONE tz;
	STIME localtime;
	if (get_tim(&clock_t, &tz) != 0)
		return 0;
	localtime = clock_t - tz.adjust + (tz.dst_flg ? (tz.dst_adj*60): 0);
#elif defined(ITRON)
	static bool initialized = false;
	SYSTIM sysTim;
	if (initialized == false) {
		sysTim.utime = 0;
		sysTim.ltime = 0;
		set_tim(&sysTim);
	}
	get_tim(&sysTim);
#endif

#if defined(BTRON)
	return localtime;
#elif defined(ITRON)
	return ((sysTim.utime / 1000) << 32) + (sysTim.ltime / 1000);
#else
  return 0;//time((clock_t *)NULL);
#endif
}

/****************************************
* uecho_random
****************************************/

float uecho_random()
{
	static bool seedDone = false;

	if (seedDone == false) {
		srand((int)(uecho_getcurrentsystemtime() % INT_MAX));
		seedDone = true;
	}
  
	return (float)rand() / (float)RAND_MAX;
}
