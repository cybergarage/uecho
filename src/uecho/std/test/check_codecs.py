#!/usr/bin/env python3
"""Exercise generated codec constraints without depending on the full MRA."""
import json
import os
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[4]
GENERATOR = ROOT / 'src/uecho/std/objects_mra.pl'


def run(*args, **kwargs):
    return subprocess.run(args, check=True, **kwargs)


def state(code='0xFD'):
    return {'type': 'state', 'size': 1, 'enum': [{'edt': code, 'name': 'undefined', 'readOnly': True}]}


number = {'type': 'number', 'format': 'uint8', 'minimum': 0, 'maximum': 50}
ref = {'$ref': '#/definitions/a~1b~0c'}
schemas = [
    ref,  # JSON pointer escapes resolve to a/b~c
    {'oneOf': [number, state()]},
    {'oneOf': [number, state('0x20')]},  # overlap
    {'oneOf': [number, number]},
    {'oneOf': [number, state(), state()]},  # duplicate sentinel
    {'oneOf': [number, {'type': 'state', 'size': 2, 'enum': [{'edt': '0xFFFF', 'name': 'unknown'}]}]},
    {'$ref': '#/definitions/a~1b~0c', 'maximum': 10},  # siblings not guessed
    {**number, 'multipleOf': 2},
    {'type': 'future'},
    {**number, 'maximum': 256},
    {**number, 'coefficient': ['0xD3']},
    {**number, 'overflowCode': True},
    {'type': 'state', 'size': 1, 'enum': [{'edt': '0x01...0x03', 'name': 'range'}]},
]

with tempfile.TemporaryDirectory() as directory:
    work = Path(directory)
    mra = work / 'fixture/mraData'
    for name in ['superClass', 'nodeProfile', 'devices', 'definitions']:
        (mra / name).mkdir(parents=True)
    definitions = mra / 'definitions/definitions.json'
    definitions.write_text(json.dumps({'definitions': {'a/b~c': number}}))
    for folder, eoj in [('superClass', '0000'), ('nodeProfile', '0EF0'), ('devices', '0130')]:
        props = [] if folder != 'devices' else [
            {'epc': f'0x{0xB0 + i:02X}', 'propertyName': {'en': 'fixture'},
             'accessRule': dict.fromkeys(['get', 'set', 'inf'], 'optional'), 'data': schema}
            for i, schema in enumerate(schemas)]
        (mra / folder / f'{eoj}.json').write_text(json.dumps({'eoj': '0x' + eoj, 'className': {'en': 'fixture'}, 'elProperties': props}))
    generated = work / 'generated.c'
    with generated.open('w') as output:
        run('perl', str(GENERATOR), str(mra.parent), stdout=output)
    check = work / 'check.c'
    check.write_text('''#include <assert.h>
#include <uecho/std/_standard.h>
int main(void) {
  const uEchoStdObject* obj = 0;
  for (size_t i = 0; i < uecho_std_objectcount; ++i)
    if (uecho_std_objects[i].grpCode == 1) obj = &uecho_std_objects[i];
  assert(obj && obj->propCnt == 13);
  assert(obj->props[0].valueConstraint && obj->props[0].valueConstraint->maximum == 50);
  assert(obj->props[1].valueConstraint && obj->props[1].valueConstraint->states[0].code == 253);
  for (size_t i = 2; i < obj->propCnt; ++i) assert(!obj->props[i].valueConstraint);
  return 0;
}
''')
    run(os.environ.get('CC', 'cc'), '-Wall', '-Werror', '-DUECHO_DATABASE_CODEC_METADATA=1',
        '-I' + str(ROOT / 'src'), '-I' + str(ROOT / 'include'), str(check), str(generated), '-o', str(work / 'check'))
    run(str(work / 'check'))
    for name, dictionary in [('missing', {}), ('cycle', {'a/b~c': ref}), ('bad escape', {'a/b~c': {'$ref': '#/definitions/a~2b'}})]:
        definitions.write_text(json.dumps({'definitions': dictionary}))
        result = subprocess.run(['perl', str(GENERATOR), str(mra.parent)], stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
        assert result.returncode != 0, name
        print('ok: rejects', name)
print('all codec generator checks passed')
