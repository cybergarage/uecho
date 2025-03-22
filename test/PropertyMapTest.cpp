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
