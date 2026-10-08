/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <algorithm>
#include <boost/test/unit_test.hpp>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <uecho/_object.h>

#include <uecho/device.h>
#include <uecho/profile.h>

static bool propertymap_has_prop(uEchoPropertyCode* propMapCodes, size_t propMapCount, uEchoPropertyCode propCode)
{
  for (int n = 0; n < propMapCount; n++) {
    if (propMapCodes[n] == propCode) {
      return true;
    }
  }
  return false;
}

BOOST_AUTO_TEST_CASE(PropertyMap)
{

  uEchoObjectCode objCodes[] = {
    0x03CE,
  };

  for (int i = 0; i < sizeof(objCodes) / sizeof(objCodes[0]); i++) {
    uEchoObjectCode objCode = objCodes[i];
    uEchoObject* obj = uecho_device_new();
    uecho_object_setcode(obj, objCode);

    uEchoPropertyCode propCodes[] = {
      uEchoObjectGetPropertyMap,
      uEchoObjectSetPropertyMap,
      uEchoObjectAnnoPropertyMap,
    };

    for (int j = 0; j < sizeof(propCodes) / sizeof(propCodes[0]); j++) {
      uEchoPropertyCode propCode = propCodes[i];

      size_t expectedPropMapCount = 0;
      for (uEchoProperty* prop = uecho_object_getproperties(obj); prop; prop = uecho_property_next(prop)) {
        switch (propCode) {
        case uEchoObjectGetPropertyMap: {
          if (uecho_property_isreadable(prop))
            expectedPropMapCount++;
        } break;
        case uEchoObjectSetPropertyMap: {
          if (uecho_property_iswritable(prop))
            expectedPropMapCount++;
        } break;
        case uEchoObjectAnnoPropertyMap: {
          if (uecho_property_isannounceable(prop))
            expectedPropMapCount++;
        } break;
        }
      }

      uEchoProperty* prop = uecho_object_getproperty(obj, propCode);
      BOOST_REQUIRE(prop);

      size_t propMapCount = 0;
      uecho_property_getpropertymapcount(prop, &propMapCount);
      BOOST_REQUIRE_EQUAL(propMapCount, expectedPropMapCount);

      uEchoPropertyCode* propMapCodes = (uEchoPropertyCode*)malloc(propMapCount);
      BOOST_REQUIRE(uecho_property_getpropertymapcodes(prop, propMapCodes, propMapCount));

      for (uEchoProperty* prop = uecho_object_getproperties(obj); prop; prop = uecho_property_next(prop)) {
        switch (propCode) {
        case uEchoObjectGetPropertyMap: {
          if (uecho_property_isreadable(prop)) {
            BOOST_REQUIRE(propertymap_has_prop(propMapCodes, propMapCount, propCode));
          }
        } break;
        case uEchoObjectSetPropertyMap: {
          if (uecho_property_iswritable(prop))
            BOOST_REQUIRE(propertymap_has_prop(propMapCodes, propMapCount, propCode));
        } break;
        case uEchoObjectAnnoPropertyMap: {
          if (uecho_property_isannounceable(prop))
            BOOST_REQUIRE(propertymap_has_prop(propMapCodes, propMapCount, propCode));
        } break;
        }
      }

      free(propMapCodes);
    }

    uecho_object_delete(obj);
  }
}

BOOST_AUTO_TEST_CASE(PropertyMapInvalidOutputs)
{
  uEchoProperty* property = uecho_property_new();
  BOOST_REQUIRE(property);
  size_t count = 0;
  uEchoPropertyCode code = 0;
  byte value = 0;
  BOOST_CHECK(!uecho_property_getpropertymapcount(nullptr, &count));
  BOOST_CHECK(!uecho_property_getpropertymapcount(property, nullptr));
  BOOST_CHECK(!uecho_property_getpropertymapcodes(nullptr, &code, 1));
  BOOST_CHECK(!uecho_property_getpropertymapcodes(property, nullptr, 1));
  BOOST_CHECK(!uecho_property_getbytedata(property, &value));
  uecho_property_delete(property);
}

BOOST_AUTO_TEST_CASE(PropertyMapFormat2HighCodes)
{
  // Format 2 stores EPC 0x80 + row + 0x10 * bit; bits 4-7 carry EPCs 0xC0-0xFF.
  uEchoObject* obj = uecho_device_new();
  BOOST_REQUIRE(obj);
  uecho_object_setcode(obj, 0x029101);
  BOOST_REQUIRE(uecho_object_setproperty(obj, 0xC0, uEchoPropertyAttrRead));
  BOOST_REQUIRE(uecho_object_setproperty(obj, 0xFF, uEchoPropertyAttrRead));

  size_t readableCount = 0;
  for (uEchoProperty* prop = uecho_object_getproperties(obj); prop; prop = uecho_property_next(prop)) {
    if (uecho_property_isreadable(prop))
      readableCount++;
  }
  BOOST_REQUIRE_GE(readableCount, (size_t)16);

  uEchoProperty* map = uecho_object_getproperty(obj, uEchoObjectGetPropertyMap);
  BOOST_REQUIRE(map);
  BOOST_REQUIRE_EQUAL(uecho_property_getdatasize(map), (size_t)uEchoPropertyMapFormat2Size);
  const byte* data = uecho_property_getdata(map);
  BOOST_CHECK_EQUAL((size_t)data[0], readableCount);
  BOOST_CHECK(data[1] & 0x10); // 0xC0: row 0, bit 4
  BOOST_CHECK(data[16] & 0x80); // 0xFF: row 15, bit 7

  size_t count = 0;
  BOOST_REQUIRE(uecho_property_getpropertymapcount(map, &count));
  BOOST_REQUIRE_EQUAL(count, readableCount);
  std::vector<uEchoPropertyCode> codes(count);
  BOOST_REQUIRE(uecho_property_getpropertymapcodes(map, codes.data(), count));
  BOOST_CHECK(propertymap_has_prop(codes.data(), count, 0xC0));
  BOOST_CHECK(propertymap_has_prop(codes.data(), count, 0xFF));
  for (uEchoProperty* prop = uecho_object_getproperties(obj); prop; prop = uecho_property_next(prop)) {
    if (uecho_property_isreadable(prop))
      BOOST_CHECK(propertymap_has_prop(codes.data(), count, uecho_property_getcode(prop)));
  }

  uecho_object_delete(obj);
}

BOOST_AUTO_TEST_CASE(PropertyMapCountBoundaries)
{
  struct PropertyMapCase {
    const char* name;
    std::vector<byte> data;
    std::vector<uEchoPropertyCode> codes;
  };
  auto codeRange = [](int first, int last) {
    std::vector<uEchoPropertyCode> codes;
    for (int code = first; code <= last; code++)
      codes.push_back(static_cast<uEchoPropertyCode>(code));
    return codes;
  };
  std::vector<byte> map16(17, 0x10);
  map16[0] = 16;
  std::vector<byte> map64(17, 0xF0);
  map64[0] = 64;
  std::vector<byte> map128(17, 0xFF);
  map128[0] = 128;
  auto mixedCodes = codeRange(0x80, 0x8F);
  mixedCodes.push_back(0xFF);
  const std::vector<PropertyMapCase> cases = {
    { "empty", { 0 }, { } },
    { "one", { 1, 0xFF }, { 0xFF } },
    { "format1_15", { 15, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE }, codeRange(0xC0, 0xCE) },
    { "format2_16", map16, codeRange(0xC0, 0xCF) },
    { "format2_17", { 17, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0x81 }, mixedCodes },
    { "all_upper", map64, codeRange(0xC0, 0xFF) },
    { "all_epcs", map128, codeRange(0x80, 0xFF) },
  };
  for (auto mapCode : { uEchoObjectGetPropertyMap, uEchoObjectSetPropertyMap, uEchoObjectAnnoPropertyMap }) {
    for (const auto& testCase : cases) {
      BOOST_TEST_CONTEXT(testCase.name << " map=" << mapCode)
      {
        // Decode known external bytes independently of the generator.
        std::unique_ptr<uEchoProperty, decltype(&uecho_property_delete)> property(uecho_property_new(), uecho_property_delete);
        BOOST_REQUIRE(property);
        uecho_property_setcode(property.get(), mapCode);
        BOOST_REQUIRE(uecho_property_setdata(property.get(), testCase.data.data(), testCase.data.size()));
        size_t count = 0;
        BOOST_REQUIRE(uecho_property_getpropertymapcount(property.get(), &count));
        BOOST_REQUIRE_EQUAL(count, testCase.codes.size());
        std::vector<uEchoPropertyCode> decoded(count, 0x55);
        BOOST_REQUIRE(uecho_property_getpropertymapcodes(property.get(), decoded.data(), count));
        std::sort(decoded.begin(), decoded.end());
        BOOST_CHECK_EQUAL_COLLECTIONS(decoded.begin(), decoded.end(), testCase.codes.begin(), testCase.codes.end());

        // Verify exact generated bytes and then decode the generated map.
        std::unique_ptr<uEchoObject, decltype(&uecho_object_delete)> object(uecho_object_new(), uecho_object_delete);
        BOOST_REQUIRE(object);
        byte emptyCode = 0;
        BOOST_REQUIRE(uecho_object_setpropertymap(object.get(), mapCode, &emptyCode, 0));
        auto codes = testCase.codes;
        BOOST_REQUIRE(uecho_object_setpropertymap(object.get(), mapCode, codes.empty() ? &emptyCode : codes.data(), codes.size()));
        auto* map = uecho_object_getproperty(object.get(), mapCode);
        BOOST_REQUIRE(map);
        BOOST_REQUIRE_EQUAL(uecho_property_getdatasize(map), testCase.data.size());
        const byte* data = uecho_property_getdata(map);
        BOOST_CHECK_EQUAL_COLLECTIONS(data, data + testCase.data.size(), testCase.data.begin(), testCase.data.end());
        decoded.assign(count, 0x55);
        BOOST_REQUIRE(uecho_property_getpropertymapcodes(map, decoded.data(), count));
        std::sort(decoded.begin(), decoded.end());
        BOOST_CHECK_EQUAL_COLLECTIONS(decoded.begin(), decoded.end(), testCase.codes.begin(), testCase.codes.end());
      }
    }
  }
}

BOOST_AUTO_TEST_CASE(PropertyMapFormat2CountMismatch)
{
  // Values are the declared count and the actual bitmap population count.
  for (auto counts : { std::pair<size_t, size_t> { 16, 0 }, { 16, 1 }, { 16, 15 }, { 16, 17 }, { 17, 16 }, { 64, 16 }, { 128, 127 }, { 129, 128 } }) {
    for (auto mapCode : { uEchoObjectGetPropertyMap, uEchoObjectSetPropertyMap, uEchoObjectAnnoPropertyMap }) {
      BOOST_TEST_CONTEXT("declared=" << counts.first << " actual=" << counts.second << " map=" << mapCode)
      {
        std::unique_ptr<uEchoProperty, decltype(&uecho_property_delete)> property(uecho_property_new(), uecho_property_delete);
        BOOST_REQUIRE(property);
        uecho_property_setcode(property.get(), mapCode);
        std::vector<byte> data(uEchoPropertyMapFormat2Size, 0);
        data[0] = static_cast<byte>(counts.first);
        for (size_t bit = 0; bit < counts.second; bit++)
          data[1 + bit / 8] |= static_cast<byte>(1 << (bit % 8));
        BOOST_REQUIRE(uecho_property_setdata(property.get(), data.data(), data.size()));
        std::vector<uEchoPropertyCode> codes(counts.first, 0x55);
        BOOST_CHECK(!uecho_property_getpropertymapcodes(property.get(), codes.data(), codes.size()));
      }
    }
  }
}
