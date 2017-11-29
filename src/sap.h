#ifndef __SAP_H__
#define __SAP_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

#ifdef __cplusplus
extern "C" {
#endif

struct sap_option_st;
struct sap_command_st;
struct sap_option_list_st;
struct sap_command_list_st;
struct sap_st;

typedef struct sap_option_st sap_option_t;
typedef struct sap_command_st sap_command_t;
typedef struct sap_option_list_st sap_option_list_t;
typedef struct sap_command_list_st sap_command_list_t;
typedef struct sap_st sap_t;

typedef int (*command_handler)(sap_command_list_t*, sap_option_list_t*);

struct sap_option_st {

    char* label;
    char* value;
    int is_flag;

    sap_option_t* next;

};

struct sap_command_st {

    char* label;
    command_handler handler;
    sap_command_t* next;

};


struct sap_option_list_st {

    sap_option_t* first;
    sap_option_t* last;
    unsigned int count;

};

struct sap_command_list_st {

  sap_command_t* first;
  sap_command_t* last;
  unsigned int count;

};

struct sap_st {

    sap_command_list_t commands;
    sap_command_t* default_command;

    sap_command_list_t* arg_commands;
    sap_option_list_t* arg_options;

    int argc;
    char** argv;

    char* error_string;

};


/* 
 * @brief Create a new Argument Parser Object.
 * @return Returns 0 on success 
 */
int sap_init(sap_t*, int argc, char** argv);

/*
 * @brief Add command to the parser by specifying a command string and
 *  a handler for the specific command.
 * @param parser
 * @param command Command String.
 * @param command_handler Command Handler.
 * @return 0 on success, -1 on failure.
 */
int sap_add_command(sap_t* parser, char* command, command_handler handler);

/*
 * @brief Set default handler that is called, when no handler fits the
 *  command string or no command string is given.
 * @param parser
 * @param command_handler Command Handler.
 */
void sap_set_default(sap_t* parser, command_handler handler);

/* 
 * @brief Execute parser with arguments.
 * @param parser The Simple Argument Parser Object.
 * @param argc Number of Arguments.
 * @param argv Argument Array.
 * @return Returns the return value from the handler and if none
 *  handler is found, returns -1. 
 */
int sap_execute(sap_t* parser);

/*
 * @brief Execute the parser with arguments and a predefined set of
 *  options. The Options delivered in the function call will be 
 *  merged with the options defined in the arguments. Options that
 *  reapear in the argument list will be overwritten by Options in 
 *  the options list.
 * @param parser The Simple Argument Parser Object.
 * @param argc Number of Arguments.
 * @param argv Argument Array.
 * @return Returns the return value from the handler and if none
 *  handler is found, returns -1.
 */
int sap_execute_ex(sap_t* parser, sap_command_list_t*, sap_option_list_t*);

/*
 * @brief Destroys the parser object.
 */
void sap_free(sap_t* parser);

/* @brief Get value as string (char*) from the options list. If
 *  not such option is available or the option is simply a flag
 *  return NULL.
 * @param options Options List.
 * @param key Key to look for in the Options List.
 * @return Return Value found in the list or NULL if not found.
 */
sap_option_t* sap_get_option_by_index(sap_option_list_t* options, unsigned int index);

/* @brief Get flag state (0, 1) as int from the options list. If
 *  not such option is available or the option disabled (0).
 * @param options Options List.
 * @param key Key to look for in the Options List.
 * @return Return 1 if flag is found in the list or 0 if not found.
 */

sap_option_t* sap_get_option_by_key(sap_option_list_t* options, char* key);

#ifdef __cplusplus
}
#endif
#endif
