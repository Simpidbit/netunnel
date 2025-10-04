#ifndef NETUNNEL_BIND_H
#define NETUNNEL_BIND_H

#include "config.h"

#include <string>
#include <unordered_map>

namespace netunnel {

  class bind_conf_t {
  public:
    std::unordered_map<std::string, std::string> files;
    std::unordered_map<std::string, std::string> dirs;

    bind_conf_t (std::string bind_filepath = BIND_FILEPATH);
  };

} // namespace netunnel

#endif
