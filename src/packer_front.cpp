// Copyright (C) 2019 Gabriel Gouvine - All Rights Reserved

#include "packer_front.hpp"

#include <iostream>

using namespace std;

void PackerFront::checkConsistency() const {
  for (int i = 0; i < size(); ++i) {
    assert (front_[i].previous < i);
    assert (front_[i].begin < front_[i].end);
  }
  for (int i = 0; i + 1 < size(); ++i) {
    assert (front_[i].end < front_[i+1].end);
    assert (front_[i].value < front_[i+1].value);
  }
}

void PackerFront::report() const {
  cout << "Front beginning at " << front_[0].end << endl;
  for (int i = 1; i < size(); ++i) {
    cout << front_[i].value << ": ";
    cout << front_[i].begin << " to " << front_[i].end;
    cout << endl;
  }
}

