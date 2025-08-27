#pragma once

#include "dictool/MultiDict.h"
#include <functional>

namespace dictool {
/******************************************************************************
 * @brief   Reverse a map-like container by swapping keys and values.
 *          Each original value maps to a vector of keys that shared it.
 *
 * @tparam  K           Type of the keys in the input map
 * @tparam  V           Type of the values in the input map
 * @tparam  DictLike   Map-like associative container template
 *                      (defaults to std::map)
 *
 * @param   inputs      Input map-like container (mapping keys to values)
 *
 * @return  DictLike<V, std::vector<K>>
 *          A map-like container where:
 *              - each original value becomes a key
 *              - the associated value is a vector of all original keys
 ******************************************************************************/
template <typename K, typename V,
          template <typename...> class DictLike = std::map>
DictLike<V, std::vector<K>> reverse(const DictLike<K, V> &inputs) {
  using SelfDict = MultiMap<V, K, std::vector, DictLike>;
  SelfDict res;
  for (const auto &[key, value] : inputs) {
    res.emplace(value, key);
  }
  return res.cdata();
}

/******************************************************************************
 * @brief   Apply a transformation function to each (key, value) pair in a map,
 *          producing a new dictionary. The input dictionary is not modified.
 *
 * @tparam  IV          Input value type
 * @tparam  OV          Output value type
 *
 * @param   inputs      Input dictionary
 * @param   func        Function to apply: OV(K, IV)
 *
 * @return  DictLike<K, OV>  New dictionary with transformed values
 ******************************************************************************/

template <typename K, typename IV, typename OV,
          template <typename...> class DictLike = std::map>
DictLike<K, OV> transform(const DictLike<K, IV> &inputs,
                          std::function<OV(K, IV)> func) {
  using OutDict = DictLike<K, OV>;
  OutDict res;
  for (const auto &[key, value] : inputs) {
    res.emplace(key, func(key, value));
  }
  return res;
}

/******************************************************************************
 * @brief   Apply a transformation function to each (key, value) pair in a map,
 *          modifying the dictionary in place.
 *
 * @param   inputs      Dictionary to modify
 * @param   func        Function to apply: V(K, V)
 *
 * @return  void
 ******************************************************************************/

template <typename K, typename V,
          template <typename...> class DictLike = std::map>
void transform(DictLike<K, V> &inputs, std::function<V(K, V)> func) {
  for (const auto &[key, value] : inputs) {
    inputs[key] = func(key, value);
  }
}

/******************************************************************************
 * @brief   Construct a dictionary from a set, where each key in the set
 *          is mapped to a value computed by a function.
 *
 * @param   inputs      Input set
 * @param   func        Function to compute values: V(K)
 *
 * @return  DictLike<K, V>  New dictionary mapping set elements to values
 ******************************************************************************/
template <typename K, typename V,
          template <typename...> class SetLike = std::set,
          template <typename...> class DictLike = std::map>
DictLike<K, V> new_by_set(const SetLike<K> &inputs, std::function<V(K)> func) {
  DictLike<K, V> res;
  for (const auto &elem : inputs) {
    res.emplace(elem, func(elem));
  }
  return res;
}

/******************************************************************************
 * @brief   Collect all values from a dictionary into a container.
 *
 * @param   in          Input dictionary
 *
 * @return  ContainerLike<V>  Container of values
 ******************************************************************************/

template <typename K, typename V,
          template <typename...> class ContainerLike = std::vector,
          template <typename...> class DictLike = std::map>
ContainerLike<V> values_of(const DictLike<K, V>  in) {
  ContainerLike<V> res;
  for (const auto &[key, value] : in) {
    container_emplace<V, ContainerLike>(res, value);
  }
  return res;
}

/******************************************************************************
 * @brief   Insert or update an element in a dictionary.
 *          - If the key does not exist, insert (key, value).
 *          - If the key exists, update the value using a user-provided
 *
 * @param   dict        Dictionary to modify
 * @param   key         Key to insert/update
 * @param   value       Value to insert or combine
 * @param   func        Function to combine old and new values: V(K, V_existing,
 *V_new)
 *
 * @return  void
 ******************************************************************************/

template <typename K, typename V,
          template <typename...> class DictLike = std::map>
auto update(DictLike<K, V> &dict, const K &key, const V &value,
            std::function<V(K, V, V)> func) {
  auto it = dict.find(key);
  if (it == dict.end())
    it = dict.emplace(key, value).first;
  else {
    auto new_elem = func(key, it->second, value);
    it->second = new_elem;
  }
  return it;
}

} // namespace dictool