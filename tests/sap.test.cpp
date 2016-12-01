#include "sap.h"
#include <gtest/gtest.h>

int test_value = 0;

int catched_argc;
char** catched_argv;
sap_options_t* catched_options;

int default_handler(int argc, char* argv[], sap_options_t* options) {

    test_value += 1;

    return 123;

}

int specific_handler(int argc, char* argv[], sap_options_t* options) {

    test_value += 2;

    catched_argc = argc;
    catched_argv = argv;
    catched_options = options;

    return 321;

}

TEST(SAP, ExecuteNoArgumentsNoHandler) {

    sap_t* parser = sap_create();

    int argc = 0;
    char* argv[0];

    test_value = 0;
    ASSERT_EQ(0, test_value);

    int ret = sap_execute(parser, argc, argv);

    ASSERT_EQ(0, test_value);
    ASSERT_EQ(-1, ret);

    sap_destroy(parser);

}

TEST(SAP, ExecuteDefaultHandlerWithNoArguments) {

    sap_t* parser = sap_create();

    sap_set_default(parser, default_handler);

    int argc = 0;
    char* argv[0];

    test_value = 0;
    ASSERT_EQ(0, test_value);

    int ret = sap_execute(parser, argc, argv);

    ASSERT_EQ(1, test_value);
    ASSERT_EQ(123, ret);

    sap_destroy(parser);

}

TEST(SAP, ExecuteDefaultHandlerWithOneUnmatchedCommand) {

    sap_t* parser = sap_create();

    sap_set_default(parser, default_handler);

    int argc = 1;
    char* argv[1] = { (char*) "command" };

    test_value = 0;
    ASSERT_EQ(0, test_value);

    int ret = sap_execute(parser, argc, argv);

    ASSERT_EQ(1, test_value);
    ASSERT_EQ(123, ret);

    sap_destroy(parser);

}

TEST(SAP, ExecuteMatchedHandlerWithoutOptions) {

    sap_t* parser = sap_create();

    sap_set_default(parser, default_handler);
    sap_add_command(parser, (char*) "command", specific_handler);

    int argc = 1;
    char* argv[1] = { (char*) "command" };

    test_value = 0;
    ASSERT_EQ(0, test_value);

    int ret = sap_execute(parser, argc, argv);

    ASSERT_EQ(2, test_value);
    ASSERT_EQ(321, ret);
    ASSERT_EQ(0, catched_argc);

    sap_destroy(parser);

}

TEST(SAP, ExecuteMatchedHandlerWithOptions) {

    sap_t* parser = sap_create();

    sap_set_default(parser, default_handler);
    sap_add_command(parser, (char*) "command", specific_handler);

    int argc = 3;
    char* argv[3] = { 
        (char*) "command", 
        (char*) "--key=value", 
        (char*) "--flag" 
    };

    test_value = 0;
    ASSERT_EQ(0, test_value);

    int ret = sap_execute(parser, argc, argv);

    ASSERT_EQ(2, test_value);
    ASSERT_EQ(321, ret);
    ASSERT_EQ(0, catched_argc);

    ASSERT_TRUE(catched_options->list[0] != NULL);
    ASSERT_STREQ(catched_options->list[0]->label, "key");
    ASSERT_STREQ(catched_options->list[0]->value, "value");
    ASSERT_EQ(0, catched_options->list[0]->is_flag);

    ASSERT_TRUE(catched_options->list[1] != NULL);
    ASSERT_STREQ(catched_options->list[1]->label, "flag");
    ASSERT_EQ(1, catched_options->list[1]->is_flag);

    sap_destroy(parser);

}

TEST(SAP, ExecuteMatchedHandlerWithOptionsAndTrailingCommands) {

    sap_t* parser = sap_create();

    sap_set_default(parser, default_handler);
    sap_add_command(parser, (char*) "command", specific_handler);

    int argc = 5;
    char* argv[5] = { 
        (char*) "command", 
        (char*) "--key=value", 
        (char*) "--flag",
        (char*) "subcommand",
        (char*) "--suboption" 
    };

    test_value = 0;
    ASSERT_EQ(test_value, 0);

    int ret = sap_execute(parser, argc, argv);

    ASSERT_EQ(2, test_value);
    ASSERT_EQ(321, ret);
    ASSERT_EQ(2, catched_argc);
    ASSERT_STREQ("subcommand", catched_argv[0]);
    ASSERT_STREQ("--suboption", catched_argv[1]);

    ASSERT_TRUE(catched_options->list[0] != NULL);
    ASSERT_STREQ(catched_options->list[0]->label, "key");
    ASSERT_STREQ(catched_options->list[0]->value, "value");
    ASSERT_EQ(0, catched_options->list[0]->is_flag);

    ASSERT_TRUE(catched_options->list[1] != NULL);
    ASSERT_STREQ(catched_options->list[1]->label, "flag");
    ASSERT_EQ(1, catched_options->list[1]->is_flag);

    sap_destroy(parser);

}

TEST(SAP, ExecuteWithMergedOptions) {

    sap_t* parser = sap_create();

    sap_set_default(parser, default_handler);
    sap_add_command(parser, (char*) "command", specific_handler);

    int argc = 3;
    char* argv[3] = { 
        (char*) "command", 
        (char*) "--key=value", 
        (char*) "--flag"
    };

    test_value = 0;
    ASSERT_EQ(test_value, 0);

    sap_options_t* old_options = (sap_options_t*) calloc(1, sizeof(sap_options_t));

    sap_option_t opt_a = {
        (char*) "key",
        (char*) "previous_value",
        0
    };

    sap_option_t opt_b = {
        (char*) "new_key",
        NULL,
        1
    };

    old_options->list[0] = &opt_a;
    old_options->list[1] = &opt_b;
    old_options->list[2] = NULL;

    sap_execute_ex(parser, argc, argv, old_options);


    ASSERT_TRUE(catched_options->list[0] != NULL);
    ASSERT_STREQ(catched_options->list[0]->label, "key");
    ASSERT_STREQ(catched_options->list[0]->value, "previous_value");
    ASSERT_EQ(0, catched_options->list[0]->is_flag);

    ASSERT_TRUE(catched_options->list[1] != NULL);
    ASSERT_STREQ(catched_options->list[1]->label, "flag");
    ASSERT_EQ(1, catched_options->list[1]->is_flag);

    ASSERT_TRUE(catched_options->list[2] != NULL);
    ASSERT_STREQ(catched_options->list[2]->label, "new_key");
    ASSERT_EQ(1, catched_options->list[2]->is_flag);

    sap_destroy(parser);

}



class OptionsTest : public ::testing::Test {

    protected:
        sap_t* parser;

    OptionsTest() {
    
    }

    virtual void SetUp() {
    
        parser = sap_create();

        sap_add_command(parser, (char*) "command", specific_handler);

        int argc = 3;
        char* argv[3] = { 
            (char*) "command", 
            (char*) "--key=value", 
            (char*) "--flag" 
        };

        sap_execute(parser, argc, argv);

    }

    virtual void TearDown() {
    
        sap_destroy(parser);

    }

};

TEST_F(OptionsTest, CheckKeyValues) {

    /* we can assume that the specific_handler is beeing called */

    char* value_a = sap_option_get(catched_options, (char*) "key");
    char* value_b = sap_option_get(catched_options, (char*) "flag");
    char* value_c = sap_option_get(catched_options, (char*) "unknown"); 

    ASSERT_STREQ(value_a, "value");
    ASSERT_TRUE(value_b == NULL);
    ASSERT_TRUE(value_c == NULL);

}

TEST_F(OptionsTest, CheckFlags) {

    int flag_a = sap_option_enabled(catched_options, (char*) "key");
    int flag_b = sap_option_enabled(catched_options, (char*) "flag");
    int flag_c = sap_option_enabled(catched_options, (char*) "unknown");

    ASSERT_FALSE(flag_a);
    ASSERT_TRUE(flag_b);
    ASSERT_FALSE(flag_c);

}
