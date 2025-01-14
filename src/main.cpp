#include <zephyr/kernel.h>
#include <zephyr/ztest.h>
#include "axo/drivers/ads1299/Ads1299.hpp"

namespace {
    std::shared_ptr<Tosoo::Ads1299> ads1299;
}

ZTEST_SUITE(lead_off_tests, NULL, NULL, NULL, NULL, NULL);

/* Test lead-off detection configuration */
ZTEST(lead_off_tests, test_lead_off_config) {
    // Initialize ADS1299
    ads1299 = std::make_shared<Tosoo::Ads1299>();
    zassert_true(ads1299->Init(), "ADS1299 initialization failed");

    // Test case 1: Configure 6nA DC test signal
    std::array<uint8_t, 8> test_signals = {
        static_cast<uint8_t>(Tosoo::ADS1299TestSignal::kINxP_Enable),  // Channel 1
        static_cast<uint8_t>(Tosoo::ADS1299TestSignal::kINxN_Enable),  // Channel 2
        static_cast<uint8_t>(Tosoo::ADS1299TestSignal::kINxP_Enable | Tosoo::ADS1299TestSignal::kINxN_Enable),  // Channel 3
        static_cast<uint8_t>(Tosoo::ADS1299TestSignal::kINxP_Enable | Tosoo::ADS1299TestSignal::kPolarityFlip),  // Channel 4
        0,  // Channel 5 disabled
        0,  // Channel 6 disabled
        0,  // Channel 7 disabled
        0   // Channel 8 disabled
    };

    zassert_true(ads1299->EnableChannelTesting(
        Tosoo::ADS1299TestCurrent::k6nA,
        Tosoo::ADS1299TestSignalFreq::kDC,
        test_signals
    ), "Failed to configure lead-off detection with 6nA DC");

    // Test case 2: Configure 24uA AC test signal at 7.8Hz
    test_signals = {
        static_cast<uint8_t>(Tosoo::ADS1299TestSignal::kINxP_Enable | Tosoo::ADS1299TestSignal::kINxN_Enable),  // Channel 1
        static_cast<uint8_t>(Tosoo::ADS1299TestSignal::kINxP_Enable | Tosoo::ADS1299TestSignal::kINxN_Enable),  // Channel 2
        0,  // Channel 3 disabled
        0,  // Channel 4 disabled
        0,  // Channel 5 disabled
        0,  // Channel 6 disabled
        0,  // Channel 7 disabled
        0   // Channel 8 disabled
    };

    zassert_true(ads1299->EnableChannelTesting(
        Tosoo::ADS1299TestCurrent::k24uA,
        Tosoo::ADS1299TestSignalFreq::kAC_7Hz8,
        test_signals
    ), "Failed to configure lead-off detection with 24uA AC");

    // Test case 3: Verify error handling for invalid configuration
    std::array<uint8_t, 8> invalid_signals = {
        0xFF,  // Invalid signal configuration
        0,
        0,
        0,
        0,
        0,
        0,
        0
    };

    zassert_false(ads1299->EnableChannelTesting(
        Tosoo::ADS1299TestCurrent::k6nA,
        Tosoo::ADS1299TestSignalFreq::kDC,
        invalid_signals
    ), "Invalid configuration not properly handled");
}

int main(void) {
    ztest_run_test_suite(lead_off_tests);
    return 0;
}
