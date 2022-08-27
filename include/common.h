#pragma once

#ifdef _STAR_MAP_DEBUG
#define LOG std::cerr
#else
#define LOG                                                                    \
  if (false)                                                                   \
  std::cerr
#endif