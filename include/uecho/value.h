/******************************************************************
 * uEcho for C
 * Copyright (C) The uecho Authors 2026
 * This is licensed under BSD-style license, see file COPYING.
 ******************************************************************/
#ifndef _UECHO_VALUE_H_
#define _UECHO_VALUE_H_
#include <stdint.h>
#include <uecho/typedef.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  uEchoValueUnsupported,
  uEchoValueInvalid,
  uEchoValueUnknown,
  uEchoValueNumber,
  uEchoValueState,
  uEchoValueSpecial
} uEchoValueStatus;

typedef struct {
  uint32_t code;
  const char* name;
  bool readOnly;
} uEchoValueStateEntry;

/* Immutable generated constraint. Borrowed for process lifetime. Unsupported
 * schemas have no constraint; no runtime JSON parser is required. */
typedef struct {
  size_t size;
  bool hasNumber;
  bool isSigned;
  int64_t minimum;
  int64_t maximum;
  double multiple;
  const char* unit;
  const uEchoValueStateEntry* states;
  size_t stateCount;
} uEchoValueConstraint;

/* Owns the original EDT even for unsupported/invalid values. name and unit
 * point into immutable generated metadata. integer is the wire number before
 * scaling; number is integer * multiple. State/special values use code. */
typedef struct {
  uEchoValueStatus status;
  byte raw[255];
  size_t rawSize;
  int64_t integer;
  double number;
  uint32_t code;
  const char* name;
  const char* unit;
  bool readOnly;
} uEchoValue;

/* NULL means absent/excluded/unsupported metadata, never device support. */
const uEchoValueConstraint* uecho_standard_getvalueconstraint(byte grpCode, byte clsCode, byte epc);
/* False only for invalid API arguments (including EDT larger than 255 bytes).
 * Schema/value outcomes are reported in value.status. No allocation or I/O. */
bool uecho_value_decode(const uEchoValueConstraint* constraint, const byte* data, size_t size, uEchoValue* value);
/* Encodes an unscaled integer or an exact state/special code. Validates the
 * constraint; does not send, retry, or apply device access policy. outputSize
 * is required capacity on failure and bytes written on success. */
bool uecho_value_encode(const uEchoValueConstraint* constraint, uEchoValueStatus kind, int64_t integer, byte* output, size_t capacity, size_t* outputSize);
#ifdef __cplusplus
}
#endif
#endif
