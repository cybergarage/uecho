# Scalar EDT codec

The C core provides local, allocation-free EDT conversion. No Foundation or JSON
parser is required. The optional generated tables are from the same MRA 1.3.0
source as the existing class metadata; names, access flags and original JSON
schemas are unchanged. `objects_mra.pl` generates both representations.

Enable CMake `-DUECHO_DATABASE_CODEC_METADATA=ON`, compile all translation units
with `UECHO_DATABASE_CODEC_METADATA=1` in Autotools/Xcode, or enable the matching
ESP-IDF Kconfig option. The ObjC framework enables the codec and original JSON
metadata flags in its generated project. The codec flag is independent of the
JSON flag, so embedded builds can retain only typed constraints. Both flags are
off by default for C. `UECHO_DATABASE_NONE` still excludes appliance classes.
Device schemas override superclass constraints; profiles do not inherit appliance
superclass metadata. Unknown classes never fabricate superclass-only metadata.
These flags affect the internal table layout and must be consistent across a build.

## C usage

```c
const uEchoValueConstraint* constraint =
    uecho_standard_getvalueconstraint(0x01, 0x30, 0xB3);
uEchoValue value;
byte edt[] = { 0xFD };
if (uecho_value_decode(constraint, edt, sizeof(edt), &value)) {
  // value.status == uEchoValueSpecial; name == "undefined"; unit == "Celsius".
  // value.raw owns the original bytes, including invalid/unsupported data.
}
byte output[4];
size_t size;
bool encoded = uecho_value_encode(constraint, uEchoValueNumber, 25,
                                 output, sizeof(output), &size);
// encoded is true, output[0] is 25. No network request is sent.
```

The number input is an exact, unscaled wire integer; decoded `number` applies
MRA `multiple` and `integer` preserves the original integer. Bounds apply before
scaling. Conversion from arbitrary floating point input is intentionally absent.
State/special input uses its exact unsigned code. Metadata and strings have
process lifetime; `uEchoValue` owns up to 255 raw bytes and no heap allocation.
`outputSize` reports required capacity; failed encode leaves output unchanged.
Encoding special read-only values is permitted for serialization/round trips;
`readOnly` is retained in results/constraints and is not a write authorization.

Decode distinguishes unsupported codec, invalid length/range, unknown state
code, number, state and a special state in a numeric union. In particular AC
B0 accepts 0x40 (`other`) and 0x41–0x45; B3 accepts 0–50 and 0xFD (`undefined`);
BB accepts signed -127–125 and 0x7E (`unmeasurable`). A device may support only a
subset: the standard schema cannot establish device capabilities or permission.

## HomeKit / Objective-C migration

```objc
CGEchoValue *value = [CGEchoValue decodeData:edt groupCode:0x01
                                 classCode:0x30 epc:0xB3 error:&error];
if (value.status == CGEchoValueNumber) {
    // Presentation uses value.number and value.unit.
} else if (value.status == CGEchoValueSpecial) {
    // Display value.name; retain value.rawData for diagnostics.
}
```

Replace the app's duplicated scalar ref resolution/codec with this call after
receiving raw EDT. ObjC only copies C results to immutable Foundation values;
all constraint interpretation and encode/decode run in C. Screen presentation,
HomeKit characteristic mapping, permission policy and combining protocols stay
in the app. Use existing session snapshots and tri-state maps for capabilities;
failed maps must remain distinct from valid-empty maps. Vendor EPCs and Set-only
properties remain in those maps even when there is no standard codec.

No controller write path is connected to this API. AC writes remain disabled in
the app. Typed write/readback, 1/17-byte installation-location decoding,
conditional access metadata and a session-bound descriptor over the union of
Get/Set/INF maps are follow-up units. Such a descriptor must preserve map failures,
provenance, vendor EPCs and session identity; standard mandatory flags must not be
used as evidence of real device support.

## Supported constraints and explicit limits

Supported: signed/unsigned 8/16/32-bit big-endian numbers with integral bounds,
positive `multiple` and optional unit; exact 1/2/4-byte state enums; pure local
refs; one numeric branch plus same-width, exact, disjoint state branches.

Unsupported constraints return NULL and decode yields `Unsupported` with raw
bytes retained. No partial interpretation is attempted. This includes ref
siblings/overrides, overlapping/duplicate unions, multiple numeric branches,
state ranges, arrays, objects, bitmap, level, numeric enums, unknown types,
`multipleOf`, coefficient-dependent scaling and implicit overflow/underflow
codes. Missing/external/cyclic refs and malformed JSON-pointer escapes reject
generation. Valid `~1` and `~0` escapes resolve at generation time.

Validation: `src/uecho/std/test/check_generators.sh` includes codec fixture tests;
CMake tests named `Value*` exercise actual generated AC metadata, signed bounds,
specials, length failures and round trips. `CGEchoValueTests` verifies the wrapper
without a real device. Use the standard CGEcho scheme; CGEchoSmoke uses a network
node and is not required for codec verification.
