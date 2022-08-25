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

static bool propertymap_has_prop(uEchoPropertyCode* prop_map_codes, size_t prop_map_count, uEchoPropertyCode prop_code)
{
  for (int n = 0; n < prop_map_count; n++) {
    if (prop_map_codes[n] == prop_code) {
      return true;
    }
  }
  return false;
}

BOOST_AUTO_TEST_CASE(PropertyMap)
{

  uEchoObjectCode obj_codes[] = {
    0x03CE,
  };

  for (int i = 0; i < sizeof(obj_codes) / sizeof(obj_codes[0]); i++) {
    uEchoObjectCode obj_code = obj_codes[i];
    uEchoObject* obj = uecho_device_new();
    uecho_object_setcode(obj, obj_code);

    uEchoPropertyCode prop_codes[] = {
      uEchoObjectGetPropertyMap,
      uEchoObjectSetPropertyMap,
      uEchoObjectAnnoPropertyMap,
    };

    for (int j = 0; j < sizeof(prop_codes) / sizeof(prop_codes[0]); j++) {
      uEchoPropertyCode prop_code = prop_codes[i];

      size_t expectedPropMapCount = 0;
      for (uEchoProperty* prop = uecho_object_getproperties(obj); prop; prop = uecho_property_next(prop)) {
        switch (prop_code) {
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

      uEchoProperty* prop = uecho_object_getproperty(obj, prop_code);
      BOOST_CHECK(prop);

      size_t prop_map_count = 0;
      uecho_property_getpropertymapcount(prop, &prop_map_count);
      BOOST_CHECK_EQUAL(prop_map_count, expectedPropMapCount);

      uEchoPropertyCode* prop_map_codes = (uEchoPropertyCode*)malloc(prop_map_count);
      BOOST_CHECK(uecho_property_getpropertymapcodes(prop, prop_map_codes, prop_map_count));

      for (uEchoProperty* prop = uecho_object_getproperties(obj); prop; prop = uecho_property_next(prop)) {
        switch (prop_code) {
        case uEchoObjectGetPropertyMap: {
          if (uecho_property_isreadable(prop)) {
            BOOST_CHECK(propertymap_has_prop(prop_map_codes, prop_map_count, prop_code));
          }
        } break;
        case uEchoObjectSetPropertyMap: {
          if (uecho_property_iswritable(prop))
            BOOST_CHECK(propertymap_has_prop(prop_map_codes, prop_map_count, prop_code));
        } break;
        case uEchoObjectAnnoPropertyMap: {
          if (uecho_property_isannounceable(prop))
            BOOST_CHECK(propertymap_has_prop(prop_map_codes, prop_map_count, prop_code));
        } break;
        }
      }

      free(prop_map_codes);
    }

    uecho_object_delete(obj);
  }
}
