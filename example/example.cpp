#include "cmd_line_options.h"

void option_test();

// OptionGroup just inserts a help message, doesn't affect parsing.
OptionGroup option_help_message(
R"~(
example
  - normal parsing, asserts if a bad option is found
)~");

int main(int argc, const char **argv)
{
    CmdLineOptions::GetInstance()->ParseOptions(argc,argv);
    option_test();
    return 0;
}
