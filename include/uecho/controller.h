/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_CONTROLLER_H_
#define _UECHO_CONTROLLER_H_

#include <stdbool.h>
#include <uecho/util/mutex.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Data Type
****************************************/

/**
 * Prototype for control point's device listener callback.
 * 
 * @param udn The UDN of the device, that the status update concerns
 * @param status The new status
 */
	
typedef struct _uEchoControlPoint {
	uEchoMutex *mutex;
} uEchoControlPoint;
	
/****************************************************************************
 * Control Point top-level control
 ****************************************************************************/

/**
 * Create a new control point. Does not start any threads.
 *
 * @return A newly-created uEchoControlPoint
 */
uEchoControlPoint *uecho_controller_new();

/**
 * Destroy the given control point
 *
 * @param ctrlPoint The control point struct to destroy
 */
void uecho_controller_delete(uEchoControlPoint *ctrlPoint);

/**
 * Activate the control point. Starts listening for SSDP messages etc.
 * You must call this function before you can actually use a control point.
 *
 * @param ctrlPoint The control point to start
 *
 * @return true if successful; otherwise false
 *
 */
bool uecho_controller_start(uEchoControlPoint *ctrlPoint);

/**
 * Stop the control point. Stops sending/receiveing/responding to any messages.
 *
 * @param ctrlPoint The control point to stop
 *
 * @return true if successful; otherwise false
 *
 */
bool uecho_controller_stop(uEchoControlPoint *ctrlPoint);

/**
* Check if  the control point is activated.
*
* @param ctrlPoint The control point to stop
*
* @return true if running; otherwise false
*
*/
bool uecho_controller_isrunning(uEchoControlPoint *ctrlPoint);

#ifdef  __cplusplus
}
#endif

#endif /* _UECHO_CONTROLLER_H_ */
