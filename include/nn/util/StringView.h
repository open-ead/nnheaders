#pragma once

#include <string_view>

namespace nn::util {

template <typename CharT, typename Traits = std::char_traits<CharT>>
class basic_string_view : public std::basic_string_view<CharT, Traits> {
public:
    using std::basic_string_view<CharT, Traits>::basic_string_view;

    friend bool operator==(const basic_string_view& lhs, const basic_string_view& rhs) {
        return lhs.compare(rhs) == 0;
    }

    friend bool operator!=(const basic_string_view& lhs, const basic_string_view& rhs) {
        return lhs.compare(rhs) != 0;
    }
};

using string_view = basic_string_view<char>;

}  // namespace nn::util
