#include "HashTable.h"

Custom::HashTable::HashTable() : CAPACITY(20), size(0), buckets(CAPACITY) {}

Custom::HashTable::~HashTable() {}

void Custom::HashTable::insert(std::string key, Stock value) {
  int index = hash(key);
  std::vector<Stock> &bucket = buckets.at(index);

  for (Stock &stock : bucket) {
    if (stock.stockCode == key) {
      stock = value;
      return;
    }
  }

  bucket.push_back(value);
  size++;

  if (((float)size) / CAPACITY > LOAD_FACTOR_THRESHOLD) {
    resize();
  }
}

std::optional<Stock> Custom::HashTable::get(std::string key) {
  int index = hash(key);
  std::vector<Stock> bucket = buckets.at(index);

  for (Stock stock : bucket) {
    if (key == stock.stockCode) {
      return stock;
    }
  }

  return {};
}

std::vector<Stock> Custom::HashTable::getAll() {
  std::vector<Stock> stockVector;
  for (std::vector<Stock> bucket : buckets) {
    for (Stock stock : bucket) {
      stockVector.push_back(stock);
    }
  }

  return stockVector;
}

bool Custom::HashTable::remove(std::string key) {
  int index = hash(key);
  std::vector<Stock> &bucket = buckets.at(index);

  for (unsigned int i = 0; i < bucket.size(); i++) {
    if (bucket.at(i).stockCode == key) {
      bucket.erase(bucket.begin() + i);
      size--;

      return true;
    }
  }

  return false;
}

bool Custom::HashTable::clear() {
  for (std::vector<Stock> &bucket : buckets) {
    bucket.clear();

    if (bucket.size() != 0) {
      return false;
    }
  }

  return true;
}

//******************PRIVATE FUNCTIONS********************
int Custom::HashTable::hash(std::string key) {
  int hashValue = 0;
  for (char c : key) {
    hashValue = (hashValue * 31 + c) % CAPACITY;
  }

  return hashValue;
}

void Custom::HashTable::resize() {
  std::vector<std::vector<Stock>> oldBuckets = buckets;
  CAPACITY *= 2;
  buckets.resize(CAPACITY);

  for (std::vector<Stock> bucket : oldBuckets) {
    for (Stock stock : bucket) {
      insert(stock.stockCode, stock);
    }
  }
}
