#include <onnc-runtime.h>

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

struct ONNC_RUNTIME_tensor_file* open_tensor_file(const char* filename)
{
  FILE* const stream = fopen(filename, "rb");
  if (stream == NULL) {
    return NULL;
  }

  fseek(stream, 0, SEEK_END);
  const long file_size = ftell(stream);

  struct ONNC_RUNTIME_tensor_file* file = calloc(1, sizeof(struct ONNC_RUNTIME_tensor_file));
  file->data                            = malloc(file_size);

  fseek(stream, 0, SEEK_SET);
  fread(file->data, 1, file_size, stream);

  fclose(stream);

  return file;
}

int close_tensor_file(struct ONNC_RUNTIME_tensor_file* file)
{
  if (file == NULL) {
    return -1;
  }

  free(file->data);
  free(file);

  return 0;
}

void finish(uint64_t id, struct ONNC_RUNTIME_tensor_view output)
{
  const float* const values = output.data;
  const size_t       count  = output.size / sizeof(float);
  printf("[");
  for (size_t idx = 0; idx < count; ++idx) {
    printf("%f, ", values[idx]);
  }
  printf("]");
}

int main(int argc, char* argv[])
{
  if (argc < 3) {
    fprintf(stderr, "usage: %s foo.input foo.weight\n", argv[0]);
    return EXIT_FAILURE;
  }

  struct ONNC_RUNTIME_tensor_file* const input  = open_tensor_file(argv[1]);
  struct ONNC_RUNTIME_tensor_file* const weight = open_tensor_file(argv[2]);

  struct ONNC_RUNTIME_inference_context context = {.input = input, .weight = weight, .id = 0, .completed = finish};

  model_main(&context);

  close_tensor_file(input);
  close_tensor_file(weight);

  return EXIT_SUCCESS;
}
