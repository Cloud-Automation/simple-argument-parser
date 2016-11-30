#ifndef __SAP_H__
#define __SAP_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

#define SAP_MAX 10
#define SAP_MAX_OPTIONS 50
#define SAP_MAX_COMMANDS 50

typedef struct sap_option_st {

    char* label;
    char* value;
    int is_flag;

} sap_option_t;

typedef struct sap_options_st {

    sap_option_t* list[SAP_MAX_OPTIONS];

} sap_options_t;

typedef int (*command_handler)(int argc, char* argv[], sap_options_t* options);

typedef struct sap_command_st {

    char* label;
    command_handler handler;

} sap_command_t;

typedef struct sap_st {

    sap_command_t* commands[SAP_MAX_COMMANDS];
    sap_command_t* default_command;

} sap_t;



sap_t* sap_create();
int sap_add_command(sap_t* parser, char* command, command_handler handler);
int sap_set_default(sap_t* parser, command_handler handler);
int sap_execute(sap_t* parser, int argc, char* argv[]);
int sap_execute_ex(sap_t* parser, int argc, char* argv[], sap_options_t* options);
void sap_destroy(sap_t* parser);

#endif
