/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <boost/test/unit_test.hpp>
#include <string>
#include <vector>

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
