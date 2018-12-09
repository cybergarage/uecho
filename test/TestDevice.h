/************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ************************************************************/

#ifndef _UECHO_TEST_TESTDEVICE_H_
#define _UECHO_TEST_TESTDEVICE_H_

#include <uecho/device.h>
#include <uecho/node_internal.h>

const uEchoObjectCode UECHO_TEST_MANCODE = 0x000000;
const uEchoObjectCode UECHO_TEST_OBJECTCODE = 0xF00101;
const uEchoPropertyCode UECHO_TEST_PROPERTY_SWITCHCODE = 0x80;
const byte UECHO_TEST_PROPERTY_SWITCH_ON = 0x30;
const byte UECHO_TEST_PROPERTY_SWITCH_OFF = 0x31;
const byte UECHO_TEST_PROPERTY_SWITCH_DEFAULT = UECHO_TEST_PROPERTY_SWITCH_ON;

const int UECHO_TEST_RESPONSE_WAIT_MAX_MTIME = 5000;
const int UECHO_TEST_RESPONSE_WAIT_RETLY_CNT = 100;

uEchoNode* uecho_test_createtestnode();
uEchoObject* uecho_test_createtestdevice();

#endif /* _UECHO_TEST_TESTDEVICE_H_ */
