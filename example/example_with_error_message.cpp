#include "cmd_line_options.h"
#include <sstream>
#include <iostream>

void option_test();

// OptionGroup just inserts a help message, doesn't affect parsing.
OptionGroup option_help_message(
R"~(
example_with_error_message
  - demonstrates how options can be passed as messages and parsed
  - does not assert on failure,... instead outputs error message to a std::ostream
)~");

int main(int argc, const char **argv)
{
    std::stringstream out;
    if (CmdLineOptions::GetInstance()->ParseOptionsOrError(argc-1,&argv[1],out))
    {
        option_test();
        return 0;
    }
    else
    {
        printf("ParseOptionsOrError returned false\n");
        std::cout << out.str();
        return 255;
    }
}
