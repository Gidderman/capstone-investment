//This is a classic partitioning function, traversing through our 
//range and placing all the smaller values to the left side
template <typename T, typename Comparator> 
int partition(std::vector<T> &vect, int low, int high, Comparator comparator) {
    int index = low - 1;

    for (int j = low; j <= high -1; j++) {
      if (comparator(vect.at(j), vect.at(high))) {
          index++;
          std::swap(vect.at(index), vect.at(j));
      }
    }

    std::swap(vect.at(index + 1), vect.at(high));

    return index + 1;
  }

//The implemented quicksort function, recursively calling itself when necessary.
template <typename T, typename Comparator> 
void performQuickSort(std::vector<T> &vect, int low, int high, Comparator comparator) {
  if (low < high) {
      int pivotIndex = partition(vect, low, high, comparator);

      performQuickSort(vect, low, pivotIndex-1, comparator);
      performQuickSort(vect, pivotIndex+1, high, comparator);
    }
}


template <typename T, typename Comparator> 
void Custom::quickSort(std::vector<T> &vect, Comparator comparator) {
    if (vect.size() == 0) {
        return;
      }

    performQuickSort(vect, 0, vect.size() -1, comparator);
}
