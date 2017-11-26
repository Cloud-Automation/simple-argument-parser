#include "sap.h"

int sap_init(sap_t* parser, int argc, char** argv) {

    parser->argc = argc;
    parser->argv = argv;

    parser->commands.first = NULL;
    parser->commands.last = NULL;

    parser->default_command = NULL;

    parser->error_string = calloc(1, 100);

    return 0;

};

int sap_add_command(sap_t* parser, char* command, command_handler handler) {

    if (parser == NULL || command == NULL) {
        return -1;
    }

    /* go through the command list and look for existing
     * commands, return -1 if so. */



    sap_command_t* current_command = parser->commands.first;


    while (current_command) {

        if (strcmp(current_command->label, command) == 0) {
            return -1;
        }
   
        current_command = current_command->next;

    }

    /* Create new command */

    sap_command_t* new_command = calloc(1, sizeof(sap_command_t));

    *new_command = (sap_command_t) {
        .label = command,
        .handler = handler,
        .next = NULL
    };

    if (parser->commands.first == NULL) {
      parser->commands.first = new_command;
      parser->commands.last = parser->commands.first;
      return 0;
    }

    sap_command_t* last_command = parser->commands.last;

    last_command->next = new_command;
    parser->commands.last = new_command;

    return 0;

}

void sap_set_default(sap_t* parser, command_handler handler) {

    if (parser == NULL || handler == NULL) {
    
        return;
    
    }

    sap_command_t* command = calloc(1, sizeof(sap_t));
    command->label = "";
    command->handler = handler;

    parser->default_command = command;

}

#define option_regex "^--\\(\\w\\+\\)\\(=\\([^[:space:]]\\+\\)\\)\\?$"

static int sap_is_option(sap_t* parser, char* arg) {

    regex_t regex;
    int reti = regcomp(&regex, option_regex, 0);

    if (reti) {
      // TODO: make this a error String
        sprintf(parser->error_string, "Could not compile regex.\n");
        exit(-1);
    }

    /* execute regular expession */

    reti = regexec(&regex, arg, 0, NULL, 0);

    regfree(&regex);

    if (!reti) {
        return 1;
    } else if (reti == REG_NOMATCH) {
        sprintf(parser->error_string, "Not an option (%s).\n", arg);
        return 0;
    } 

        
    return 0;

}

static sap_option_t* sap_parse_option(char* arg) {

    int status;
    regmatch_t match[4];

    /* set the first and third match to -1 so we can identify those late
     * if they match than there will be some plausable values */

    match[1].rm_so = -1;
    match[1].rm_eo = -1;

    match[3].rm_so = -1;
    match[3].rm_eo = -1;

    /* execute the regular expression, we assume that
     * the string has been checked before */

    regex_t regex;
    regcomp(&regex, option_regex, 0);
    regexec(&regex, arg, 4, match, 0);

    regfree(&regex);

    /* if there is no match on the first argument then
     * there is somehow an error and we return NULL */

    if (match[1].rm_so == -1) {
        return NULL;
    }

    /* if we found something then we create a new option
     * and store this string as the options label */

    int len = match[1].rm_eo - match[1].rm_so;
    char* label = calloc(1, len);
    memcpy(label, arg + match[1].rm_so, len);

    sap_option_t* option = calloc(1, sizeof(sap_option_t));
    option->label = label;
    option->value = NULL;
    option->is_flag = 1;

    /* if there is no value assigned to the option then we mark
     * this option as flag. Meaning this option is enabled.
     * if there is a value, we mark this no flag and store the
     * value in the options value attribute */

    if (match[3].rm_so == -1) {
        return option;
    }

    len = match[3].rm_eo - match[3].rm_so;
    char* value = calloc(1, len);
    memcpy(value, arg + match[3].rm_so, len);

    option->value = value;
    option->is_flag = 0;

    return option;

}


#define command_regex "^[^--][[:alnum:]\\.\\/-]\\+$"

static int sap_is_command(sap_t* parser, char* arg) {

    regex_t regex;
    int reti = regcomp(&regex, command_regex, 0);

    if (reti) {
        sprintf(parser->error_string, "Could not compile regex.\n");
        exit(-1);
    }

    /* execute regular expession */

    reti = regexec(&regex, arg, 0, NULL, 0);

    regfree(&regex);

    if (!reti) {
        return 1;
    } else if (reti == REG_NOMATCH) {
        return 0;
    } 
        
    return 0;

}

int sap_execute(sap_t* parser) {

    sap_command_list_t* commands = calloc(1, sizeof(sap_command_list_t));
    sap_option_list_t* options = calloc(1, sizeof(sap_option_list_t));

    sap_command_t* command = parser->default_command;

    for (unsigned int i = 1; i < parser->argc; i += 1) {

      char* command_str = parser->argv[i];

      if (sap_is_command(parser, command_str)) {

        sap_command_t* new_command = calloc(1, sizeof(sap_command_t));
        new_command->label = calloc(1, strlen(command_str) + 1);
        strcpy(new_command->label, command_str);

        if (!commands->first) {

          commands->first = new_command; 
          commands->last = new_command;

        } else {
        
          commands->last->next = new_command;
          commands->last = new_command;

        }

      }
    }

    for (unsigned int i = 1; i < parser->argc; i += 1) {

        char* current_string = parser->argv[i];

        /* check if current_string is option */

        if (sap_is_option(parser, current_string)) {

            sap_option_t* option = sap_parse_option(current_string);    

            if (!options->first) {

              options->first = option;
              options->last = option;

              option->next = NULL;

            } else {
            
              options->last->next = option;
              options->last = option;

              option->next = NULL;

            }

        }
           
    }

    /* get command from the parsers commands list */

    sap_command_t* first_command = commands->first;
    sap_command_t* current_command = parser->commands.first;

    while (current_command) {

        if (!current_command) {
          break;
        }
    
        if (strcmp(first_command->label, current_command->label) == 0) {
        
          break;

        }

        current_command = current_command->next;
    
    }


    if (current_command) {

      commands->first = commands->first->next;

      int retVal = current_command->handler(commands, options);

      free(commands->first);

      return retVal;

    } else if (parser->default_command) {

      return parser->default_command->handler(commands, options);

    } else {
    
      return -1;
    
    }

}

int sap_execute_ex(sap_t* parser, sap_command_list_t* commands, sap_option_list_t* options) {

  /* get command from the parsers commands list */

    sap_command_t* first_command = commands->first;
    sap_command_t* current_command = parser->commands.first;

    while (current_command) {

        if (!current_command || !first_command) {
          break;
        }
    
        if (strcmp(first_command->label, current_command->label) == 0) {
        
          break;

        }

        current_command = current_command->next;
    
    }


    if (current_command && first_command) {

      commands->first = commands->first->next;

      int retVal = current_command->handler(commands, options);

      free(commands->first);

      return retVal;

    } else if (parser->default_command) {

      return parser->default_command->handler(commands, options);

    } else {
    
      return -1;
    
    }

}

void sap_free(sap_t* parser) {

    if (parser == NULL) {
        return;
    }

    free(parser->error_string);

    /* remove default command */
    free(parser->default_command);

    /* go through command list and
     * remove every command */

    sap_command_t* command = parser->commands.first;

    while (command) {
      sap_command_t* tmp = command;
      command = tmp->next;
      free(tmp);
    }

}

sap_option_t* sap_get_option_by_index(sap_option_list_t* options, unsigned int index) {

    if (options == NULL) {
        return NULL;
    }

    unsigned int counter = 0;

    sap_option_t* curOption = options->first;


    while (curOption) {

      if (counter == index) {
        return curOption;
      }

      curOption = curOption->next;
      counter += 1;

    }

    return NULL;

}

sap_option_t* sap_get_option_by_key(sap_option_list_t* options, char* key) {

    if (options == NULL || key == NULL) {
        return NULL;
    }

    sap_option_t* curOption = options->first;


    while (curOption) {
    
        if (strcmp(curOption->label, key) == 0) {
            return curOption;
        }

        curOption =curOption->next;

    }

    return NULL;

}
