/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/util/timer.h>

#include <limits.h>

#if defined(WIN32)
#include <time.h>
#include <windows.h>
#else
#include <time.h>
#include <unistd.h>
#endif

/****************************************
* uecho_time_wait
****************************************/

void uecho_wait(clock_t mtime)
{
#if defined(WIN32)
  Sleep(mtime);
#else
  usleep(((useconds_t)(mtime * 1000)));
#endif
}

/****************************************
* uecho_time_wait
****************************************/

void uecho_waitrandom(clock_t mtime)
{
  double factor;
  long wait_time;

  factor = (double)rand() / (double)RAND_MAX;
  wait_time = (long)((double)mtime * factor);
  uecho_wait(wait_time);
}

/****************************************
* uecho_time_wait
****************************************/

clock_t uecho_getcurrentsystemtime(void)
{
  return (size_t)(time((time_t*)NULL));
}

/****************************************
* uecho_random
****************************************/

float uecho_random(void)
{
  static bool seed_done = false;

  if (seed_done == false) {
    srand((int)(uecho_getcurrentsystemtime() % INT_MAX));
    seed_done = true;
  }

  return (float)rand() / (float)RAND_MAX;
}
