/*
   � Copyright (c) Marshall Clow 2012-2015.
   � Copyright Beman Dawes 2015

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org

    Based on the StringRef implementation in LLVM (http://llvm.org) and
    N3422 by Jeffrey Yasskin
        http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3442.html
    Updated July 2015 to reflect the Library Fundamentals TS
        http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4480.html
*/

#ifndef STX_STRING_VIEW_HPP_INCLUDED
#define STX_STRING_VIEW_HPP_INCLUDED

#ifndef STX_NAMESPACE_NAME
#define STX_NAMESPACE_NAME stx
#endif

// libstdc++'s std::experimental::string_view requries C++14
#if !defined(STX_NO_STD_STRING_VIEW) && (__cplusplus < 201402)
#include <cstddef>
#if defined(__GLIBCXX__)
#define STX_NO_STD_STRING_VIEW
#endif
#endif

#if defined(__has_include) && !defined(STX_NO_STD_STRING_VIEW)
#if __has_include(<string_view>) && (__cplusplus > 201402)
        #include <string_view>
        #define STX_HAVE_STD_STRING_VIEW 1
        namespace STX_NAMESPACE_NAME {
            using std::basic_string_view;
            using std::string_view;
            using std::u16string_view;
            using std::u32string_view;
            using std::wstring_view;
        }
    #elif __has_include(<experimental/string_view>)
        #include <experimental/string_view>
        #define STX_HAVE_STD_STRING_VIEW 1
        namespace STX_NAMESPACE_NAME {
            using std::experimental::basic_string_view;
            using std::experimental::string_view;
            using std::experimental::u16string_view;
            using std::experimental::u32string_view;
            using std::experimental::wstring_view;
        }
    #endif // (__has_include(<string_view>) || __has_include(<experimental/string_view>))
#endif // __has_include

#ifndef STX_HAVE_STD_STRING_VIEW

#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <string>
#include <cstring>
#include <iosfwd>

#if __cpp_constexpr >= 201304
#define STX_CONSTEXPR14 constexpr
#else
#define STX_CONSTEXPR14
#endif

namespace STX_NAMESPACE_NAME {

namespace detail {
//  A helper functor because sometimes we don't have lambdas
template <typename charT, typename traits>
class string_view_traits_eq {
public:
    string_view_traits_eq ( charT ch ) : ch_(ch) {}
    bool operator()( charT val ) const { return traits::eq (ch_, val); }
    charT ch_;
};
}

template<typename charT, typename traits = std::char_traits<charT>>
class basic_string_view {
public:
    // types
    typedef traits                                traits_type;
    typedef charT                                 value_type;
    typedef charT*                                pointer;
    typedef const charT*                          const_pointer;
    typedef charT&                                reference;
    typedef const charT&                          const_reference;
    typedef const_pointer                         const_iterator; // impl-defined
    typedef const_iterator                        iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef const_reverse_iterator                reverse_iterator;
    typedef std::size_t                           size_type;
    typedef std::ptrdiff_t                        difference_type;
    static constexpr size_type     npos = size_type(-1);

    // construct/copy
    constexpr basic_string_view() noexcept
            : ptr_(NULL), len_(0) {}

    // by defaulting these functions, basic_string_ref becomes
    //  trivially copy/move constructible.
    constexpr basic_string_view(const basic_string_view &rhs) noexcept = default;
#if 0
    : ptr_(rhs.ptr_), len_(rhs.len_) {}
#endif

    basic_string_view& operator=(const basic_string_view &rhs) noexcept
    = default;
#if 0
    {
        ptr_ = rhs.ptr_;
        len_ = rhs.len_;
        return *this;
        }
#endif

    template<typename Allocator>
    basic_string_view(const std::basic_string<charT, traits,
            Allocator>& str) noexcept
            : ptr_(str.data()), len_(str.length()) {}

    constexpr basic_string_view(const charT* str)
            : ptr_(str), len_(traits::length(str)) {}

    constexpr basic_string_view(const charT* str, size_type len)
            : ptr_(str), len_(len) {}

    // iterators
    constexpr const_iterator   begin() const noexcept { return ptr_; }
    constexpr const_iterator  cbegin() const noexcept { return ptr_; }
    constexpr const_iterator     end() const noexcept { return ptr_ + len_; }
    constexpr const_iterator    cend() const noexcept { return ptr_ + len_; }
    const_reverse_iterator  rbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator    rend() const noexcept { return const_reverse_iterator(begin()); }
    const_reverse_iterator   crend() const noexcept { return const_reverse_iterator(begin()); }

    // capacity
    constexpr size_type size()     const noexcept { return len_; }
    constexpr size_type length()   const noexcept { return len_; }
    constexpr size_type max_size() const noexcept { return len_; }
    constexpr bool empty()         const noexcept { return len_ == 0; }

    // element access
    constexpr const_reference operator[](size_type pos) const noexcept { return ptr_[pos]; }

    constexpr const_reference at(size_t pos) const {
        return pos >= len_ ?  throw std::out_of_range("stx::string_view::at") : ptr_[pos];
//             if ( pos >= len_ )
//                 BOOST_THROW_EXCEPTION( std::out_of_range ( "stx::string_view::at" ) );
//             return ptr_[pos];
    }

    constexpr const_reference front() const                { return ptr_[0]; }
    constexpr const_reference back()  const                { return ptr_[len_-1]; }
    constexpr const_pointer data()    const noexcept { return ptr_; }

    // modifiers
    void clear() noexcept { len_ = 0; }          // Boost extension

    STX_CONSTEXPR14 void remove_prefix(size_type n) {
        if ( n > len_ )
            n = len_;
        ptr_ += n;
        len_ -= n;
    }

    STX_CONSTEXPR14 void remove_suffix(size_type n) {
        if ( n > len_ )
            n = len_;
        len_ -= n;
    }

    STX_CONSTEXPR14 void swap(basic_string_view& s) noexcept {
        std::swap(ptr_, s.ptr_);
        std::swap(len_, s.len_);
    }

    // basic_string_view string operations
#ifndef BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS
    template<typename Allocator>
    explicit operator std::basic_string<charT, traits, Allocator>() const {
        return std::basic_string<charT, traits, Allocator>(begin(), end());
    }
#endif

#ifndef BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS
    template<typename Allocator = std::allocator<charT> >
    std::basic_string<charT, traits, Allocator> to_string(const Allocator& a = Allocator()) const {
        return std::basic_string<charT, traits, Allocator>(begin(), end(), a);
    }
#else
    std::basic_string<charT, traits> to_string() const {
            return std::basic_string<charT, traits>(begin(), end());
            }

        template<typename Allocator>
        std::basic_string<charT, traits, Allocator> to_string(const Allocator& a) const {
            return std::basic_string<charT, traits, Allocator>(begin(), end(), a);
            }
#endif

    size_type copy(charT* s, size_type n, size_type pos=0) const {
        if (pos > size())
            throw std::out_of_range("string_view::copy" );
        size_type rlen = (std::min)(n, len_ - pos);
        // use std::copy(begin() + pos, begin() + pos + rlen, s) rather than
        // std::copy_n(begin() + pos, rlen, s) to support pre-C++11 standard libraries
        std::copy(begin() + pos, begin() + pos + rlen, s);
        return rlen;
    }

    STX_CONSTEXPR14 basic_string_view substr(size_type pos, size_type n=npos) const {
        if ( pos > size())
            throw std::out_of_range ( "string_view::substr" );
        if (n == npos || pos + n > size())
            n = size () - pos;
        return basic_string_view(data() + pos, n);
    }

    STX_CONSTEXPR14 int compare(basic_string_view x) const noexcept {
        const int cmp = traits::compare(ptr_, x.ptr_, (std::min)(len_, x.len_));
        return cmp != 0 ? cmp : (len_ == x.len_ ? 0 : len_ < x.len_ ? -1 : 1);
    }

    STX_CONSTEXPR14 int compare(size_type pos1, size_type n1, basic_string_view x)
    const noexcept {
        return substr(pos1, n1).compare(x);
    }

    STX_CONSTEXPR14 int compare(size_type pos1, size_type n1,
                                      basic_string_view x, size_type pos2, size_type n2) const {
        return substr(pos1, n1).compare(x.substr(pos2, n2));
    }

    STX_CONSTEXPR14 int compare(const charT* x) const {
        return compare(basic_string_view(x));
    }

    STX_CONSTEXPR14 int compare(size_type pos1, size_type n1, const charT* x) const {
        return substr(pos1, n1).compare(basic_string_view(x));
    }

    STX_CONSTEXPR14 int compare(size_type pos1, size_type n1,
                                      const charT* x, size_type n2) const {
        return substr(pos1, n1).compare(basic_string_view(x, n2));
    }

    //  Searches
    constexpr bool starts_with(charT c) const noexcept {              // Boost extension
        return !empty() && traits::eq(c, front());
    }

    constexpr bool starts_with(basic_string_view x) const noexcept {  // Boost extension
        return len_ >= x.len_ && traits::compare(ptr_, x.ptr_, x.len_) == 0;
    }

    constexpr bool ends_with(charT c) const noexcept {                // Boost extension
        return !empty() && traits::eq(c, back());
    }

    constexpr bool ends_with(basic_string_view x) const noexcept {    // Boost extension
        return len_ >= x.len_ &&
                traits::compare(ptr_ + len_ - x.len_, x.ptr_, x.len_) == 0;
    }

    //  find
    STX_CONSTEXPR14 size_type find(basic_string_view s, size_type pos = 0) const noexcept {
        if (pos > size())
            return npos;
        if (s.empty())
            return pos;
        const_iterator iter = std::search(this->cbegin() + pos, this->cend(),
                                          s.cbegin (), s.cend (), traits::eq);
        return iter == this->cend () ? npos : std::distance(this->cbegin (), iter);
    }
    STX_CONSTEXPR14 size_type find(charT c, size_type pos = 0) const noexcept
    { return find(basic_string_view(&c, 1), pos); }
    STX_CONSTEXPR14 size_type find(const charT* s, size_type pos, size_type n) const noexcept
    { return find(basic_string_view(s, n), pos); }
    STX_CONSTEXPR14 size_type find(const charT* s, size_type pos = 0) const noexcept
    { return find(basic_string_view(s), pos); }

    //  rfind
    STX_CONSTEXPR14 size_type rfind(basic_string_view s, size_type pos = npos) const noexcept {
        if (len_ < s.len_)
            return npos;
        if (pos > len_ - s.len_)
            pos = len_ - s.len_;
        if (s.len_ == 0u)     // an empty string is always found
            return pos;
        for (const charT* cur = ptr_ + pos; ; --cur) {
            if (traits::compare(cur, s.ptr_, s.len_) == 0)
                return cur - ptr_;
            if (cur == ptr_)
                return npos;
        };
    }
    STX_CONSTEXPR14 size_type rfind(charT c, size_type pos = npos) const noexcept
    { return rfind(basic_string_view(&c, 1), pos); }
    STX_CONSTEXPR14 size_type rfind(const charT* s, size_type pos, size_type n) const noexcept
    { return rfind(basic_string_view(s, n), pos); }
    STX_CONSTEXPR14 size_type rfind(const charT* s, size_type pos = npos) const noexcept
    { return rfind(basic_string_view(s), pos); }

    //  find_first_of
    STX_CONSTEXPR14 size_type find_first_of(basic_string_view s, size_type pos = 0) const noexcept {
        if (pos >= len_ || s.len_ == 0)
            return npos;
        const_iterator iter = std::find_first_of
                (this->cbegin () + pos, this->cend (), s.cbegin (), s.cend (), traits::eq);
        return iter == this->cend () ? npos : std::distance ( this->cbegin (), iter );
    }
    STX_CONSTEXPR14 size_type find_first_of(charT c, size_type pos = 0) const noexcept
    { return find_first_of(basic_string_view(&c, 1), pos); }
    STX_CONSTEXPR14 size_type find_first_of(const charT* s, size_type pos, size_type n) const noexcept
    { return find_first_of(basic_string_view(s, n), pos); }
    STX_CONSTEXPR14 size_type find_first_of(const charT* s, size_type pos = 0) const noexcept
    { return find_first_of(basic_string_view(s), pos); }

    //  find_last_of
    STX_CONSTEXPR14 size_type find_last_of(basic_string_view s, size_type pos = npos) const noexcept {
        if (s.len_ == 0u)
            return npos;
        if (pos >= len_)
            pos = 0;
        else
            pos = len_ - (pos+1);
        const_reverse_iterator iter = std::find_first_of
                ( this->crbegin () + pos, this->crend (), s.cbegin (), s.cend (), traits::eq );
        return iter == this->crend () ? npos : reverse_distance ( this->crbegin (), iter);
    }
    STX_CONSTEXPR14 size_type find_last_of(charT c, size_type pos = npos) const noexcept
    { return find_last_of(basic_string_view(&c, 1), pos); }
    STX_CONSTEXPR14 size_type find_last_of(const charT* s, size_type pos, size_type n) const noexcept
    { return find_last_of(basic_string_view(s, n), pos); }
    STX_CONSTEXPR14 size_type find_last_of(const charT* s, size_type pos = npos) const noexcept
    { return find_last_of(basic_string_view(s), pos); }

    //  find_first_not_of
    STX_CONSTEXPR14 size_type find_first_not_of(basic_string_view s, size_type pos = 0) const noexcept {
        if (pos >= len_)
            return npos;
        if (s.len_ == 0)
            return pos;
        const_iterator iter = find_not_of ( this->cbegin () + pos, this->cend (), s );
        return iter == this->cend () ? npos : std::distance ( this->cbegin (), iter );
    }
    STX_CONSTEXPR14 size_type find_first_not_of(charT c, size_type pos = 0) const noexcept
    { return find_first_not_of(basic_string_view(&c, 1), pos); }
    STX_CONSTEXPR14 size_type find_first_not_of(const charT* s, size_type pos, size_type n) const noexcept
    { return find_first_not_of(basic_string_view(s, n), pos); }
    STX_CONSTEXPR14 size_type find_first_not_of(const charT* s, size_type pos = 0) const noexcept
    { return find_first_not_of(basic_string_view(s), pos); }

    //  find_last_not_of
    STX_CONSTEXPR14 size_type find_last_not_of(basic_string_view s, size_type pos = npos) const noexcept {
        if (pos >= len_)
            pos = len_ - 1;
        if (s.len_ == 0u)
            return pos;
        pos = len_ - (pos+1);
        const_reverse_iterator iter = find_not_of ( this->crbegin () + pos, this->crend (), s );
        return iter == this->crend () ? npos : reverse_distance ( this->crbegin (), iter );
    }
    STX_CONSTEXPR14 size_type find_last_not_of(charT c, size_type pos = npos) const noexcept
    { return find_last_not_of(basic_string_view(&c, 1), pos); }
    STX_CONSTEXPR14 size_type find_last_not_of(const charT* s, size_type pos, size_type n) const noexcept
    { return find_last_not_of(basic_string_view(s, n), pos); }
    STX_CONSTEXPR14 size_type find_last_not_of(const charT* s, size_type pos = npos) const noexcept
    { return find_last_not_of(basic_string_view(s), pos); }

private:
    template <typename r_iter>
    size_type reverse_distance(r_iter first, r_iter last) const noexcept {
        // Portability note here: std::distance is not NOEXCEPT, but calling it with a string_view::reverse_iterator will not throw.
        return len_ - 1 - std::distance ( first, last );
    }

    template <typename Iterator>
    Iterator find_not_of(Iterator first, Iterator last, basic_string_view s) const noexcept {
        for (; first != last ; ++first)
            if ( 0 == traits::find(s.ptr_, s.len_, *first))
                return first;
        return last;
    }

    const charT *ptr_;
    std::size_t len_;
};


//  Comparison operators
//  Equality
template<typename charT, typename traits>
inline bool operator==(basic_string_view<charT, traits> x,
                       basic_string_view<charT, traits> y) noexcept {
    if (x.size () != y.size ()) return false;
    return x.compare(y) == 0;
}

//  Inequality
template<typename charT, typename traits>
inline bool operator!=(basic_string_view<charT, traits> x,
                       basic_string_view<charT, traits> y) noexcept {
    if ( x.size () != y.size ()) return true;
    return x.compare(y) != 0;
}

//  Less than
template<typename charT, typename traits>
inline bool operator<(basic_string_view<charT, traits> x,
                      basic_string_view<charT, traits> y) noexcept {
    return x.compare(y) < 0;
}

//  Greater than
template<typename charT, typename traits>
inline bool operator>(basic_string_view<charT, traits> x,
                      basic_string_view<charT, traits> y) noexcept {
    return x.compare(y) > 0;
}

//  Less than or equal to
template<typename charT, typename traits>
inline bool operator<=(basic_string_view<charT, traits> x,
                       basic_string_view<charT, traits> y) noexcept {
    return x.compare(y) <= 0;
}

//  Greater than or equal to
template<typename charT, typename traits>
inline bool operator>=(basic_string_view<charT, traits> x,
                       basic_string_view<charT, traits> y) noexcept {
    return x.compare(y) >= 0;
}

// "sufficient additional overloads of comparison functions"
template<typename charT, typename traits, typename Allocator>
inline bool operator==(basic_string_view<charT, traits> x,
                       const std::basic_string<charT, traits, Allocator> & y) noexcept {
    return x == basic_string_view<charT, traits>(y);
}

template<typename charT, typename traits, typename Allocator>
inline bool operator==(const std::basic_string<charT, traits, Allocator> & x,
                       basic_string_view<charT, traits> y) noexcept {
    return basic_string_view<charT, traits>(x) == y;
}

template<typename charT, typename traits>
inline bool operator==(basic_string_view<charT, traits> x,
                       const charT * y) noexcept {
    return x == basic_string_view<charT, traits>(y);
}

template<typename charT, typename traits>
inline bool operator==(const charT * x,
                       basic_string_view<charT, traits> y) noexcept {
    return basic_string_view<charT, traits>(x) == y;
}

template<typename charT, typename traits, typename Allocator>
inline bool operator!=(basic_string_view<charT, traits> x,
                       const std::basic_string<charT, traits, Allocator> & y) noexcept {
    return x != basic_string_view<charT, traits>(y);
}

template<typename charT, typename traits, typename Allocator>
inline bool operator!=(const std::basic_string<charT, traits, Allocator> & x,
                       basic_string_view<charT, traits> y) noexcept {
    return basic_string_view<charT, traits>(x) != y;
}

template<typename charT, typename traits>
inline bool operator!=(basic_string_view<charT, traits> x,
                       const charT * y) noexcept {
    return x != basic_string_view<charT, traits>(y);
}

template<typename charT, typename traits>
inline bool operator!=(const charT * x,
                       basic_string_view<charT, traits> y) noexcept {
    return basic_string_view<charT, traits>(x) != y;
}

template<typename charT, typename traits, typename Allocator>
inline bool operator<(basic_string_view<charT, traits> x,
                      const std::basic_string<charT, traits, Allocator> & y) noexcept {
    return x < basic_string_view<charT, traits>(y);
}

template<typename charT, typename traits, typename Allocator>
inline bool operator<(const std::basic_string<charT, traits, Allocator> & x,
                      basic_string_view<charT, traits> y) noexcept {
    return basic_string_view<charT, traits>(x) < y;
}

template<typename charT, typename traits>
inline bool operator<(basic_string_view<charT, traits> x,
                      const charT * y) noexcept {
    return x < basic_string_view<charT, traits>(y);
}

template<typename charT, typename traits>
inline bool operator<(const charT * x,
                      basic_string_view<charT, traits> y) noexcept {
    return basic_string_view<charT, traits>(x) < y;
}

template<typename charT, typename traits, typename Allocator>
inline bool operator>(basic_string_view<charT, traits> x,
                      const std::basic_string<charT, traits, Allocator> & y) noexcept {
    return x > basic_string_view<charT, traits>(y);
}

template<typename charT, typename traits, typename Allocator>
inline bool operator>(const std::basic_string<charT, traits, Allocator> & x,
                      basic_string_view<charT, traits> y) noexcept {
    return basic_string_view<charT, traits>(x) > y;
}

template<typename charT, typename traits>
inline bool operator>(basic_string_view<charT, traits> x,
                      const charT * y) noexcept {
    return x > basic_string_view<charT, traits>(y);
}

template<typename charT, typename traits>
inline bool operator>(const charT * x,
                      basic_string_view<charT, traits> y) noexcept {
    return basic_string_view<charT, traits>(x) > y;
}

template<typename charT, typename traits, typename Allocator>
inline bool operator<=(basic_string_view<charT, traits> x,
                       const std::basic_string<charT, traits, Allocator> & y) noexcept {
    return x <= basic_string_view<charT, traits>(y);
}

template<typename charT, typename traits, typename Allocator>
inline bool operator<=(const std::basic_string<charT, traits, Allocator> & x,
                       basic_string_view<charT, traits> y) noexcept {
    return basic_string_view<charT, traits>(x) <= y;
}

template<typename charT, typename traits>
inline bool operator<=(basic_string_view<charT, traits> x,
                       const charT * y) noexcept {
    return x <= basic_string_view<charT, traits>(y);
}

template<typename charT, typename traits>
inline bool operator<=(const charT * x,
                       basic_string_view<charT, traits> y) noexcept {
    return basic_string_view<charT, traits>(x) <= y;
}

template<typename charT, typename traits, typename Allocator>
inline bool operator>=(basic_string_view<charT, traits> x,
                       const std::basic_string<charT, traits, Allocator> & y) noexcept {
    return x >= basic_string_view<charT, traits>(y);
}

template<typename charT, typename traits, typename Allocator>
inline bool operator>=(const std::basic_string<charT, traits, Allocator> & x,
                       basic_string_view<charT, traits> y) noexcept {
    return basic_string_view<charT, traits>(x) >= y;
}

template<typename charT, typename traits>
inline bool operator>=(basic_string_view<charT, traits> x,
                       const charT * y) noexcept {
    return x >= basic_string_view<charT, traits>(y);
}

template<typename charT, typename traits>
inline bool operator>=(const charT * x,
                       basic_string_view<charT, traits> y) noexcept {
    return basic_string_view<charT, traits>(x) >= y;
}

namespace detail {

template<class charT, class traits>
inline void sv_insert_fill_chars(std::basic_ostream<charT, traits>& os, std::size_t n) {
    enum { chunk_size = 8 };
    charT fill_chars[chunk_size];
    std::fill_n(fill_chars, static_cast< std::size_t >(chunk_size), os.fill());
    for (; n >= chunk_size && os.good(); n -= chunk_size)
        os.write(fill_chars, static_cast< std::size_t >(chunk_size));
    if (n > 0 && os.good())
        os.write(fill_chars, n);
}

template<class charT, class traits>
void sv_insert_aligned(std::basic_ostream<charT, traits>& os, const basic_string_view<charT,traits>& str) {
    const std::size_t size = str.size();
    const std::size_t alignment_size = static_cast< std::size_t >(os.width()) - size;
    const bool align_left = (os.flags() & std::basic_ostream<charT, traits>::adjustfield) == std::basic_ostream<charT, traits>::left;
    if (!align_left) {
        detail::sv_insert_fill_chars(os, alignment_size);
        if (os.good())
            os.write(str.data(), size);
    }
    else {
        os.write(str.data(), size);
        if (os.good())
            detail::sv_insert_fill_chars(os, alignment_size);
    }
}

} // namespace detail

// Inserter
template<class charT, class traits>
inline std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& os,
           const basic_string_view<charT,traits>& str) {
    if (os.good()) {
        const std::size_t size = str.size();
        const std::size_t w = static_cast< std::size_t >(os.width());
        if (w <= size)
            os.write(str.data(), size);
        else
            detail::sv_insert_aligned(os, str);
        os.width(0);
    }
    return os;
}

#if 0
// numeric conversions
    //
    //  These are short-term implementations.
    //  In a production environment, I would rather avoid the copying.
    //
    inline int stoi (string_view str, size_t* idx=0, int base=10) {
        return std::stoi ( std::string(str), idx, base );
        }

    inline long stol (string_view str, size_t* idx=0, int base=10) {
        return std::stol ( std::string(str), idx, base );
        }

    inline unsigned long stoul (string_view str, size_t* idx=0, int base=10) {
        return std::stoul ( std::string(str), idx, base );
        }

    inline long long stoll (string_view str, size_t* idx=0, int base=10) {
        return std::stoll ( std::string(str), idx, base );
        }

    inline unsigned long long stoull (string_view str, size_t* idx=0, int base=10) {
        return std::stoull ( std::string(str), idx, base );
        }

    inline float stof (string_view str, size_t* idx=0) {
        return std::stof ( std::string(str), idx );
        }

    inline double stod (string_view str, size_t* idx=0) {
        return std::stod ( std::string(str), idx );
        }

    inline long double stold (string_view str, size_t* idx=0)  {
        return std::stold ( std::string(str), idx );
        }

    inline int  stoi (wstring_view str, size_t* idx=0, int base=10) {
        return std::stoi ( std::wstring(str), idx, base );
        }

    inline long stol (wstring_view str, size_t* idx=0, int base=10) {
        return std::stol ( std::wstring(str), idx, base );
        }

    inline unsigned long stoul (wstring_view str, size_t* idx=0, int base=10) {
        return std::stoul ( std::wstring(str), idx, base );
        }

    inline long long stoll (wstring_view str, size_t* idx=0, int base=10) {
        return std::stoll ( std::wstring(str), idx, base );
        }

    inline unsigned long long stoull (wstring_view str, size_t* idx=0, int base=10) {
        return std::stoull ( std::wstring(str), idx, base );
        }

    inline float  stof (wstring_view str, size_t* idx=0) {
        return std::stof ( std::wstring(str), idx );
        }

    inline double stod (wstring_view str, size_t* idx=0) {
        return std::stod ( std::wstring(str), idx );
        }

    inline long double stold (wstring_view str, size_t* idx=0) {
        return std::stold ( std::wstring(str), idx );
        }
#endif

using string_view = basic_string_view<char, std::char_traits<char>>;
using u16string_view = basic_string_view<char16_t, std::char_traits<char16_t>>;
using u32string_view = basic_string_view<char32_t, std::char_traits<char32_t>>;
using wstring_view = basic_string_view<wchar_t, std::char_traits<wchar_t>>;

} // end namespace

#if 0
namespace std {
    // Hashing
    template<> struct hash<STX_NAMESPACE_NAME::string_view>;
    template<> struct hash<STX_NAMESPACE_NAME::u16string_view>;
    template<> struct hash<STX_NAMESPACE_NAME::u32string_view>;
    template<> struct hash<STX_NAMESPACE_NAME::wstring_view>;
}
#endif

#endif // STX_HAVE_STD_STRING_VIEW

#endif // STX_STRING_VIEW_HPP_INCLUDED
