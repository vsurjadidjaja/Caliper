#include "caliper/common/RuntimeConfig.h"

#include "gtest/gtest.h"

using namespace cali;

namespace cali
{

extern void clear_caliper_runtime_config();

}

namespace
{

const ConfigSet::Entry test_configdata[] = { 
    { "string_val",  CALI_TYPE_STRING, "string-default",
      "Description for the string config entry",
      "Long description for the string config entry"
    },
    { "list_val",    CALI_TYPE_STRING, "first, second, \"third,but not fourth\"",
      "Description of the list config entry",
      "Long description for the list config entry"
    },
    { "int_val",     CALI_TYPE_INT,    "1337",
      "Description for the int config entry",
      "Long description for the int config entry"
    },
    { "another_int", CALI_TYPE_INT,    "4242",
      "Description for another int config entry",
      "Long description for another int config entry"
    },
    
    ConfigSet::Terminator
};

} // namespace [anonymous]


TEST(RuntimeConfigTest, DefineProfile) {
    clear_caliper_runtime_config();
    
    const char* my_profile[][2] =
        { { "CALI_TEST_INT_VAL",    "42"             },
          { "CALI_TEST_STRING_VAL", "my test string" },
          { NULL, NULL }
        };

    cali::RuntimeConfig::define_profile("my profile", my_profile);
    cali::RuntimeConfig::set("CALI_CONFIG_PROFILE", "my profile");

    ConfigSet config = cali::RuntimeConfig::init("test", ::test_configdata);

    EXPECT_EQ(cali::RuntimeConfig::get("config", "profile").to_string(), std::string("my profile"));

    EXPECT_EQ(config.get("string_val").to_string(), std::string("my test string"));
    EXPECT_EQ(config.get("int_val").to_int(),     42);
    EXPECT_EQ(config.get("another_int").to_int(), 4242);
}

TEST(RuntimeConfigTest, ConfigFile) {
    clear_caliper_runtime_config();
    
    cali::RuntimeConfig::set("CALI_CONFIG_FILE", "caliper-common_test.config");
    
    cali::RuntimeConfig::preset("CALI_TEST_STRING_VAL", "wrong value!");
    cali::RuntimeConfig::set("CALI_TEST_INT_VAL", "42");
    
    ConfigSet config = cali::RuntimeConfig::init("test", ::test_configdata);

    EXPECT_EQ(config.get("string_val").to_string(), std::string("file-profile1-string"));
    EXPECT_EQ(config.get("int_val").to_int(), 42);
    EXPECT_EQ(config.get("another_int").to_int(), 4242);
}

TEST(RuntimeConfigTest, ConfigFileProfile2) {
    clear_caliper_runtime_config();
    
    cali::RuntimeConfig::preset("CALI_CONFIG_FILE", "caliper-common_test.config");
    cali::RuntimeConfig::set("CALI_CONFIG_PROFILE", "file-profile2");
    
    ConfigSet config = cali::RuntimeConfig::init("test", ::test_configdata);

    EXPECT_EQ(config.get("string_val").to_string(), std::string("string-default"));
    EXPECT_EQ(config.get("int_val").to_int(), 42);
}