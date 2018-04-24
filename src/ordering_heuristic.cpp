
#include "ordering_heuristic.hpp"

using namespace std;

vector<Item> OrderingHeuristic::orderKeepStacks(const Problem &problem, unsigned seed) {
  OrderingHeuristic heuristic(problem, seed);
  heuristic.orderKeepStacks();
  return heuristic.ordering_;
}

vector<Item> OrderingHeuristic::orderShuffleStacks(const Problem &problem, unsigned seed) {
  OrderingHeuristic heuristic(problem, seed);
  heuristic.orderShuffleStacks();
  return heuristic.ordering_;
}

OrderingHeuristic::OrderingHeuristic(const Problem &problem, unsigned seed)
: rgen_(seed)
, leftover_(problem.stackItems()) {
}

vector<Item> OrderingHeuristic::orderKeepStacks() {
  while (!leftover_.empty()) {
    uniform_int_distribution<size_t> dist(0, leftover_.size() - 1);
    size_t ind = dist(rgen_);
    ordering_.insert(ordering_.end(), leftover_[ind].begin(), leftover_[ind].end());
    leftover_.erase(leftover_.begin() + ind);
  }
  return ordering_;
}

vector<Item> OrderingHeuristic::orderShuffleStacks() {
  size_t nbElements = 0;
  for (auto left : leftover_) {
    nbElements += left.size();
  }
  for (; nbElements > 0; --nbElements) {
    uniform_int_distribution<size_t> dist(0, nbElements- 1);
    size_t eltInd = dist(rgen_);
    size_t stackInd = 0;
    while (eltInd >= leftover_[stackInd].size()) {
      eltInd -= leftover_[stackInd].size();
      ++stackInd;
    }
    ordering_.push_back(leftover_[stackInd].front());
    leftover_[stackInd].erase(leftover_[stackInd].begin());
  }
  return ordering_;
}
