#pragma once
#include <fstream>
#include <iostream>
#include <string>

enum State { SEARCHVAR, SEARCHSECTION, FOUND };

class MyParce {
 public:
  MyParce() = delete;

  ~MyParce();

  bool Sign = false;

  State st = SEARCHSECTION;

  std::ifstream stream;

  MyParce(const std::string& str);

  template <typename T>
  T GetValue(const std::string& find_str) {
    bool SearchSection = false;
    bool SearchVar = false;
    std::string varName = find_str.substr(find_str.find(".") + 1);
    std::string sectionName = find_str.substr(0, find_str.find("."));
    std::string result;
    stream.clear();
    stream.seekg(0, std::ios::beg);
    Sign = false;
    st = SEARCHSECTION;
    while (!stream.eof()) {
      if (st == State::FOUND) {
        break;
      }
      std::string line;
      std::getline(stream, line);

      if (line.empty()) continue;

      size_t pos = 0;
      if (st == State::SEARCHSECTION) {
        while (pos < line.size()) {
          if ((line[pos] == ' ')) {
            pos++;
            continue;
          }

          if (line[pos] == ';') {
            break;
          }

          if (line[pos] == '[') {
            pos++;
            if (sectionName == GetSection(line, pos)) {
              SearchSection = true;
              st = State::SEARCHVAR;
              pos++;
              break;
            }
          }
          pos++;
        }
      }

      if (st == State::SEARCHVAR) {
        while (pos < line.size()) {
          if (line[pos] == ' ') {
            pos++;
            continue;
          }

          if (line[pos] == ';') {
            break;
          }

          if (line[pos] == '[') {
            st = State::SEARCHSECTION;
            break;
          }
          auto temp = GetVariable(line, pos, varName);
          result = temp.first;
          SearchVar = temp.second;

          if (!result.empty()) {
            break;
          }
        }
      }
    }

    if (!SearchSection && stream.eof()) {
      stream.close();
      throw std::logic_error("Wrong Section\n");
    }

    if (!SearchVar && stream.eof()) {
      stream.close();
      throw std::logic_error("Wrong variable\n");
    }

    return CheckType<T>(result);
  }

  template <typename T>
  T CheckType(std::string& result) {
    if constexpr (std::is_same<T, int>::value) {
      int int_variable = 0;
      size_t index = 0;
      while (index < result.size()) {
        int_variable = int_variable * 10;

        int_variable += result[index] - '0';

        index++;
      }
      if (Sign) {
        return int_variable * (-1);
      }
      return int_variable;
    } else if constexpr (std::is_same<T, double>::value) {
      double double_variable = 0;

      bool flag = false;

      int level = 1;

      size_t index = 0;

      while (index < result.size()) {
        if (result[index] == '.') {
          flag = true;
          index++;
          continue;
        }

        if (!flag) {
          if (result[index] != '0') {
            double_variable += result[index] - '0' + double_variable * 10;
          } else {
            double_variable *= 10;
          }

        } else {
          double double_temp = result[index] - '0';
          double_variable += double_temp / (level * 10);
          level *= 10;
        }

        index++;
      }
      if (Sign) {
        return double_variable * (-1);
      }
      return double_variable;
    } else if constexpr (std::is_same<T, std::string>::value) {
      return result;
    } else {
      stream.close();
      throw std::logic_error("Unknown type of variable\n");
    }
  }

  std::string GetSection(const std::string& str, size_t& pos);

  std::pair<std::string, bool> GetVariable(const std::string& str, size_t& pos,
                                           const std::string& varName);

  std::string GetValue(const std::string& str, size_t& pos);
};
