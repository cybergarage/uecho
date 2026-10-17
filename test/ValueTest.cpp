/******************************************************************
 * uEcho for C
 * Copyright (C) The uecho Authors 2026
 * This is licensed under BSD-style license, see file COPYING.
 ******************************************************************/
#include <array>
#include <boost/test/unit_test.hpp>
#include <uecho/value.h>

BOOST_AUTO_TEST_CASE(ValueScalar)
{
  const std::array<uEchoValueStateEntry, 1> states = { { { 0x7E, "unmeasurable", true } } };
  const uEchoValueConstraint constraint = { 1, true, true, -127, 125, 0.1, "Celsius", states.data(), states.size() };
  uEchoValue value;
  std::array<byte, 4> output = { 0xAA };
  size_t size;
  for (int n = -128; n <= 127; n++) {
    auto data = static_cast<byte>(n);
    BOOST_REQUIRE(uecho_value_decode(&constraint, &data, 1, &value));
    BOOST_CHECK_EQUAL(value.raw[0], data);
    if (n >= -127 && n <= 125) {
      BOOST_CHECK_EQUAL(value.status, uEchoValueNumber);
      BOOST_CHECK_EQUAL(value.integer, n);
      BOOST_CHECK_CLOSE(value.number, n * 0.1, 0.000001);
      BOOST_REQUIRE(uecho_value_encode(&constraint, value.status, value.integer, output.data(), output.size(), &size));
      BOOST_CHECK_EQUAL(output[0], data);
    }
    else if (n == 126) {
      BOOST_CHECK_EQUAL(value.status, uEchoValueSpecial);
      BOOST_CHECK_EQUAL(value.name, "unmeasurable");
      BOOST_REQUIRE(uecho_value_encode(&constraint, value.status, value.code, output.data(), output.size(), &size));
      BOOST_CHECK_EQUAL(output[0], data);
    }
    else
      BOOST_CHECK_EQUAL(value.status, uEchoValueInvalid);
  }
  BOOST_CHECK(!uecho_value_encode(&constraint, uEchoValueNumber, 126, output.data(), output.size(), &size));
  BOOST_CHECK(!uecho_value_encode(&constraint, uEchoValueNumber, -128, output.data(), output.size(), &size));
  BOOST_CHECK(!uecho_value_encode(&constraint, uEchoValueNumber, 1, output.data(), 0, &size));
  BOOST_CHECK_EQUAL(size, 1);
  BOOST_CHECK(uecho_value_decode(&constraint, output.data(), 0, &value));
  BOOST_CHECK_EQUAL(value.status, uEchoValueInvalid);
  BOOST_CHECK(uecho_value_decode(&constraint, output.data(), 2, &value));
  BOOST_CHECK_EQUAL(value.status, uEchoValueInvalid);
  BOOST_CHECK(uecho_value_decode(nullptr, output.data(), 2, &value));
  BOOST_CHECK_EQUAL(value.status, uEchoValueUnsupported);
  BOOST_CHECK_EQUAL(value.rawSize, 2);
  BOOST_CHECK(!uecho_value_decode(nullptr, nullptr, 1, &value));
  BOOST_CHECK(!uecho_value_decode(nullptr, output.data(), 256, &value));
  BOOST_CHECK(!uecho_value_decode(nullptr, output.data(), 1, nullptr));
}

BOOST_AUTO_TEST_CASE(ValueSignedBounds)
{
  for (size_t width : { 1, 2, 4 }) {
    int64_t minimum = -(INT64_C(1) << (width * 8 - 1));
    int64_t maximum = -minimum - 1;
    const uEchoValueConstraint constraint = { width, true, true, minimum, maximum, 1, nullptr, nullptr, 0 };
    for (int64_t integer : { minimum, maximum, INT64_C(-1), INT64_C(0) }) {
      std::array<byte, 4> data;
      size_t size;
      uEchoValue value;
      BOOST_REQUIRE(uecho_value_encode(&constraint, uEchoValueNumber, integer, data.data(), data.size(), &size));
      BOOST_REQUIRE(uecho_value_decode(&constraint, data.data(), size, &value));
      BOOST_CHECK_EQUAL(value.integer, integer);
    }
  }
}

BOOST_AUTO_TEST_CASE(ValueGeneratedMetadata)
{
  BOOST_CHECK(!uecho_standard_getvalueconstraint(0x0E, 0xF0, 0x8F));
  BOOST_CHECK(!uecho_standard_getvalueconstraint(0xFF, 0xFF, 0x93));
  auto mode = uecho_standard_getvalueconstraint(1, 0x30, 0xB0);
#if defined(UECHO_DATABASE_CODEC_METADATA) && UECHO_DATABASE_CODEC_METADATA && !defined(UECHO_DATABASE_NONE)
  BOOST_REQUIRE(mode);
  BOOST_CHECK(uecho_standard_getvalueconstraint(1, 0x30, 0x93));
  for (byte data = 0x40; data <= 0x45; data++) {
    uEchoValue value;
    byte output;
    size_t size;
    BOOST_REQUIRE(uecho_value_decode(mode, &data, 1, &value));
    BOOST_CHECK_EQUAL(value.status, uEchoValueState);
    if (data == 0x40)
      BOOST_CHECK_EQUAL(value.name, "other");
    BOOST_REQUIRE(uecho_value_encode(mode, value.status, value.code, &output, 1, &size));
    BOOST_CHECK_EQUAL(data, output);
  }
  byte unknown = 0x46;
  uEchoValue value;
  BOOST_REQUIRE(uecho_value_decode(mode, &unknown, 1, &value));
  BOOST_CHECK_EQUAL(value.status, uEchoValueUnknown);
  for (byte epc : { 0xB3, 0xBB }) {
    auto constraint = uecho_standard_getvalueconstraint(1, 0x30, epc);
    BOOST_REQUIRE(constraint);
    BOOST_CHECK_EQUAL(constraint->unit, "Celsius");
    byte sentinel = epc == 0xB3 ? 0xFD : 0x7E;
    BOOST_REQUIRE(uecho_value_decode(constraint, &sentinel, 1, &value));
    BOOST_CHECK_EQUAL(value.status, uEchoValueSpecial);
    BOOST_CHECK_EQUAL(value.name, epc == 0xB3 ? "undefined" : "unmeasurable");
    byte data = epc == 0xB3 ? 50 : (byte)-127;
    BOOST_REQUIRE(uecho_value_decode(constraint, &data, 1, &value));
    BOOST_CHECK_EQUAL(value.status, uEchoValueNumber);
    BOOST_CHECK_EQUAL(value.integer, epc == 0xB3 ? 50 : -127);
  }
#else
  BOOST_CHECK(!mode);
#endif
  BOOST_CHECK(!uecho_standard_getvalueconstraint(0xFF, 0xFF, 0xFF));
}

BOOST_AUTO_TEST_CASE(ValueUnsignedBounds)
{
  const uEchoValueConstraint constraint = { 4, true, false, 0, UINT32_MAX, 1, nullptr, nullptr, 0 };
  std::array<byte, 4> output;
  size_t size;
  uEchoValue value;
  BOOST_REQUIRE(uecho_value_encode(&constraint, uEchoValueNumber, UINT32_MAX, output.data(), output.size(), &size));
  BOOST_REQUIRE(uecho_value_decode(&constraint, output.data(), size, &value));
  BOOST_CHECK_EQUAL(value.integer, UINT32_MAX);
  BOOST_CHECK(!uecho_value_encode(&constraint, uEchoValueNumber, -1, output.data(), output.size(), &size));
  BOOST_CHECK(!uecho_value_encode(&constraint, uEchoValueNumber, INT64_C(4294967296), output.data(), output.size(), &size));
}
