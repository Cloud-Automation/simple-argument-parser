#include "sap.h"
#include <gtest/gtest.h>

int call_count = 0;
int return_value = 0;

sap_option_list_t* catched_options = NULL;
sap_command_list_t* catched_commands = NULL;

int default_handler(sap_command_list_t* commands, sap_option_list_t* options) {

    call_count += 1;

    catched_options = options;
    catched_commands = commands;

    return return_value;

}

int specific_handler(sap_command_list_t* commands, sap_option_list_t* options) {

    call_count += 1;

    catched_options = options;
    catched_commands = commands;

    return return_value;

}

TEST(SAP, ExecuteNoArgumentsNoHandler) {


    int argc = 0;
    char* argv[0];

    sap_t parser;

    int retCreate = sap_init(&parser, argc, argv);

    ASSERT_EQ(0, retCreate);

    call_count = 0;
    return_value = 1;
    ASSERT_EQ(0, call_count);

    int ret = sap_execute(&parser);

    ASSERT_EQ(0, call_count);
    ASSERT_EQ(-1, ret);

    sap_free(&parser);

}

TEST(SAP, ExecuteDefaultHandlerWithNoArguments) {


    int argc = 0;
    char* argv[0];

    sap_t parser;

    sap_init(&parser, argc, argv);

    sap_set_default(&parser, default_handler);

    call_count = 0;
    return_value = 123;
    ASSERT_EQ(0, call_count);

    int ret = sap_execute(&parser);

    ASSERT_EQ(1, call_count);
    ASSERT_EQ(123, ret);

    sap_free(&parser);

}


TEST(SAP, ExecuteDefaultHandlerWithOneUnmatchedCommand) {

    int argc = 1;
    char* argv[1] = { (char*) "command" };

    sap_t parser;

    sap_init(&parser, argc, argv);

    sap_set_default(&parser, default_handler);

    call_count = 0;
    return_value = 321;
    ASSERT_EQ(0, call_count);

    int ret = sap_execute(&parser);

    ASSERT_EQ(1, call_count);
    ASSERT_EQ(321, ret);

    sap_free(&parser);

}


TEST(SAP, ExecuteMatchedHandlerWithoutOptions) {

    int argc = 2;
    char* argv[2] = { (char*) "filename", (char*) "command" };

    sap_t parser;
    
    sap_init(&parser, argc, argv);

    sap_set_default(&parser, default_handler);
    sap_add_command(&parser, (char*) "command", specific_handler);

    call_count = 0;
    return_value = 111;
    ASSERT_EQ(0, call_count);

    int ret = sap_execute(&parser);

    ASSERT_EQ(1, call_count);
    ASSERT_EQ(111, ret);

    sap_free(&parser);

}

TEST(SAP, ExecuteMatchedHandlerWithOptions) {

    int argc = 5;
    char* argv[5] = {
        (char*) "filename", 
        (char*) "command", 
        (char*) "-faultyCommand",
        (char*) "--key=value", 
        (char*) "--flag" 
    };

    sap_t parser;
    
    sap_init(&parser, argc, argv);

    sap_set_default(&parser, default_handler);
    sap_add_command(&parser, (char*) "command", specific_handler);

    call_count = 0;
    return_value = 321;
    ASSERT_EQ(0, call_count);

    int ret = sap_execute(&parser);

    ASSERT_EQ(1, call_count);
    ASSERT_EQ(321, ret);

    sap_option_t* first = sap_get_option_by_index(catched_options, 0);
    sap_option_t* second = sap_get_option_by_index(catched_options, 1);
    sap_option_t* third = sap_get_option_by_index(catched_options, 2);

    ASSERT_TRUE(first != NULL);
    ASSERT_TRUE(first->label != NULL);
    ASSERT_STREQ(first->label, "key");
    ASSERT_STREQ(first->value, "value");
    ASSERT_EQ(0, first->is_flag);

    ASSERT_TRUE(second != NULL);
    ASSERT_STREQ(second->label, "flag");
    ASSERT_EQ(1, second->is_flag);

    ASSERT_TRUE(third == NULL);

    sap_option_t* key_option = sap_get_option_by_key(catched_options, (char*) "key");
    sap_option_t* flag_option = sap_get_option_by_key(catched_options, (char*) "flag");
    sap_option_t* other_option = sap_get_option_by_key(catched_options, (char*) "other");

    ASSERT_TRUE(key_option == first);
    ASSERT_TRUE(flag_option == second);
    ASSERT_TRUE(other_option == NULL);

    sap_free(&parser);

}

TEST(SAP, ExecuteDefaultHandlerWithOptionsAndTrailingCommands) {

    int argc = 5;
    char* argv[5] = { 
        (char*) "./some-common-application", 
        (char*) "--key=some.value", 
        (char*) "--flag",
        (char*) "subcommand",
        (char*) "--suboption" 
    };

    sap_t parser;
    
    sap_init(&parser, argc, argv);

    sap_set_default(&parser, specific_handler);

    call_count = 0;
    return_value = 123;
    ASSERT_EQ(0, call_count);

    int ret = sap_execute(&parser);

    ASSERT_EQ(1, call_count);
    ASSERT_EQ(123, ret);

    sap_option_t* first = catched_options->first;
    sap_command_t* first_command = catched_commands->first;

    ASSERT_TRUE(first != NULL);
    ASSERT_STREQ(first->label, "key");
    ASSERT_STREQ(first->value, "some.value");
    ASSERT_TRUE(first_command != NULL);
    ASSERT_STREQ(first_command->label, "subcommand");
    ASSERT_EQ(0, first->is_flag);

    sap_option_t* second = first->next;
    sap_command_t* second_command = first_command->next;

    ASSERT_TRUE(second != NULL);
    ASSERT_TRUE(second_command == NULL);
    ASSERT_STREQ(second->label, "flag");
    ASSERT_EQ(1, second->is_flag);

    sap_free(&parser);

}
