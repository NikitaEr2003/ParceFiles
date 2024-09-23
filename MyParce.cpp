#include "MyParce.h"

#include <iterator>
#include <type_traits>

MyParce::MyParce(const std::string& str) : stream(str) {
  if (!stream.is_open()) {
    stream.close();
    throw std::logic_error("File is not open, perhaps path is wrong\n");
  } else {
    stream.seekg(0, std::ios::end);
    std::streamsize fileSize = stream.tellg();
    stream.seekg(0, std::ios::beg);

    if (fileSize == 0) {
      stream.close();
      throw std::logic_error("File is empty, check please path\n");
    }
  }
}

std::string MyParce::GetSection(const std::string& str, size_t& pos) {
  std::string section;
  while (pos < str.size() && str[pos] != ']') {
    section += str[pos];
    pos++;
  }

  return section;
}

std::pair<std::string, bool> MyParce::GetVariable(const std::string& str,
                                                  size_t& pos,
                                                  const std::string& varName) {
  std::string result;
  std::pair<std::string, bool> PairObject = {result, false};
  std::string name;
  while (pos < str.size() && str[pos] != ';') {
    if (str[pos] == ' ') {
      pos++;
      continue;
    }
    if (str[pos] == '=') {
      if (name == varName) {
        st = State::FOUND;
        PairObject.second = true;
        pos++;
        std::string res = GetValue(str, pos);
        std::pair<std::string, bool> obj = {res, true};
        return obj;
      } else {
        pos++;
        break;
      }
    }

    name += str[pos];
    pos++;
  }

  return PairObject;
}

std::string MyParce::GetValue(const std::string& str, size_t& pos) {
  std::string result;

  while (pos < str.size() && str[pos] != ';') {
    if (str[pos] == ' ') {
      pos++;
      continue;
    }
    if (str[pos] == '-') {
      Sign = true;
      pos++;
      continue;
    }
    result += str[pos];
    pos++;
  }
  if (result.empty()) {
    stream.close();
    throw std::logic_error("Value is empty in file\n");
  }
  return result;
}

MyParce ::~MyParce() { stream.close(); }