#include "cmd_line_options.h"
#include <string>

void option_test();


// OptionGroup just inserts a help message, doesn't affect parsing.
OptionGroup option_help_message(
R"~(
example_as_string
  - this was used for creating a script file.
)~");

int main(int argc, const char **argv)
{
    std::string big_string;
    for (int i = 1; i < argc; i++)
    {
        big_string += argv[i];
        big_string += ' ';
    }
    CmdLineOptions::GetInstance()->ParseString(big_string.c_str());
    
    option_test();
    // if performing actions from a file, could reset the previous ones and parse them one at a time.
    CmdLineOptions::GetInstance()->Reset();

    return 0;
}
