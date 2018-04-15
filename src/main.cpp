
#include "problem.hpp"
#include "solver.hpp"
#include "solution_checker.hpp"
#include "utils.hpp"

#include <iostream>

using namespace std;

int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "cut NAME" << std::endl;
    cout << "will run the optimization on NAME_batch.csv with defects NAME_defects.csv" << endl;
    exit(1);
  }

  Problem pb = Problem::read(argv[1]);

  if (argc >= 3) {
    pb.write(argv[2]);
  }

  Problem down = downscale(pb, 100);
  Solution downsol = Solver::run(down);
  Solution solution = upscale(pb, downsol, 100);
  //Solution solution = Solver::run(pb);

  solution.write(cout);
  SolutionChecker::report(pb, solution);

  return 0;
}


