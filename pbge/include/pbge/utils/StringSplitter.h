#ifndef PBGE_UTILS_STRINGSPLITTER_H_
#define PBGE_UTILS_STRINGSPLITTER_H_

#include <string>
#include <vector>

namespace pbge {
    class StringSplitter {
    public:
        StringSplitter(const std::string & sep = " ") : separator(sep) {}

        void split(const std::string text, std::vector<std::string> & tokens) {
            size_t begin = text.find_first_not_of(separator);
            size_t end = text.find_first_of(separator, begin);
            while(begin != std::string::npos) {
                tokens.push_back(std::string(text, begin, end - begin));
                begin = text.find_first_not_of(separator, end);
                end = text.find_first_of(separator, begin);
            }
        }
    private:
        std::string separator;
    };
}

#endif