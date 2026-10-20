#pragma once

namespace hello_space
{
// Returns true if the time between two messages exceeds max_gap_s.
inline bool is_gap_too_long(double previous_s, double current_s, double max_gap_s)
{
  return (current_s - previous_s) > max_gap_s;
}
}  // namespace hello_space
