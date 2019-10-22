#include <onnc-runtime.h>

#include <assert.h>

bool ONNC_RUNTIME_has_tensor(const struct ONNC_RUNTIME_tensor_offset_table* table, uint64_t tensor)
{
  if (table == NULL) {
    return false;
  }

  return tensor < table->number_of_tensors;
}

struct ONNC_RUNTIME_tensor_offset ONNC_RUNTIME_get_tensor_offset(const struct ONNC_RUNTIME_tensor_offset_table* table,
                                                                 uint64_t                                       tensor)
{
  assert(ONNC_RUNTIME_has_tensor(table, tensor));

  return table->tensor_offsets[tensor];
}
