/************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ************************************************************/

#ifndef _UECHO_TEST_TESTDEVICE_H_
#define _UECHO_TEST_TESTDEVICE_H_

#include <uecho/node.h>
#include <uecho/device.h>

const uEchoObjectCode UECHO_TEST_MANCODE = 0x000000;
const uEchoObjectCode UECHO_TEST_OBJECTCODE = 0xF00101;
const uEchoPropertyCode UECHO_TEST_PROPERTY_SWITCHCODE = 0x80;
const byte UECHO_TEST_PROPERTY_SWITCH_ON = 0x30;
const byte UECHO_TEST_PROPERTY_SWITCH_OFF = 0x31;

uEchoNode *uecho_test_createtestnode();
uEchoObject *uecho_test_createtestdevice();

#endif /* _UECHO_TEST_TESTDEVICE_H_ */
