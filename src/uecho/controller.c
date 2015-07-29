/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/controller.h>

uEchoControlPoint *uecho_controller_new()
{
	uEchoControlPoint *ctrl;

    ctrl = (uEchoControlPoint *)malloc(sizeof(uEchoControlPoint));

    if (!ctrl)
        return NULL;

    ctrl->mutex = uecho_mutex_new();

	return ctrl;
}

void uecho_controller_delete(uEchoControlPoint *ctrl)
{
	uecho_controller_stop(ctrl);
	
	uecho_mutex_delete(ctrl->mutex);

    free(ctrl);
}

bool uecho_controller_start(uEchoControlPoint *ctrl)
{
	return true;
}

bool uecho_controller_stop(uEchoControlPoint *ctrl)
{
	return true;
}

bool uecho_controller_isrunning(uEchoControlPoint *ctrl)
{
	return true;
}
