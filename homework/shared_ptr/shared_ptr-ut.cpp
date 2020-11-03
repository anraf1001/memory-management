#include <gtest/gtest.h>

#include "shared_ptr.hpp"
#include "weak_ptr.hpp"

constexpr int initialValue = 5;

struct A {
    int foo() { return initialValue; }
};

struct SharedPtrTest : ::testing::Test {
    SharedPtrTest()
        : ptr(new int{initialValue}) {}

    cs::shared_ptr<int> ptr;
};

TEST_F(SharedPtrTest, shouldDereferenceSharedPtr) {
    ASSERT_EQ(*ptr, initialValue);
}

TEST_F(SharedPtrTest, shouldCopySharedPtr) {
    constexpr size_t numOfPtrs = 3;
    auto ptr2(ptr);
    auto ptr3 = ptr;

    ASSERT_EQ(*ptr, *ptr2);
    ASSERT_EQ(*ptr, *ptr3);

    ASSERT_EQ(ptr.use_count(), numOfPtrs);
    ASSERT_EQ(ptr2.use_count(), numOfPtrs);
    ASSERT_EQ(ptr3.use_count(), numOfPtrs);
}

TEST_F(SharedPtrTest, shouldMoveSharedPtr) {
    auto ptr2(std::move(ptr));
    ASSERT_EQ(*ptr2, initialValue);
    ASSERT_EQ(ptr2.use_count(), 1);

    auto ptr3 = std::move(ptr2);
    ASSERT_EQ(*ptr3, initialValue);
    ASSERT_EQ(ptr3.use_count(), 1);
}

TEST_F(SharedPtrTest, shouldConvertSharedPtrToBool) {
    cs::shared_ptr<int> ptr2;

    ASSERT_TRUE(ptr);
    ASSERT_FALSE(ptr2);
}

TEST_F(SharedPtrTest, shouldResetSharedPtr) {
    ptr.reset();

    ASSERT_EQ(ptr.get(), nullptr);

    constexpr int valueAfterReset = 6;
    int* newPtr = new int{valueAfterReset};

    ptr.reset(newPtr);

    ASSERT_EQ(*ptr, valueAfterReset);
}

TEST(SharedPtrArrowOperator, shouldUseArrowOperator) {
    cs::shared_ptr<A> ptr(new A{});

    ASSERT_EQ(ptr->foo(), initialValue);
}

TEST_F(SharedPtrTest, shouldCreateSharedPtrFromWeakPtr) {
    cs::weak_ptr<int> wptr{ptr};

    cs::shared_ptr<int> sptr{wptr};

    ASSERT_EQ(sptr.get(), ptr.get());
}

TEST_F(SharedPtrTest, shouldIncrementCounterIfCopyAssignmentIsUsed) {
    constexpr int expectedCounterValue = 2;
    cs::shared_ptr<int> copyPtr;
    copyPtr = ptr;
    ASSERT_EQ(copyPtr.use_count(), expectedCounterValue);
}

TEST_F(SharedPtrTest, shouldCounterConstIfMoveAssignmentIsUsed){
    constexpr int expectedCounterValue = 1;
    cs::shared_ptr<int> movePtr;
    movePtr = std::move(ptr);
    ASSERT_EQ(movePtr.use_count(), expectedCounterValue);
}
