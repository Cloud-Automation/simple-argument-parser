
#include "sap.h"

static int test_handler(sap_command_list_t* command, sap_option_list_t* option)
{

	printf("Executing the test handler\n");
	return 0;

}

static int default_handler(sap_command_list_t* commands, sap_option_list_t* options)
{

	printf("Executing the default handler.\n");

	sap_command_t* cc = commands->first;
	while (cc)
	{
		printf("Command %s\n", cc->label);
		cc = cc->next;
	}

	unsigned int curIndex = 0;
	sap_option_t* co = sap_get_option_by_index(options, curIndex);
	while (co)
	{
		if (co->is_flag)
		{
			printf("flag %s\n", co->label);
		}
		else
		{
			printf("option %s with value %s.\n", co->label, co->value);
		}

		co = sap_get_option_by_index(options, ++curIndex);

	}

}

int main(int argc, char** argv)
{

	sap_t parser;

	sap_init(&parser, argc, argv);

	sap_set_default(&parser, default_handler);

	sap_add_command(&parser, "test", test_handler);

	int retVal =  sap_execute(&parser);

	sap_free(&parser);

	return retVal;

}
