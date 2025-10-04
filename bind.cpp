#include "bind.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <json.hpp>

netunnel::bind_conf_t::bind_conf_t(std::string bind_filepath)
{
  std::ifstream bind_file(bind_filepath);
  if (!bind_file) {
    std::cerr << "ERROR bind_file!" << std::endl;
  }

  std::stringstream buffer;

  buffer << bind_file.rdbuf();
  std::string conftext = buffer.str();

  bind_file.close();

  nlohmann::json jdata;
  try {
    jdata = nlohmann::json::parse(conftext);
  } catch (nlohmann::json::parse_error &e) {
    std::cerr << "JSON PARSE FAILED!" << std::endl;
  }

  for (auto &each : jdata["files"].items())
    this->files[each.key()] = each.value();

  for (auto &each : jdata["dirs"].items())
    this->dirs[each.key()] = each.value();
}
