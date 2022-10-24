#include "cmd_line_options.h"

void option_test();

int main(int argc, const char **argv)
{
    CmdLineOptions::GetInstance()->ParseOptions(argc,argv);
    option_test();
    return 0;
}
