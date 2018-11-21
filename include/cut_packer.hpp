
#ifndef CUT_PACKER_HPP
#define CUT_PACKER_HPP

#include "problem.hpp"
#include "solution.hpp"
#include "packer.hpp"
#include "row_packer.hpp"
#include "packer_front.hpp"

class CutPacker : Packer {
 public:
  CutPacker(const Problem &problem, const std::vector<Item> &sequence);
  CutSolution run(Rectangle cut, int start, const std::vector<Defect> &defects);
  int count(Rectangle cut, int start, const std::vector<Defect> &defects);

 private:
  void runCommon(Rectangle cut, int start, const std::vector<Defect> &defects);
  void propagate(int previousFront, int previousItems, int beginCoord);
  void propagateBreakpoints(int after);

  CutSolution backtrack();
  int countBacktrack();

  RowPacker::RowDescription countRow(int start, int minY, int maxY);
  RowSolution packRow(int start, int minY, int maxY);

  bool isAdmissibleCutLine(int y) const;
  int nDefectsPresent(Rectangle row);

 private:
  PackerFront front_;
  RowPacker rowPacker_;
  std::vector<int> slices_;
};

#endif

