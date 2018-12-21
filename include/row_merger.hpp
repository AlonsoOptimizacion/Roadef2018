
#ifndef ROW_PACKER_HPP
#define ROW_PACKER_HPP

#include "problem.hpp"
#include "solution.hpp"
#include "merger.hpp"

class RowMerger : Merger {
 public:
  RowMerger(SolverParams options, const std::pair<std::vector<Item>, std::vector<Item> > &sequences);

  // Initialize with these starting points
  void init(Rectangle row, const std::vector<Defect> &defects, std::pair<int, int> starts);
  void init(Rectangle row, const std::vector<Defect> &defects, const std::vector<std::pair<int, int> > &starts);

  // Run the algorithm itself
  void buildFront();

  std::vector<std::pair<int, int> > getParetoFront() const;
  RowSolution getSolution(std::pair<int, int> ends) const;
  std::pair<int, int> getStarts(std::pair<int, int> ends) const;

  void checkConsistency() const;

 private:
  void buildFrontExact();
  void buildFrontApproximate();
  void checkSolution(const RowSolution &solution) const;

  bool canPlace(int x, int width, int height) const;
  bool canFit(int x, int width, int height) const;
  bool canPlaceDown(int x, int width, int height) const;
  bool canPlaceUp(int x, int width, int height) const;
  bool isAdmissibleCutLine(int x) const;
};

#endif
