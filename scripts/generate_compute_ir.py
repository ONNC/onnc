#!/usr/bin/env python
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

import os
import sys
from collections import defaultdict
from string import Template

from onnx import defs
from onnx.defs import OpSchema

ONNX_ML = bool(os.getenv('ONNX_ML') == '1')
def should_render_domain(domain):
  if domain == 'ai.onnx.ml' and not ONNX_ML:
    return False
  elif ONNX_ML and domain != 'ai.onnx.ml':
    return False
  return True

def to_camel_case(snake_str, init_upper = True):
    components = snake_str.split('_')
    if init_upper:
        components[0] = ''.join([components[0][0].upper(), components[0][1:]])
    return components[0] + ''.join(''.join([x[0].upper(), x[1:]]) for x in components[1:])

def format_attr_type(attr_type):
    return to_camel_case(str(attr_type).rsplit('.', 1)[-1].lower())+'Attr'

def gen_compute_ir_substitution_hash(schema):
  hash = {
    'OperatorName': schema.name,
    'OPERATORNAME': schema.name.upper(),
  }

  # ========== Input Output ==============
  def for_io_schema(io_schemas, cb):
    def transform_io_schema(idx, io_schema):
      return {
        'idx': idx,
        'io_name': to_camel_case(io_schema.name),
        'option': io_schema.option,
      }
    return [cb(transform_io_schema(idx, io_schema)) for idx, io_schema in enumerate(io_schemas)]

  # ${IOConst}
  def io_const(io_schema):
    return 'k{io_name} = {idx}'.format(**io_schema)
  hash['IOConst'] = ',\n    '.join(for_io_schema(schema.inputs, io_const) + for_io_schema(schema.outputs, io_const))

  def io_getter(prefix):
    def io_getter_cb(io_schema):
      if OpSchema.FormalParameterOption.Variadic == io_schema['option']:
        return 'Tensor* get{io_name}(size_t pIdx) {{ return get{prefix}(k{io_name} + pIdx); }}\n'.format(prefix=prefix, **io_schema)
      else:
        return 'Tensor* get{io_name}() {{ return get{prefix}(k{io_name}); }}\n'.format(prefix=prefix, **io_schema)
    return io_getter_cb
  # ${InputsGetters}
  hash['InputsGetters'] = '\n  '.join(for_io_schema(schema.inputs, io_getter('Input')))
  # ${OutputsGetters}
  hash['OutputsGetters'] = '\n  '.join(for_io_schema(schema.outputs, io_getter('Output')))

  def io_setter(prefix):
    def io_setter_cb(io_schema):
      if OpSchema.FormalParameterOption.Variadic == io_schema['option']:
        return 'void set{io_name}(size_t pIdx, Tensor& pTensor) {{ m_{prefix}s[k{io_name} + pIdx] = &pTensor; }}\n'.format(prefix=prefix, **io_schema)
      else:
        return 'void set{io_name}(Tensor& pTensor) {{ m_{prefix}s[k{io_name}] = &pTensor; }}\n'.format(prefix = prefix, **io_schema)
    return io_setter_cb
  # ${InputsSetters}
  hash['InputsSetters'] = '\n  '.join(for_io_schema(schema.inputs, io_setter('Input')))
  # ${OutputsSetters}
  hash['OutputsSetters'] = '\n  '.join(for_io_schema(schema.outputs, io_setter('Output')))
  # ========== end of Input Output ==============

  # ========== Attributes ==============
  attrs = []
  if schema.attributes:
    attrs = [attr for _, attr in sorted(schema.attributes.items())]
  def for_attr(cb):
    def transform_attr(attr):
      return {
        'attr_type': format_attr_type(attr.type),
        'attr_name': to_camel_case(attr.name),
      }
    return [cb(transform_attr(attr)) for attr in attrs]

  if schema.attributes:
    # ${AttributesGetters}
    hash['AttributesGetters'] = '\n  '.join(for_attr(lambda attr:
      'const {attr_type}& get{attr_name}() const {{ return m_{attr_name}; }}\n'.format(**attr)
    ))

    # ${AttributesMemberVariables}
    hash['AttributesMemberVariables'] = '\n  '.join(for_attr(lambda attr:
      '{attr_type} m_{attr_name};'.format(**attr)
    ))

    indent = ' ' * len(hash['OperatorName'])
    attrs_params = for_attr(lambda attr: 'const {attr_type}& p{attr_name}'.format(**attr))

    # ${ConstructorByAttributes}
    hash['ConstructorByAttributes'] = '{OperatorName}({attrs_params});\n'.format(
      attrs_params=(',\n   ' + indent).join(attrs_params),
      **hash
    )

    # ${ConstructorByAttributesImplementation}
    hash['ConstructorByAttributesImplementation'] = '''{OperatorName}::{OperatorName}({attrs_params})
  : ComputeOperator("{OperatorName}"),
    {attrs_call_constructor} {{
}}'''.format(
      attrs_params = (',\n   ' + (indent * 2)).join(attrs_params),
      attrs_call_constructor = (',\n    ').join(for_attr(lambda attr: 'm_{attr_name}(p{attr_name})'.format(**attr))),
      **hash
    )

    # ${CallAttributesDefaultConstructor}
    hash['CallAttributesDefaultConstructor'] = ',\n    ' + (',\n    ').join(for_attr(lambda attr: 'm_{attr_name}()'.format(**attr)))

    # ${CallAttributesCopyConstructor}
    hash['CallAttributesCopyConstructor'] = ',\n    ' + (',\n    ').join(for_attr(lambda attr: 'm_{attr_name}(pCopy.get{attr_name}())'.format(**attr)))

    # ${PrintAttributes}
    hash['PrintAttributes'] = ' << "< "' + (' << ", "').join(for_attr(lambda attr: ' << get{attr_name}()'.format(**attr))) + ' << ">"'
  else:
    hash['AttributesGetters'] = ''
    hash['AttributesMemberVariables'] = ''
    hash['ConstructorByAttributes'] = ''
    hash['ConstructorByAttributesImplementation'] = ''
    hash['CallAttributesDefaultConstructor'] = ''
    hash['CallAttributesCopyConstructor'] = ''
    hash['PrintAttributes'] = ''
  # ========== end of Attributes ==============

  return hash

def gen_compute_ir(operator_schemas, template_filename, dist):
  # TODO: Refactor to simple data structure and simple for loop
  for domain, supportmap in operator_schemas:
    for _, namemap in supportmap:
      for op_type, schema, versions in namemap:
        substitution_hash = gen_compute_ir_substitution_hash(schema)

        template_file = open(template_filename)
        template_str = template_file.read()
        out_file = open(Template(dist).substitute(substitution_hash), 'w')
        out_file.write(Template(template_str).substitute(substitution_hash))
        out_file.close()
        template_file.close()


def gen_compute_ir_visitor(operator_schemas, template_filename, dist):
  forward_declaration = []
  visitor_member_functions = []
  # TODO: Refactor to simple data structure and simple for loop
  for domain, supportmap in operator_schemas:
    for _, namemap in supportmap:
      for op_type, schema, versions in namemap:
        substitution_hash = gen_compute_ir_substitution_hash(schema)
        # ${forward_declaration}
        forward_declaration.append('class {OperatorName};'.format(**substitution_hash))
        # ${visitor_member_functions}
        visitor_member_functions.append('virtual void visit({OperatorName}& p{OperatorName}) {{ }}'.format(**substitution_hash))

  visitor_substitution_hash = {
    'forward_declaration': '\n'.join(forward_declaration),
    'visitor_member_functions': '\n  '.join(visitor_member_functions),
  }

  template_file = open(template_filename)
  template_str = template_file.read()
  out_file = open(dist, 'w')
  out_file.write(Template(template_str).substitute(visitor_substitution_hash))
  out_file.close()
  template_file.close()
  return

if __name__ == '__main__':
  # domain -> support level -> name -> [schema]
  index = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))  # type: Dict[Text, Dict[int, Dict[Text, List[OpSchema]]]]
  for schema in defs.get_all_schemas_with_history():
    index[schema.domain][int(schema.support_level)][schema.name].append(schema)

  # Preprocess the Operator Schemas
  # [(domain, [(support_level, [(schema name, current schema, all versions schemas)])])]
  operator_schemas = list()  # type: List[Tuple[Text, List[Tuple[int, List[Tuple[Text, OpSchema, List[OpSchema]]]]]]]
  exsting_ops = set()  # type: Set[Text]
  for domain, _supportmap in sorted(index.items()):
    if not should_render_domain(domain):
      continue

    processed_supportmap = list()
    for _support, _namemap in sorted(_supportmap.items()):
      processed_namemap = list()
      for n, unsorted_versions in sorted(_namemap.items()):
        versions = sorted(unsorted_versions, key=lambda s: s.since_version)
        schema = versions[-1]
        if schema.name in exsting_ops:
          continue
        exsting_ops.add(schema.name)
        processed_namemap.append((n, schema, versions))
      processed_supportmap.append((_support, processed_namemap))
    operator_schemas.append((domain, processed_supportmap))

  # FIXME: Path
  if not os.path.exists("ComputeIR.h"):
    os.makedirs("ComputeIR.h")
  gen_compute_ir(operator_schemas, 'ComputeIRTemplate.h', 'ComputeIR.h/${OperatorName}.h')
  if not os.path.exists("ComputeIR.cpp"):
    os.makedirs("ComputeIR.cpp")
  gen_compute_ir(operator_schemas, 'ComputeIRTemplate.cpp', 'ComputeIR.cpp/${OperatorName}.cpp')
  # TODO: better template engine
  gen_compute_ir_visitor(operator_schemas, 'ComputeVisitorTemplate.h', 'ComputeVisitor.h')
