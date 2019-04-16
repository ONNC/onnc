#include <onnc-runtime.h>

#include <stdio.h>
#include <stdlib.h>

const struct ONNC_RUNTIME_tensor_offset_table*
ONNC_RUNTIME_read_tensor_offset_table(struct ONNC_RUNTIME_tensor_file* file)
{
  if (file == NULL) {
    return NULL;
  }

  return file->data;
}

struct ONNC_RUNTIME_tensor_view ONNC_RUNTIME_read_tensor(struct ONNC_RUNTIME_tensor_file* file, uint64_t tensor)
{
  if (file == NULL) {
    const struct ONNC_RUNTIME_tensor_view tensor_view = {.data = NULL, .size = 0};
    return tensor_view;
  }

  const struct ONNC_RUNTIME_tensor_offset_table* const table = ONNC_RUNTIME_read_tensor_offset_table(file);
  if (!ONNC_RUNTIME_has_tensor(table, tensor)) {
    const struct ONNC_RUNTIME_tensor_view tensor_view = {.data = NULL, .size = 0};
    return tensor_view;
  }

  const struct ONNC_RUNTIME_tensor_offset tensor_offset = ONNC_RUNTIME_get_tensor_offset(table, tensor);
  const struct ONNC_RUNTIME_tensor_view   tensor_view   = {.data = (char*)file->data + tensor_offset.offset,
                                                       .size = tensor_offset.size};
  return tensor_view;
}
