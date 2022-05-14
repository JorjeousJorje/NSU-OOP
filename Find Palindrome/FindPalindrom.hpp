#pragma once

#include <istream>
#include <string>
#include <vector>


class palindrom {
public:

    palindrom() = default;
    bool read(std::istream& in);
    void write_result();

private:

    std::string str_;
    std::vector<std::string> v_;

    static std::vector<std::string> parse(const std::string& str);
    static void prepare(std::string& str);
    static void words_as_literals(std::vector<std::string>& v);
    static bool is_palindrom(const std::string& str);
    static void whole_sentence(std::string& str);
    static void single_words(std::vector<std::string>& v);
};