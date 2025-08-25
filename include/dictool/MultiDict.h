#ifndef __SMART_DICT_H__
#define __SMART_DICT_H__

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>

namespace dictool {

/**
 * @brief Type trait to check if a container has emplace_back method.
 *
 * @tparam T The container type to check.
 * @tparam void SFINAE parameter.
 */
template <typename T, typename = void>
struct HasEmplaceBack : std::false_type {};

/**
 * @brief Specialization for containers that have emplace_back method.
 *
 * @tparam T The container type that has emplace_back.
 */
template <typename T>
struct HasEmplaceBack<T, std::void_t<decltype(std::declval<T>().emplace_back(
                             std::declval<typename T::value_type>()))>>
    : std::true_type {};

/**
 * @brief Helper function to check if a container has emplace_back method.
 *
 * @tparam T The container type to check.
 * @return true if container has emplace_back, false otherwise.
 */
template <typename T> constexpr bool has_emplace_back() {
  return HasEmplaceBack<T>::value;
}

/**
 * @brief Type trait to check if a container has emplace method.
 *
 * @tparam T The container type to check.
 * @tparam void SFINAE parameter.
 */
template <typename T, typename = void>
struct HasEmplaceValue : std::false_type {};

/**
 * @brief Specialization for containers that have emplace method.
 *
 * @tparam T The container type that has emplace.
 */
template <typename T>
struct HasEmplaceValue<T, std::void_t<decltype(std::declval<T>().emplace())>>
    : std::true_type {};

/**
 * @brief Helper function to check if a container has emplace method.
 *
 * @tparam T The container type to check.
 * @return true if container has emplace, false otherwise.
 */
template <typename T> constexpr bool has_emplace_value() {
  return HasEmplaceValue<T>::value;
}

template <typename T, template <typename...> class CTemplate = std::vector>
static void container_emplace(CTemplate<T> &c, const T &elem) {
  using Container = CTemplate<T>;
  // Static assertion to ensure container has required methods
  static_assert(has_emplace_back<Container>() || has_emplace_value<Container>(),
                "Container must have either emplace_back or emplace method");
  if constexpr (has_emplace_back<Container>()) {
    c.emplace_back(elem);
  } else {
    c.emplace(elem);
  }
}

/**
 * @brief A multi-map implementation that stores multiple values per key using
 * customizable containers.
 *
 * @tparam K Key type.
 * @tparam V Value type.
 * @tparam CTemplate Container template for storing values (default:
 * std::vector).
 * @tparam DTemplate Dictionary template for storing key-container pairs
 * (default: std::map).
 */
template <typename K, typename V,
          template <typename...> class CTemplate = std::vector,
          template <typename...> class DTemplate = std::map>
class MultiMap {
private:
  using Container = CTemplate<V>; /**< Type of the value container */
  using Dict =
      DTemplate<K, Container>; /**< Type of the underlying dictionary */
  Dict data;                   /**< The underlying data storage */

  // Static assertion to ensure container has required methods
  static_assert(has_emplace_back<Container>() || has_emplace_value<Container>(),
                "Container must have either emplace_back or emplace method");

public:
  // Type aliases (compatible with std::multimap)
  using key_type = K;                         /**< Type of keys */
  using mapped_type = V;                      /**< Type of values */
  using value_type = std::pair<const K, V>;   /**< Type of key-value pairs */
  using size_type = size_t;                   /**< Size type */
  using difference_type = ptrdiff_t;          /**< Difference type */
  using reference = value_type &;             /**< Reference type */
  using const_reference = const value_type &; /**< Const reference type */
  using pointer = value_type *;               /**< Pointer type */
  using const_pointer = const value_type *;   /**< Const pointer type */

  /**
   * @brief Const iterator for MultiMap.
   *
   * Provides read-only access to key-value pairs.
   */
  class Iterator {
  private:
    using outer_iterator =
        typename Dict::const_iterator; /**< Iterator for the outer dictionary */
    using inner_iterator =
        typename Container::const_iterator; /**< Iterator for the inner
                                               container */

    outer_iterator outer_it;  /**< Current outer iterator position */
    outer_iterator outer_end; /**< End of outer dictionary */
    inner_iterator inner_it;  /**< Current inner iterator position */

    /**
     * @brief Advance to the next valid position.
     *
     * Skips empty containers and moves to the next valid key-value pair.
     */
    void advance_to_next_valid() {
      while (outer_it != outer_end && inner_it == outer_it->second.end()) {
        ++outer_it;
        if (outer_it != outer_end) {
          inner_it = outer_it->second.begin();
        }
      }
    }

  public:
    /**
     * @brief Construct a new iterator object.
     *
     * @param oit Outer iterator position.
     * @param oend End of outer dictionary.
     * @param iit Inner iterator position.
     */
    Iterator(outer_iterator oit, outer_iterator oend, inner_iterator iit)
        : outer_it(oit), outer_end(oend), inner_it(iit) {
      advance_to_next_valid();
    }

    /**
     * @brief Get the current key.
     *
     * @return const key_type& Const reference to the current key.
     */
    const key_type &key() const { return outer_it->first; }

    /**
     * @brief Get the current value.
     *
     * @return const mapped_type& Const reference to the current value.
     */
    const mapped_type &value() const { return *inner_it; }

    /**
     * @brief Convert the current position to a key-value pair.
     *
     * @return std::pair<const key_type&, const mapped_type&> Key-value pair.
     */
    std::pair<const key_type &, const mapped_type &> to_pair() const {
      return {key(), value()};
    }

    /**
     * @brief Dereference operator.
     *
     * @return std::pair<const K&, const V&> Key-value pair.
     */
    std::pair<const K &, const V &> operator*() const { return to_pair(); }

    /**
     * @brief Pre-increment operator.
     *
     * @return iterator& Reference to the incremented iterator.
     */
    Iterator &operator++() {
      if (outer_it != outer_end) {
        ++inner_it;
        advance_to_next_valid();
      }
      return *this;
    }

    /**
     * @brief Post-increment operator.
     *
     * @return iterator Copy of the iterator before incrementing.
     */
    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    /**
     * @brief Equality comparison operator.
     *
     * @param other Other iterator to compare with.
     * @return true if iterators are equal, false otherwise.
     */
    bool operator==(const Iterator &other) const {
      return outer_it == other.outer_it &&
             (outer_it == outer_end || inner_it == other.inner_it);
    }

    /**
     * @brief Inequality comparison operator.
     *
     * @param other Other iterator to compare with.
     * @return true if iterators are not equal, false otherwise.
     */
    bool operator!=(const Iterator &other) const { return !(*this == other); }
  };

  // ========== std::multimap compatible API ==========

  /**
   * @brief Default constructor.
   */
  MultiMap() = default;

  /**
   * @brief Range constructor.
   *
   * @tparam InputIt Input iterator type.
   * @param first Beginning of the range.
   * @param last End of the range.
   */
  template <typename InputIt> MultiMap(InputIt first, InputIt last) {
    insert(first, last);
  }

  /**
   * @brief Initializer list constructor.
   *
   * @param init Initializer list of key-value pairs.
   */
  MultiMap(std::initializer_list<value_type> init) { insert(init); }

  // Capacity methods

  /**
   * @brief Check if the multimap is empty.
   *
   * @return true if empty, false otherwise.
   */
  [[nodiscard]] bool empty() const { return data.empty(); }

  /**
   * @brief Get the total number of values in the multimap.
   *
   * @return size_type Total number of values.
   */
  [[nodiscard]] size_type size() const {
    size_type count = 0;
    for (const auto &pair : data) {
      count += pair.second.size();
    }
    return count;
  }

  /**
   * @brief Get the maximum possible size.
   *
   * @return size_type Maximum size.
   */
  [[nodiscard]] size_type max_size() const { return data.max_size(); }

  // Modifiers

  /**
   * @brief Clear all data from the multimap.
   */
  void clear() { data.clear(); }

  // Insert methods

  /**
   * @brief Insert a key-value pair.
   *
   * @param value Key-value pair to insert.
   * @return iterator Iterator to the inserted element.
   */
  Iterator insert(const value_type &value) {
    return emplace(value.first, value.second);
  }

  /**
   * @brief Insert a key-value pair using move semantics.
   *
   * @param value Key-value pair to move.
   * @return iterator Iterator to the inserted element.
   */
  Iterator insert(value_type &&value) {
    return emplace(std::move(const_cast<K &>(value.first)),
                   std::move(value.second));
  }

  /**
   * @brief Insert a range of key-value pairs.
   *
   * @tparam InputIt Input iterator type.
   * @param first Beginning of the range.
   * @param last End of the range.
   */
  // only allow interator
  // template <typename InputIt, typename std::enable_if_t<
  //                                 !std::is_same_v<K, V> ||
  //                                 !std::is_same_v<std::decay_t<InputIt>, K>>>

  template <typename InputIt> void insert(InputIt first, InputIt last) {
    for (; first != last; ++first) {
      insert(*first);
    }
  }

  /**
   * @brief Insert from initializer list.
   *
   * @param init Initializer list of key-value pairs.
   */
  void insert(std::initializer_list<value_type> init) {
    insert(init.begin(), init.end());
  }

  /**
   * @brief Append a range of values to the container for the given key.
   *
   * @tparam InputIt Input iterator type.
   * @param key Key to append to.
   * @param first Beginning of the range.
   * @param last End of the range.
   */
  template <typename InputIt>
  void insert(const K &key, InputIt first, InputIt last) {
    auto &container = data[key];
    if constexpr (has_emplace_back<Container>()) {
      container.insert(container.end(), first, last);
    } else {
      container.insert(first, last);
    }
  }

  // Emplace methods

  /**
   * @brief Construct a key-value pair in place.
   *
   * @tparam Args Types of arguments to forward.
   * @param args Arguments to forward to value constructor.
   * @return iterator Iterator to the emplaced element.
   */
  template <typename... Args> Iterator emplace(Args &&...args) {
    // Construct pair to decompose arguments
    value_type pair(std::forward<Args>(args)...);
    auto &container = data[pair.first];

    if constexpr (has_emplace_back<Container>()) {
      container.emplace_back(std::move(pair.second));
      return Iterator(data.find(pair.first), data.end(), --container.end());
    } else if constexpr (has_emplace_value<Container>()) {
      auto it = container.emplace(std::move(pair.second));
      return Iterator(data.find(pair.first), data.end(), it.first);
    }
  }

  // Erase methods

  /**
   * @brief Erase element at position.
   *
   * @param pos Iterator to the element to erase.
   * @return iterator Iterator following the erased element.
   */
  Iterator erase(Iterator pos) {
    if (pos == end())
      return end();

    auto outer_it = data.find(pos.key());
    if (outer_it != data.end()) {
      auto &container = outer_it->second;
      auto inner_it =
          std::find(container.begin(), container.end(), pos.value());
      if (inner_it != container.end()) {
        inner_it = container.erase(inner_it);
        if (container.empty()) {
          outer_it = data.erase(outer_it);
          if (outer_it == data.end())
            return end();
          return Iterator(outer_it, data.end(), outer_it->second.begin());
        }
        return Iterator(outer_it, data.end(), inner_it);
      }
    }
    return end();
  }

  /**
   * @brief Erase a range of elements.
   *
   * @param first Beginning of the range to erase.
   * @param last End of the range to erase.
   * @return iterator Iterator following the last erased element.
   */
  Iterator erase(Iterator first, Iterator last) {
    Iterator result = end();
    while (first != last) {
      result = erase(first++);
    }
    return result;
  }

  /**
   * @brief Erase all elements with the given key.
   *
   * @param key Key to erase.
   * @return size_type Number of elements erased.
   */
  size_type erase(const key_type &key) {
    auto it = data.find(key);
    if (it != data.end()) {
      size_type count = it->second.size();
      data.erase(it);
      return count;
    }
    return 0;
  }

  /**
   * @brief Erase specific value from a key's container.
   *
   * @param key Key to erase from.
   * @param value Value to erase.
   * @return true if value was erased, false otherwise.
   */
  bool erase(const K &key, const V &value) {
    auto it = data.find(key);
    if (it != data.end()) {
      auto &container = it->second;
      if constexpr (has_emplace_back<Container>()) {
        // For sequence containers, use find + erase
        auto pos = std::find(container.begin(), container.end(), value);
        if (pos != container.end()) {
          container.erase(pos);
          return true;
        }
      } else {
        // For associative containers, direct erase
        return container.erase(value) > 0;
      }
    }
    return false;
  }

  // Lookup methods

  /**
   * @brief Count elements with the given key.
   *
   * @param key Key to count.
   * @return size_type Number of elements with the key.
   */
  size_type count(const key_type &key) const {
    auto it = data.find(key);
    return it != data.end() ? it->second.size() : 0;
  }

  /**
   * @brief Find an element with the given key (const version).
   *
   * @param key Key to find.
   * @return iterator Iterator to the first element with the key, or end() if
   * not found.
   */
  Iterator find(const key_type &key) const {
    auto it = data.find(key);
    if (it != data.end() && !it->second.empty()) {
      return Iterator(it, data.end(), it->second.begin());
    }
    return end();
  }

  /**
   * @brief Get the range of elements with the given key (const version).
   *
   * @param key Key to find.
   * @return std::pair<iterator, iterator> Range of elements with the key.
   */
  std::pair<Iterator, Iterator> equal_range(const key_type &key) const {
    auto it = data.find(key);
    if (it == data.end()) {
      return {end(), end()};
    }
    return {Iterator(it, data.end(), it->second.begin()),
            Iterator(it, data.end(), it->second.end())};
  }

  // Iterator methods

  /**
   * @brief Get iterator to the beginning.
   *
   * @return iterator Iterator to the first element.
   */
  Iterator begin() const {
    if (data.empty())
      return end();
    return Iterator(data.begin(), data.end(), data.begin()->second.begin());
  }

  /**
   * @brief Get iterator to the end (const version).
   *
   * @return iterator Iterator to the end.
   */
  Iterator end() const {
    return Iterator(data.end(), data.end(),
                    typename Container::const_iterator());
  }

  // Additional convenience methods (non-std::multimap API)

  /**
   * @brief Check if the multimap contains a key.
   *
   * @param key Key to check.
   * @return true if key exists, false otherwise.
   */
  bool contains(const key_type &key) const {
    return data.find(key) != data.end();
  }

  /**
   * @brief Get all keys in the multimap.
   *
   * @return std::vector<key_type> Vector of all keys.
   */
  std::vector<key_type> keys() const {
    std::vector<key_type> result;
    for (const auto &pair : data) {
      result.push_back(pair.first);
    }
    return result;
  }

  /**
   * @brief Helper method to emplace into a container.
   *
   * @param c Container to emplace into.
   * @param value Value to emplace.
   * @return typename Container::iterator Iterator to the emplaced element.
   */

  /**
   * @brief Get the container for a key safely.
   *
   * @param key Key to look up.
   * @return const Container& Reference to the container (empty if key doesn't
   * exist).
   */
  [[nodiscard]] Container get(const K &key, Container defval = {}) const {
    auto it = data.find(key);
    return it != data.end() ? it->second : defval;
  }

  /**
   * get the internal data struct
   * @return const reference of internal Dict Container
   */
  const Dict &cdata() const { return data; }

  /**
   * @brief Access container for a key (creates if not exists).
   *
   * @param key Key to access.
   * @return Container& Reference to the container for the key.
   */
  Container &operator[](const K &key) { return data[key]; }

  /**
   * @brief Access container for a key with bounds checking.
   *
   * @param key Key to access.
   * @return const Container& Reference to the container for the key.
   * @throw std::out_of_range if key doesn't exist.
   */
  [[nodiscard]] const Container &at(const K &key) const { return data.at(key); }

  /**
   * @brief Merge another MultiMap into this one.
   *
   * @param other Other MultiMap to merge.
   */
  void merge(const MultiMap &other) {
    for (const auto &[key, container] : other.data) {
      insert(key, container.begin(), container.end());
    }
  }

  /**
   * @brief Get the number of keys in the multimap.
   *
   * @return size_t Number of keys.
   */
  [[nodiscard]] size_t key_count() const { return data.size(); }

  /**
   * @brief Get the number of values for a specific key.
   *
   * @param key Key to count values for.
   * @return size_t Number of values for the key.
   */
  size_t value_count(const K &key) const { return count(key); }

  /**
   * @brief Get the total number of values in the multimap.
   *
   * @return size_t Total number of values.
   */
  [[nodiscard]] size_t total_value_count() const {
    size_t count = 0;
    for (const auto &pair : data) {
      count += pair.second.size();
    }
    return count;
  }

  /**
   * @brief Print statistics for debugging.
   */
  void print_stats(std::ostream &os = std::cout) const {
    os << "Total keys: " << key_count() << "\n";
    os << "Total values: " << total_value_count() << "\n";
    for (const auto &[key, container] : data) {
      os << "Key " << key << ": " << container.size() << " values\n";
    }
  }

  /**
   * @brief Print all data for debugging.
   */
  void print_all(std::ostream &os = std::cout) const {
    for (const auto &[key, container] : data) {
      os << "Key " << key << ": [";
      for (const auto &value : container) {
        os << value << " ";
      }
      os << "]\n";
    }
  }
};

template <typename K, typename V,
          template <typename...> class DTemplate = std::map>
class UniqueMultiDict : public MultiMap<K, V, std::set> {};

template <typename K, typename V,
          template <typename...> class DTemplate = std::map>
class VectorMultiDict : public MultiMap<K, V, std::vector> {};

} // namespace dictool

#endif