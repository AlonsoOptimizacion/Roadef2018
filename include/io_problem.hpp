// Copyright (C) 2019 Gabriel Gouvine - All Rights Reserved

#ifndef IO_PROBLEM_HPP
#define IO_PROBLEM_HPP

#include "problem.hpp"

class IOProblem {
 public:
  IOProblem(std::string nameItems, std::string nameDefects, std::string nameParams);
  Problem read();
  void write(const Problem &pb);

  void setPermissive(bool permissive);

 private:
  std::vector<Item> readItems();
  std::vector<Defect> readDefects();

  void writeParams();
  void writeItems(const std::vector<Item> &items);
  void writeDefects(const std::vector<Defect> &defects);

  std::string nameParams() const;
  std::string nameItems() const;
  std::string nameDefects() const;

  void readItem(const std::string &s, std::vector<Item> &items);
  void readDefect(const std::string &s, std::vector<Defect> &defects);

  std::vector<std::string> readCSVLine(const std::string &s);

 private:
  std::string nameItems_;
  std::string nameDefects_;
  std::string nameParams_;
  bool permissive_;
};

#endif

