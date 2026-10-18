#include <gtest/gtest.h>

#include "vyshkat/domain/Bike.hpp"
#include "vyshkat/services/BeginnerPolicy.hpp"

using vyshkat::BeginnerPolicy;
using vyshkat::Bike;


TEST(BeginnerPolicyTest, SimplicityFiveIsNotSuitable) {
    BeginnerPolicy policy;
    EXPECT_FALSE(policy.isSuitable(Bike("BK-005", "Велосипед", 5)));
}

TEST(BeginnerPolicyTest, SimplicitySixIsSuitable) {
    BeginnerPolicy policy;
    EXPECT_TRUE(policy.isSuitable(Bike("BK-006", "Велосипед", 6)));
}

TEST(BeginnerPolicyTest, SimplicityTenIsSuitable) {
    BeginnerPolicy policy;
    EXPECT_TRUE(policy.isSuitable(Bike("BK-010", "Велосипед", 10)));
}

TEST(BeginnerPolicyTest, UsesThresholdFromConstructor) {
    BeginnerPolicy strictPolicy(8);
    EXPECT_FALSE(strictPolicy.isSuitable(Bike("BK-007", "Велосипед", 7)));
    EXPECT_TRUE(strictPolicy.isSuitable(Bike("BK-008", "Велосипед", 8)));
}
