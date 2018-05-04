
#include "plate_packer.hpp"
#include "pareto_front.hpp"
#include "utils.hpp"

#include <cassert>
#include <algorithm>

using namespace std;

PlateSolution PlatePacker::run(const Problem &problem, int plateId, const vector<Item> &sequence, int start) {
  PlatePacker packer(problem, plateId, sequence, start);
  return packer.run();
}

int PlatePacker::count(const Problem &problem, int plateId, const vector<Item> &sequence, int start) {
  PlatePacker packer(problem, plateId, sequence, start);
  return packer.count();
}

PlatePacker::PlatePacker(const Problem &problem, int plateId, const vector<Item> &sequence, int start)
: Packer(sequence, problem.plateDefects()[plateId]) {
  Params p = problem.params();
  start_ = start;
  minXX_ = p.minXX;
  maxXX_ = p.maxXX;
  minYY_ = p.minYY;
  minWaste_ = p.minWaste;
  region_ = Rectangle::FromCoordinates(0, 0, p.widthPlates, p.heightPlates);
}

PlateSolution PlatePacker::run() {
  // Dynamic programming on the first-level cuts
  assert (region_.minX() == 0);
  assert (region_.minY() == 0);

  int maxCoord = region_.maxX();

  ParetoFront front;
  front.insert(region_.minX(), start_, -1);
  for (int i = 0; i < front.size(); ++i) {
    auto elt = front[i];
    int beginCoord = elt.coord;
    int maxEndCoord = min(maxCoord, beginCoord + maxXX_);
    int previousItems = elt.valeur;
    for (int endCoord = maxEndCoord + minWaste_; endCoord >= beginCoord + minXX_; --endCoord) {
      CutSolution cutSolution = packCut(previousItems, beginCoord, endCoord);

      int maxUsed = cutSolution.maxUsedX();
      if (maxUsed + minWaste_ < endCoord) {
        // Shortcut from the current solution: no need to try all the next ones
        endCoord = maxUsed + minWaste_;
        cutSolution = packCut(previousItems, beginCoord, endCoord);
      }
      if (endCoord <= maxEndCoord)
        front.insert(endCoord, previousItems + cutSolution.nItems(), i);

      if (maxUsed < endCoord) {
        // Fully packed case
        cutSolution = packCut(previousItems, beginCoord, maxUsed);
        front.insert(maxUsed, previousItems + cutSolution.nItems(), i);
      }
    }
  }
  front.checkConsistency();

  // Backtrack for the best solution
  PlateSolution plateSolution(region_);
  int cur = front.size() - 1;
  bool lastCut = true;
  while (cur != 0) {
    auto eltEnd = front[cur];
    int next = eltEnd.previous;
    auto eltBegin = front[next];

    int beginCoord = eltBegin.coord;
    int endCoord = eltEnd.coord;
    if (lastCut && eltEnd.valeur != nItems()) {
      // End but not the residual
      if (maxCoord - beginCoord <= maxXX_) {
        // Extend the last cut to the full plate
        endCoord = maxCoord;
      }
      else if (maxCoord - endCoord >= minXX_) {
        // Not possible to extend, add another cut instead
        // FIXME: when we need to add several new cuts; not possible with standard dimensions
        Rectangle emptyCut = Rectangle::FromCoordinates(endCoord, region_.minY(), maxCoord, region_.maxY());
        plateSolution.cuts.emplace_back(emptyCut);
      }
      else {
        // Not possible to add a cut; try again
        --cur;
        continue;
      }
    }

    auto solution = packCut(eltBegin.valeur, beginCoord, endCoord);
    assert (eltBegin.valeur + solution.nItems() == eltEnd.valeur || endCoord != eltEnd.coord);
    assert (solution.width() >= minXX_);
    assert (solution.width() <= maxXX_);
    plateSolution.cuts.push_back(solution);
    cur = next;
    lastCut = false;
  }
  reverse(plateSolution.cuts.begin(), plateSolution.cuts.end());

  return plateSolution;
}

int PlatePacker::count() {
  return run().nItems();
}

int PlatePacker::countCut(int start, int minX, int maxX) const {
  Rectangle cut = Rectangle::FromCoordinates(minX, region_.minY(), maxX, region_.maxY());
  return CutPacker::count(*this, cut, start);
}

CutSolution PlatePacker::packCut(int start, int minX, int maxX) const {
  Rectangle cut = Rectangle::FromCoordinates(minX, region_.minY(), maxX, region_.maxY());
  return CutPacker::run(*this, cut, start);
}

