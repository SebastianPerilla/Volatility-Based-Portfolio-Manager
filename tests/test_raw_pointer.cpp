#include "gtest/gtest.h"
#include "is_raw_pointer.h"


using namespace homework;

TEST(IsRawPointer, Int) {
    ASSERT_FALSE(IsRawPointer<int>::value);
}

TEST(IsRawPointer, IntPointer) {
    ASSERT_TRUE(IsRawPointer<int*>::value);
}

TEST(IsRawPointer, IntConstPointer) {
    ASSERT_TRUE(IsRawPointer<const int*>::value);
}

TEST(IsRawPointer, IntReference) {
    ASSERT_FALSE(IsRawPointer<int&>::value);
}

TEST(IsRawPointer, String){
    ASSERT_FALSE(IsRawPointer<std::string>::value);
}