#include <stdlib.h>
#include <stdio.h>

#include "execution_slot_tracker.hpp"

execution_slot_tracker::execution_slot_tracker() {}

execution_slot_tracker &execution_slot_tracker::operator =(const execution_slot_tracker &other)
  {
  return *this;
  }
