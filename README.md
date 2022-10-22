# CmdLineOptions
yet another command line parser for c++ 

## What is it good for

Command line parsers are annoying.

Typically in your top level source file right beside main() you define a parse_options() function and a usage() message, and if there are any wierd flags in different modules, you have to include all these files and take actions when those flags are set.

e.g.

```c
void parse_options(int argc, char **argv)
{
   if (strncmp(argv[i],"log_level",9) == 0)
   {
      // extract and set logging level
   } else if (strncmp(argv[i],"parse_debug",11) == 0)
   {
      // extract and set parse debug flag.
   }
}
```

This command line parser doesn't do that.

Instead whenever you want an options to be available,... in any module you define the option like:

Maybe in `log.c` you have a global variable like:
```c++
IntOption option_log_level( "log_level", "set the log_level from 0..9" );
```

Maybe in `parse.c` you have a flag for extra debugging:
```c++
BoolOption option_parse_debug( "parse_debug", "enable lots of debugging for the parser" );
```

In `main.c` you just call:
```c++
CmdLineOptions::ParseOptions( argc, argv );
```
And this 'magically' updates all those variables in those other `.c` files that it knew nothing about, and generates a help message if an invalid option is specified.

The 'magic' isn't really magic, each option is a static object with a static initializer that adds the option to a global list.

This all happens before `main()` is called.

oh,... and environment variables are also checked for that match the option name.

e.g. export PROJECT_log_level=1

Is the same as adding log_level=1 on the command line.

# todo: add some example code