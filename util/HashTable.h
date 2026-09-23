// This class is for a hash table that will be used to
// store stock values

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "Stock.h"

#include <optional>
#include <string>
#include <vector>

namespace Custom {

class HashTable {
private:
  // The load factor above which we resize
  const float LOAD_FACTOR_THRESHOLD = 0.75f;

  int CAPACITY;
  int size;
  std::vector<std::vector<Stock>> buckets;

  int hash(std::string key);
  void resize();

public:
  HashTable();
  ~HashTable();
  void insert(std::string key, Stock value);
  // Use of the optional class in case the value is not found
  std::optional<Stock> get(std::string key);
  std::vector<Stock> getAll();
  bool remove(std::string key);
  bool clear();
};

}; // namespace Custom

#endif
