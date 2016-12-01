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

  int main_handler(int argc, char* argv[], sap_options_t* options) {
  
    /* handles the first arguments in ./somecommand --optionA --optionB ... */

    return 0;    
      
  }

  int main(int argc, char* argv[]) {
  
    sap_t* parser = sap_create();

    sap_set_default_command(parser, main_handler);

    int ret = sap_execute(parser);

    sap_destroy(parser);

    return ret;
      
  }


````

The parser itself only parses the first argument as a command. Since the file that should be executed is part of the argument list we create a parser and set a default handler. The default handler points to the `main_handler`` method which will be calld on execution. The 'main_handler` is now called with the `argc` minus the options following the program in the argument list. The `argv` array also starts at the next command.

From the `main_handler` we create a new parser to handle subcommands.

````

  int suba_handler(int argc, char* argv[], sap_options_t* options) {...}
  int subb_handler(int argc, char* argv[], sap_options_t* options) {...}
  int help_handler(int argc, char* argv[], sap_options_t* options) {...}

  int main_handler(int argc, char* argv[], sap_options_t* options) {
  
    sap_t* main_parser = sap_create();
    
    sap_add_command(main_parser, "subcommand_A", suba_handler);
    sap_add_command(main_parser, "subcommand_B", subb_handler);
    sap_add_command(main_parser, "help", help_handler);

    sap_set_default_command(main_parser, help_handler);
     
    int ret = sap_execute(main_parser);

    sap_destroy(parser);

    return ret;
      
  }

  int main(int argc, char* argv[]) { ... }

````

Here we see behaviour for commands like `./command [OPTIONS...] subcommand [OPTIONS...]` where subcommand can be anything like subcommand_A, subcommand_B or help. If a unknown or no subcommand is specified then the default command will be called. In this case it is the `help_handler`.

## License

Copyright (C) 2016 Stefan Poeter (Stefan.Poeter[at]cloud-automation.de)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
