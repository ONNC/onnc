//===- EditDistance.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_EDIT_DISTANCE_H
#define ONNC_ADT_EDIT_DISTANCE_H
#include <algorithm>
#include <vector>

namespace onnc {

// The algorithm implemented below is the "classic"
// dynamic-programming algorithm for computing the Levenshtein
// distance, which is described here:
//
//   http://en.wikipedia.org/wiki/Levenshtein_distance
//
// Although the algorithm is typically described using an m x n
// array, only two rows are used at a time, so this implementation
// just keeps two separate vectors for those two rows.
inline unsigned int
EditDistance(StringRef pFromArray, StringRef pToArray,
             bool pAllowReplacements = true)
{
  StringRef::size_type m = pFromArray.size();
  StringRef::size_type n = pToArray.size();

  unsigned int* previous = new unsigned int [2*(n+1)];
  unsigned int* current = previous + (n + 1);
  unsigned int* save_for_delete = previous;

  // initialize
  for (unsigned int i = 0; i <= n; ++i)
    previous[i] = i;

  for (StringRef::size_type y = 1; y <= m; ++y) {
    current[0] = y;
    unsigned int best_this_row = current[0];

    for (StringRef::size_type x = 1; x <= n; ++x) {
      if (pAllowReplacements) {
        current[x] = std::min(
            previous[x-1] + (pFromArray[y-1] == pToArray[x-1] ? 0u : 1u),
            std::min(current[x-1], previous[x])+1);
      }
      else {
        if (pFromArray[y-1] == pToArray[x-1])
          current[x] = previous[x-1];
        else
          current[x] = std::min(current[x-1], previous[x]) + 1;
      }
      best_this_row = std::min(best_this_row, current[x]);
    } // end of for

    // swap
    std::swap(current, previous);
  } // end of for

  unsigned int result = previous[n];
  delete save_for_delete;
  return result;
}

} // namespace of onnc

#endif
