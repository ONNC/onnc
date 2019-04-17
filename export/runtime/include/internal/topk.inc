/* node */
struct node
{
  float   value;
  int32_t index;
};

void init(struct node* restrict heap, int32_t input_index, float value, int32_t index)
{
  heap[input_index].value = value;
  heap[input_index].index = index;
}

void swap(struct node* a, struct node* b)
{
  struct node tmp = *a;
  *a              = *b;
  *b              = tmp;
}

/* node comparison */
bool smaller(struct node a, struct node b)
{
  if (a.value < b.value)
    return true;
  if (a.value > b.value)
    return false;
  if (a.index > b.index)
    return true;
  return false;
}

/* up_heap */
void up_heap(struct node* restrict heap, int32_t index)
{
  if (index == 1)
    return;
  if (smaller(heap[index], heap[index / 2])) {
    swap(&heap[index / 2], &heap[index]);
    up_heap(heap, index / 2);
    return;
  }
}

/* down_heap */
void down_heap(struct node* restrict heap, int32_t index, int32_t inside_num)
{
  if (index >= inside_num)
    return;

  int32_t right_index = 2 * index + 1;
  int32_t left_index  = 2 * index;
  int32_t value       = heap[index].value;

  if (left_index > inside_num)
    return;
  if (right_index > inside_num && smaller(heap[left_index], heap[index])) {
    swap(&heap[left_index], &heap[index]);
    down_heap(heap, left_index, inside_num);
    return;
  }

  if (right_index <= inside_num && smaller(heap[left_index], heap[index]) &&
      smaller(heap[left_index], heap[right_index])) {
    swap(&heap[left_index], &heap[index]);
    down_heap(heap, left_index, inside_num);
    return;
  }

  if (right_index <= inside_num && smaller(heap[right_index], heap[index])) {
    swap(&heap[right_index], &heap[index]);
    down_heap(heap, right_index, inside_num);
    return;
  }
  return;
}

/* insert a node */
void insert_node(struct node* restrict heap, int32_t inside_num, int32_t value, int32_t index)
{
  init(heap, inside_num + 1, value, index);
  up_heap(heap, inside_num + 1);
}

void delete_node(struct node* restrict heap, int32_t inside_num)
{
  init(heap, 1, FLT_MAX, 0);
  swap(&heap[1], &heap[inside_num]);
  down_heap(heap, 1, inside_num - 1);
}

#include "topk.impl.inc"
