#pragma once

#include "dictool/MultiDict.h"
#include <map>
#include <vector>
namespace dictool {

/******************************************************************************
 * @brief   Reverse a map-like container by swapping keys and values.
 *          Each original value maps to a vector of keys that shared it.
 *
 * @tparam  K           Type of the keys in the input map
 * @tparam  V           Type of the values in the input map
 * @tparam  DTemplate   Map-like associative container template
 *                      (defaults to std::map)
 *
 * @param   inputs      Input map-like container (mapping keys to values)
 *
 * @return  DTemplate<V, std::vector<K>>
 *          A map-like container where:
 *              - each original value becomes a key
 *              - the associated value is a vector of all original keys
 ******************************************************************************/
template <typename K, typename V,
          template <typename...> class DTemplate = std::map>
DTemplate<V, std::vector<K>> reverse(DTemplate<K, V> inputs) {
  using SelfDict = MultiMap<V, K, std::vector, DTemplate>;
  SelfDict res;
  for (const auto &[key, value] : inputs) {
    res.emplace(value, key);
  }
  return res.cdata();
}
} // namespace dictool