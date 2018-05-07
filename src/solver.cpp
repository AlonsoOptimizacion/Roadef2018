
#include "solver.hpp"
#include "sequence_packer.hpp"
#include "ordering_heuristic.hpp"
#include "solution_checker.hpp"

#include "move.hpp"
#include <iostream>

using namespace std;

Solver::Solver(const Problem &problem, size_t seed, size_t nMoves)
: problem_(problem)
, seed_(seed)
, nMoves_(nMoves) {
  moves_.emplace_back(make_unique<ShuffleMove>());
  moves_.emplace_back(make_unique<StackShuffleMove>());
  moves_.emplace_back(make_unique<SwapMove>());
  moves_.emplace_back(make_unique<AdjacentSwapMove>());
  moves_.emplace_back(make_unique<InsertMove>());
  moves_.emplace_back(make_unique<RowInsertMove>());
  moves_.emplace_back(make_unique<CutInsertMove>());
  moves_.emplace_back(make_unique<PlateInsertMove>());
  moves_.emplace_back(make_unique<RowSwapMove>());
  moves_.emplace_back(make_unique<CutSwapMove>());
  moves_.emplace_back(make_unique<PlateSwapMove>());
}

Solution Solver::run(const Problem &problem, size_t seed, size_t nMoves) {
  Solver solver(problem, seed, nMoves);
  solver.run();
  return solver.solution_;
}

void Solver::run() {
  mt19937 rgen(seed_);
  for (size_t i = 0; i < nMoves_; ++i) {
    pickMove(rgen).run(problem_, solution_, rgen);
  }

  cout << endl << "MoveName\tTotal\tErr\t-\t=\t+" << endl;
  for (auto &m : moves_) {
    string name = m->name();
    while (name.size() < 12)
      name.append(" ");
    cout << name << "\t" << m->nCalls() << "\t" << m->nViolations() << "\t" << m->nDegrade() << "\t" << m->nEquiv() << "\t" << m->nImprove() << endl;
  }
}

Move& Solver::pickMove(mt19937 &rgen) {
  uniform_int_distribution<int> dist(0, moves_.size()-1);
  return *moves_[dist(rgen)];
}

