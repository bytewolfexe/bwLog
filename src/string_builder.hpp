#ifndef BWTK_STRING_BUILDER_HPP
#define BWTK_STRING_BUILDER_HPP

#include <cstdint>
#include <sstream>

namespace bwtk
{
    template <size_t S>
    class string_builder
    {
    public:
        string_builder();
        ~string_builder() = default;

        template <typename T>
        void write(const T& value);

        void clear();

        const char* get() const;
    private:
        char buffer_[S];
        std::stringstream writer_;
    };


    template <size_t S>
    string_builder<S>::string_builder()
        : buffer_ {}, writer_()
    {
        static_assert(S != 0, "Incorrect size of string_builder!");

        writer_.rdbuf()->pubsetbuf(buffer_, S - 1);
        clear();
    }

    template <size_t S>
    void string_builder<S>::clear()
    {
        for(auto& c : buffer_)
        {
            c = 0;
        }
    }

    template <size_t S>
    template <typename T>
    void string_builder<S>::write(const T &value)
    {
        writer_ << value;
    }

    template <size_t S>
    const char* string_builder<S>::get() const
    {
        return static_cast<const char*>(&buffer_[0]);
    }
}

#endif //BWTK_STRING_BUILDER_HPP
