# CmdLineOptions
yet another command line parser for c++ 

## What is it good for

Command line parsers are annoying.

Typically in your top level source file right beside main() you define a parse_options() function and a usage() message, and if there are any wierd flags in different modules, you have to include all these files and take actions when those flags are set.

e.g.

```c
void parse_options(int argc, char **argv)
{
   if (strcmp(argv[i],"-v")
  {
      // set some verbosity flag
  }
}
```

This command line parser doesn't do that.

Instead whenever you want an options to be available,... in any module you define the option like:

Maybe in `log.c` you have a global variable like:
```c++
IntOption option_verbosity( "verbosity", "set the verbosity from 0..9" );
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
