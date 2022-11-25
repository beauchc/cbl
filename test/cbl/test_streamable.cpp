#include <cbl/streamable.h>

#include <sstream>

#include <gtest/gtest.h>

//------------------------------------------------------------------------------
//
TEST(streamable, simple) {
    struct MyStruct {
        int   i;
        float f;
    };

    std::stringstream ss;

    MyStruct ms{42, 84.84f};
    ss << cbl::make_streamable([&ms](auto& os) {
        os << "MyStruct[ " << ms.i << ", " << ms.f << " ]";
    });
    EXPECT_STREQ("MyStruct[ 42, 84.84 ]", ss.str().c_str());
}

//------------------------------------------------------------------------------
//
TEST(streamable, lambda) {
    int v = 1;

    std::stringstream ss;
    ss << cbl::make_streamable([&v](auto& os) { os << "val " << v++; });
    ss << ", ";

    auto lambda = [&v](std::ostream& os) { os << "ref " << v++; };
    ss << cbl::make_streamable(lambda);
    ss << ", ";

    auto const clambda = [&v](std::ostream& os) { os << "cref " << v++; };
    ss << cbl::make_streamable(clambda);

    EXPECT_STREQ("val 1, ref 2, cref 3", ss.str().c_str());
}

//------------------------------------------------------------------------------
//
TEST(streamable, callable) {
    struct Callable {
        explicit Callable(int n) : m_n(n) {}
        void operator()(std::ostream& os) const { os << "const " << m_n; }
        void operator()(std::ostream& os) { os << "non const " << m_n; }
        int m_n;
    };

    std::stringstream ss;
    ss << cbl::make_streamable(Callable(1)) << ", ";

    Callable callable{2};
    ss << cbl::make_streamable(callable) << ", ";

    Callable const ccallable{3};
    ss << cbl::make_streamable(ccallable);

    EXPECT_STREQ("const 1, const 2, const 3", ss.str().c_str());
}

//------------------------------------------------------------------------------
//
TEST(streamable, function_pointer) {
    struct S {
        static void fp1(std::ostream& os) { os << "fp 1"; }
        static void fp2(std::ostream& os) { os << "fp 2"; }
    };

    std::stringstream ss;
    ss << cbl::make_streamable(&S::fp1) << ", ";

    auto fp = &S::fp2;
    ss << cbl::make_streamable(fp);

    EXPECT_STREQ("fp 1, fp 2", ss.str().c_str());
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
TEST(streamable, constexpr) {
    struct Os {
        static constexpr Os get(int a, int b, int c, int d) {
            return Os{} << a
                        << cbl::make_streamable([=](auto& os) { os << b << c; })
                        << d;
        }
        constexpr Os() = default;
        constexpr Os& operator<<(int value) {
            m_value *= 10;
            m_value += value;
            return *this;
        }
        constexpr int get() const { return m_value; }
        int           m_value = 0;
    };

    constexpr auto v1 = Os::get(1, 2, 3, 4).get();
    static_assert(1234 == v1, "");
    EXPECT_EQ(1234, v1);

    constexpr auto v2 = Os::get(5, 6, 7, 8).get();
    static_assert(5678 == v2, "");
    EXPECT_EQ(5678, v2);
}
