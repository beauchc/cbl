#include <cbl/streamable.h>

#include <sstream>

#include <gtest/gtest.h>

//------------------------------------------------------------------------------
//
TEST(streamable, simple) {
    struct MyStruct {
        int   m_int;
        float m_flt;
    };

    std::stringstream ss;

    MyStruct myStruct{42, 84.84f};
    ss << cbl::make_streamable([&myStruct](auto& os) {
        os << "MyStruct[ " << myStruct.m_int << ", " << myStruct.m_flt << " ]";
    });

    EXPECT_STREQ("MyStruct[ 42, 84.84 ]", ss.str().c_str());
}


//------------------------------------------------------------------------------
//
TEST(streamable, CustomOs) {
    struct CustomOs {
        CustomOs& operator<<(int value) {
            m_value *= 10;
            m_value += value;
            return *this;
        }
        int m_value = 0;
    };

    CustomOs myOs;
    myOs << cbl::make_streamable([](auto& os) { os << 1; });
    EXPECT_EQ(1, myOs.m_value);

    std::array<int, 3> myArray = {2, 3, 4};
    myOs << cbl::make_streamable([&myArray](auto& os) {
        for (auto const& v : myArray) os << v;
    });
    EXPECT_EQ(1234, myOs.m_value);

    myOs << cbl::make_streamable([v = 5](auto& os) {
        os << v << (v+1);
    }) << 7;
    EXPECT_EQ(1234567, myOs.m_value);
}

//------------------------------------------------------------------------------
//
struct ConstexprOs {
    constexpr ConstexprOs() = default;
    constexpr ConstexprOs& operator<<(int value) {
        m_value *= 10;
        m_value += value;
        return *this;
    }
    constexpr int get() const { return m_value; }
    int m_value = 0;
};
constexpr ConstexprOs create_constexpr_os() {
    ConstexprOs myOs;
    myOs << cbl::make_streamable([](auto& os) { os << 1 << 2 << 3; }) << 4;
    return myOs;
}

TEST(streamable, constexpr) {
    constexpr auto value = create_constexpr_os().get();
    static_assert(1234 == value, "");
    EXPECT_EQ(1234, value);
}
