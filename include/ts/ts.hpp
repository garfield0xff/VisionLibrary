#ifndef _VL_TEST_H_
#define _VL_TEST_H_

#include <gtest/gtest.h>

namespace vl_test
{
    int main(int argc, char** argv) {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }
}// namespace vl_test

#endif // _VL_TEST_H_