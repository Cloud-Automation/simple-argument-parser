# Simple Argument Parser (SAP) [![Build Status](https://travis-ci.org/Cloud-Automation/simple-argument-parser.png)](https://travis-ci.org/Cloud-Automation/simple-argument-parser)

This library provides means and methods to parse command line arguments in the form of

```` command [OPTIONS...] subcommand_1 [OPTIONS...] subcommand_2 [OPTIONS...] ... ````

and is written in C.

## Installation

Clone the library.

```` git clone https://github.com/cloud-automation/simple-argument-parser.git ````

Make a `cd simple-argument-parser` and create a build directory `mkdir build & cd build`.

Execute `cmake ..` and `make` to build the library. 

Execute `sudo make install` to install the library into your system.

## Usage

````

  int command_handler(sap_command_list_t* commands, sap_option_list_t* options) {
  
      unsigned int counter = 0;
      sap_option_t* option = sap_get_option_by_index(options, counter);

      while (option) {
      
          printf("%s : %s\n", option.label, option.value);

          option = sap_get_option_by_index(options, ++counter);

      }
     
      return 0;
  
  }

  int main_handler(sap_command_list_t* commands, sap_option_list_t* options) {
  
    /* handles the first arguments in ./somecommand --optionA --optionB ... */

    sap_option_t* key_option = sap_get_option_by_key(options, "key");

    if (!key_option) {
        printf("No option 'key'.\n");
        return -1;
    }

    printf("Value of option 'key' = %s\n", key_option->value);

    return 0;    
      
  }

  int main(int argc, char* argv[]) {
 
    sap_t parser;

    sap_init(&parser, argc, argv[]);

    sap_set_default_command(&parser, default_handler);
    sap_set_command(&parser, "command", command_handler);

    int ret = sap_execute(&parser);

    sap_free(&parser);

    return ret;
      
  }


````

The parser itself only parses the first argument as a command. Since the file that should be executed is part of the argument list we create a parser and set a default handler. The default handler points to the `default_handler` method which will be called on execution when no command is given or recognized. The `main_handler` is now called with a list of commands and options all parsed from the original `argc` and `argv`.

When the `command` string turns up first in the `argc` datastructure then the command handler method is called. The command list given to the command handler method will be one short, missing the first command.


## License

Copyright (C) 2017 Stefan Poeter (Stefan.Poeter[at]cloud-automation.de)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
