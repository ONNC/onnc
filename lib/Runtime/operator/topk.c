#include <onnc/Runtime/operator/topk.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <float.h>

/* node */
struct node{
    float value;
    int32_t index;
};

static void init(struct node * restrict heap, int32_t input_index, float value, int32_t index){
    heap[input_index].value = value;
    heap[input_index].index = index;
}

static void swap(struct node * a, struct node * b){
    struct node tmp = *a;
    *a = *b;
    *b = tmp;
}

/* node comparison */
static bool smaller(struct node a, struct node b){
	if(a.value < b.value) return true;
	if(a.value > b.value) return false;
	if(a.index > b.index) return true;
	return false;
}

/* up_heap */
static void up_heap(struct node * restrict heap, int32_t index){
    if(index == 1) return;
    if(smaller(heap[index], heap[index/2])){
        swap(&heap[index/2], &heap[index]);
        up_heap(heap, index/2);
        return;
    }
}

/* down_heap */
static void down_heap(struct node * restrict heap, int32_t index, int32_t inside_num){
    if(index >= inside_num) return;

    int32_t right_index = 2 * index + 1;
    int32_t left_index = 2 * index;
    int32_t value = heap[index].value;

    if(left_index > inside_num) return;
	if(right_index > inside_num && smaller(heap[left_index], heap[index])){
        swap(&heap[left_index], &heap[index]);
        down_heap(heap, left_index, inside_num);
		return;
	}

	if(right_index <= inside_num && smaller(heap[left_index], heap[index]) && smaller(heap[left_index], heap[right_index])){
        swap(&heap[left_index], &heap[index]);
        down_heap(heap, left_index, inside_num);
		return;
    }

    if(right_index <= inside_num && smaller(heap[right_index], heap[index])){
        swap(&heap[right_index], &heap[index]);
        down_heap(heap, right_index, inside_num);
		return;
    }
    return;
}

/* insert a node */
static void insert_node(struct node * restrict heap, int32_t inside_num, int32_t value, int32_t index){
    init(heap, inside_num + 1, value, index);
    up_heap(heap, inside_num + 1);
}

static void delete_node(struct node * restrict heap, int32_t inside_num){
    init(heap, 1, FLT_MAX, 0);
    swap(&heap[1], &heap[inside_num]);
    down_heap(heap, 1, inside_num - 1);
}

static inline void calculate_axis_dis(int32_t ndim, const int32_t * restrict dims, int32_t * restrict axis_dis){
    int32_t base = axis_dis[ndim-1] = 1;
    for(int32_t dim = ndim - 2 ; dim >= 0 ; --dim){
        axis_dis[dim] = base * dims[dim+1];
        base = axis_dis[dim];
    }
}

static inline bool next_dim(int32_t ndim, int32_t * restrict dim, const int32_t * restrict dim_max) {
  if(ndim == 0)
    return false;
  do {
    ndim = ndim - 1;
    dim[ndim] += 1;
    if (dim[ndim] < dim_max[ndim]) {
      return true;
    } else { // reach dimension max
      if (ndim == 0) { // all dimension done
        return false;
      }
      dim[ndim] = 0;
    }
  } while(true);
}

static inline int64_t dim_to_offset(int32_t ndim, const int32_t * restrict dim,
                                    const int32_t * restrict axisDistance) {
  if(ndim == 0) return 0;
  int64_t offset = 0;
  for (int32_t i = ndim - 1; i >= 0; --i) {
    offset += dim[i] * axisDistance[i];
  }
  return offset;
}

#include "topk.inc"