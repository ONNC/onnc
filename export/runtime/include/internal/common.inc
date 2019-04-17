int32_t findIndex(int32_t index, int32_t axisDim, int32_t elementDistance)
{
  return (index / (axisDim * elementDistance)) * elementDistance + index % elementDistance;
}

void calculate_axis_dis(int32_t ndim, const int32_t* restrict dims, int32_t* restrict axis_dis)
{
  int32_t base = axis_dis[ndim - 1] = 1;
  for (int32_t dim = ndim - 2; dim >= 0; --dim) {
    axis_dis[dim] = base * dims[dim + 1];
    base          = axis_dis[dim];
  }
}

bool next_dim_no_check(int32_t ndim, int32_t* restrict dim, const int32_t* restrict dim_max)
{
  do {
    ndim = ndim - 1;
    dim[ndim] += 1;
    if (dim[ndim] < dim_max[ndim]) {
      return true;
    } else {           // reach dimension max
      if (ndim == 0) { // all dimension done
        return false;
      }
      dim[ndim] = 0;
    }
  } while (true);
}

bool next_dim(int32_t ndim, int32_t* restrict dim, const int32_t* restrict dim_max)
{
  if (ndim == 0)
    return false;
  return next_dim_no_check(ndim, dim, dim_max);
}

int64_t dim_to_offset(int32_t ndim, const int32_t* restrict dim, const int32_t* restrict axisDistance)
{
  if (ndim == 0)
    return 0;
  int64_t offset = 0;
  for (int32_t i = ndim - 1; i >= 0; --i) {
    offset += dim[i] * axisDistance[i];
  }
  return offset;
}

int64_t dim_to_offset_with_step(int32_t ndim, const int32_t* restrict dim, const int32_t* restrict dim_max)
{
  int64_t offset = 0;
  int64_t step   = 1;
  for (int32_t i = ndim - 1; i >= 0; --i) {
    offset += dim[i] * step;
    step *= dim_max[i];
  }
  return offset;
}

float get_value_or_zero(int32_t ndim, const int32_t* restrict dim_max, const float* restrict value,
                                      const int32_t* restrict dim)
{
  for (int32_t i = 0; i < ndim; ++i) {
    if (dim[i] < 0 || dim[i] >= dim_max[i]) {
      return 0.f;
    }
  }
  return value[dim_to_offset_with_step(ndim, dim, dim_max)];
}

float get_value_or_zero_5(int32_t ndim, const int32_t* restrict dim_max, const float* restrict value,
                                        const int32_t* restrict dim, int32_t* isPad)
{
  for (int32_t i = 0; i < ndim; ++i) {
    if (dim[i] < 0 || dim[i] >= dim_max[i]) {
			if (isPad) {
				*isPad = 1;
			}
      return 0.f;
    }
  }
	if (isPad) {
		*isPad = 0;
  }
  return value[dim_to_offset_with_step(ndim, dim, dim_max)];
}

int32_t con(int32_t* meofarr, int32_t ndim, const int32_t* dims)
{
  int32_t mul = 1, res = 0;
  for (int32_t i = ndim - 1; i >= 0; --i) {
    res += meofarr[i] * mul;
    mul *= dims[i];
  }
  return res;
}

float randomNormal(float mean, float stddev)
{
  float x = (float)rand() / (RAND_MAX + 1.0f);
  float y = (float)rand() / (RAND_MAX + 1.0f);
  return mean + stddev * sqrtf(-2.0f * logf(x)) * cosf(2.0f * acosf(-1.0f) * y);
}
