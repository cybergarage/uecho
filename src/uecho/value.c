/******************************************************************
 * uEcho for C
 * Copyright (C) The uecho Authors 2026
 * This is licensed under BSD-style license, see file COPYING.
 ******************************************************************/
#include <string.h>
#include <uecho/std/_standard.h>
#include <uecho/value.h>

const uEchoValueConstraint* uecho_standard_getvalueconstraint(byte grpCode, byte clsCode, byte epc)
{
#if defined(UECHO_DATABASE_CODEC_METADATA) && UECHO_DATABASE_CODEC_METADATA
  const uEchoStdObject* obj = uecho_std_getobject(grpCode, clsCode);
  if (!obj)
    return NULL;
  const uEchoStdProperty* prop = uecho_std_object_getproperty(obj, epc);
  // MRA device definitions override superclass constraints. Profiles do not
  // inherit appliance superclass schemas. Unknown/excluded classes stay absent.
  if (!prop && grpCode != 0x0E)
    prop = uecho_std_object_getproperty(uecho_std_getobject(0, 0), epc);
  return prop ? prop->valueConstraint : NULL;
#else
  (void)grpCode;
  (void)clsCode;
  (void)epc;
  return NULL;
#endif
}

bool uecho_value_decode(const uEchoValueConstraint* constraint, const byte* data, size_t size, uEchoValue* value)
{
  uint32_t code = 0;
  if (!value || size > sizeof(value->raw) || (size && !data))
    return false;
  memset(value, 0, sizeof(*value));
  if (size)
    memcpy(value->raw, data, size);
  value->rawSize = size;
  value->status = uEchoValueUnsupported;
  if (!constraint)
    return true;
  value->unit = constraint->unit;
  value->status = uEchoValueInvalid;
  if (size != constraint->size || size == 0 || size > 4)
    return true;
  for (size_t n = 0; n < size; n++)
    code = (code << 8) | data[n];
  value->code = code;
  for (size_t n = 0; n < constraint->stateCount; n++) {
    if (constraint->states[n].code != code)
      continue;
    value->name = constraint->states[n].name;
    value->readOnly = constraint->states[n].readOnly;
    value->status = constraint->hasNumber ? uEchoValueSpecial : uEchoValueState;
    return true;
  }
  value->integer = code;
  if (constraint->isSigned && (data[0] & 0x80))
    value->integer -= (INT64_C(1) << (size * 8));
  if (constraint->hasNumber) {
    if (value->integer < constraint->minimum || constraint->maximum < value->integer)
      return true;
    value->number = value->integer * constraint->multiple;
    value->status = uEchoValueNumber;
  }
  else
    value->status = uEchoValueUnknown;
  return true;
}

bool uecho_value_encode(const uEchoValueConstraint* constraint, uEchoValueStatus kind, int64_t integer, byte* output, size_t capacity, size_t* outputSize)
{
  byte data[4];
  uEchoValue value;
  uint32_t code;
  if (!outputSize)
    return false;
  *outputSize = constraint ? constraint->size : 0;
  if (!constraint || !constraint->size || constraint->size > sizeof(data))
    return false;
  if (kind == uEchoValueNumber) {
    if (!constraint->hasNumber || integer < constraint->minimum || constraint->maximum < integer)
      return false;
  }
  else if (kind == uEchoValueState || kind == uEchoValueSpecial) {
    if (integer < 0 || integer > UINT32_MAX)
      return false;
  }
  else
    return false;
  code = (uint32_t)integer;
  for (size_t n = constraint->size; n > 0; n--) {
    data[n - 1] = code & 0xFF;
    code >>= 8;
  }
  if (!uecho_value_decode(constraint, data, constraint->size, &value) || value.status != kind)
    return false;
  if ((kind == uEchoValueNumber && value.integer != integer) || (kind != uEchoValueNumber && value.code != (uint32_t)integer))
    return false;
  if (!output || capacity < constraint->size)
    return false;
  memcpy(output, data, constraint->size);
  return true;
}
