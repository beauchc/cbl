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
    myOs << cbl::make_streamable([](auto& os) { os << 7; });
    EXPECT_EQ(7, myOs.m_value);

    std::array<int, 3> myArray = {1, 2, 3};
    myOs << cbl::make_streamable([&myArray](auto& os) {
        for (auto const& v : myArray) os << v;
    });
    EXPECT_EQ(7123, myOs.m_value);

    myOs << cbl::make_streamable([v = 8](auto& os) {
        os << v << (v+1);
    });
    EXPECT_EQ(712389, myOs.m_value);
}
