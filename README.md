# CmdLineOptions
Yet another command line parser for c++.

This command line parser lets you scatter options throughout your code base, and the top level main() function doesn't need to know anything about them.

Just like how googletest doesn't need to know about all the GTEST() macros to run those tests, this command line parser doesn't need to know about all the command line options exposed in your code.

## build status

![build/test status](https://github.com/byronwatt/CmdLineOptions/actions/workflows/c-cpp.yml/badge.svg)

## Notes from the author 

At Microchip, we have hundreds of standalone test programs for different features of our products, and we have a lot of common options that can be used to help with investigating and auditing execution.  

Often when you are debugging a problem you tweak some variables and recompile to turn on a certain feature.  But in many cases, it is possible to achieve the same effect with a runtime call or global variable, and exposing that variable through a command line option leads to all sorts of convenient features.  Having all these easily accessible run-time knobs is very convenient as a developer, but also lets you document various workflows in a straight-forward manner.

For instance, sometimes we want to generate an audit trail of all the actions performed. Sometimes we want to add a delay after each action, or sometimes we want to test with a different version of the firmware, or we want to specify a host/port for a simulator, or which board number to test or change a timeout.

All of these 'nice to have' options would make writing a parse_options tool annoying, unless the parse_options function could somehow discover all the options that are 'inside' the libraries being used.

So that's what this package provides: In any source file, you can define an option.

## Overview 

Maybe in `log.cpp` you have a global variable like:
```c++
static IntOption option_log_level( "log_level", 0, "set the log_level from 0..9" );
```

Maybe in `parse.cpp` you have a flag for extra debugging:
```c++
static BoolOption option_parse_debug( "parse_debug", false, "enable lots of debugging for the parser" );
```

In `main.cpp` you just call:
```c++
CmdLineOptions::InstanceGet()->ParseOptions( argc, argv );
```
And this 'magically' updates all those variables in those other `.cpp` files that it knew nothing about, and generates a help message if an invalid option is specified.

The 'magic' isn't really magic, each option is a static object with a static initializer that updates a global list.

This all happens before `main()` is called.

## Todo: add some example code and some test scripts

## Confessions from the author

### Positional arguments and list arguments

The parser doesn't handle positional arguments, we just didn't need it to handle positional arguments for our purposes.

It does have a 'list' argument, like a list of integers or a list of strings.  Both of those go into vector<int> or vector<const char *>

e.g. If somewhere in your code you have:

```c++
static IntListOption option_channels("channels:", "list of channels to use");
```
Then on the command line you can type:
```bash
program channels: 0 1 2 3
```

An integer list terminates if it finds an argument that isn't a valid integer.

A string list terminates if it finds an argument that doesn't match a valid command line option.

### '-' or '--'

I'm really lazy,... so I didn't bother requiring that you put a '-' in front of an argument.

e.g.  `log_level=1` is allowed as is `-log_level=1` as is `--log_level=1` as is any number of minus signs.

### short options

There's no feature to combine a bunch of boolean options with a bunch of single letter characters.  For instance as is done with 'ls -ltr' or 'ps auxf'.

There is an 'alias' feature.  e.g. you could say the option 'everything' is the same as 'verbosity=9 parse_debug=true audit_trail=audit.txt' 

But there's no way to have single character options,... maybe that's just my personal preference, I'm too old to remember single character flags.

### Enumerations

enumerations are a little clunky, you have to extend the EnumOption class and provide a list of string -> value pairs in the constructor.

But having enumerated options is really nice for the user,... maybe someone can come up with a better technique?

```c++
typedef enum {
    TEST_ALL,
    TEST_SMOKE_TEST,
    TEST_ENDURANCE_TEST,
} TestSelection;

class TestSelectionOption: public EnumOption {
public:
    TestSelectionOption( uint32_t default_value, const char *_name, const char *_usage_message ) : EnumOption(default_value,_name,_usage_message) {
        AddEnum(0,"all","run all tests");
        AddEnum(1,"smoke_test","just a quick smoke test");
        AddEnum(2,"endurance_test","longer test");
        SetFromEnvironmentVariable();
    }
} ;
TestSelectionOption option_test_selection("test",0,"test to run" );
```

Then you can use a command line option like:
```
test=smoke_test
```
If you try to set it to an invalid enumeration it displays a help message with the valid enumerations and their help message.


### Environment variables

Environment variables are also checked that match the option name, which is sometimes convenient if the test you are running is inside a script that you don't want to modify.

e.g. export PROJECT_log_level=1

Is the same as adding log_level=1 on the command line.
    
Note: the project name prefix is hard-coded inside `CmdLineOption::SetFromEnvironmentVariable()`.

### ParseString

At Microchip, we often have a utility program that we repeatedly execute with different arguments to do little things.  As an optimization to reduce startup time, we allow that program to be called with a script file as input, so we use the ParseString() and Reset() functions to pretend the program was called again with different command line arguments.

### ParseOptionsOrError

At Microchip, we found it is nice to allow changing options on the fly, e.g. one program can send a message to another program messages to adjust it's runtime flags,...  ParseOptionsOrError() can return an error message to the caller rather than exit'ing with the error message displayed to stderr. 

### xterm window title

At Microchip, the projects that use this command line parser often use multiple windows for running the simulator and firmware and host code, so to help with keeping thing sorted, we modify the xterm window title inside ParseOptions to include the program name with:

```c++
printf("\033]0;%s\007",window_title.c_str());
```

but you probably do not want that, so I'll comment that out !

### calling functions instead of setting variables

If you need to call a function in addition to setting a variable, then you can create a derived class and override the virtual function `OptionSet()` and modify it to suit.

The `LogLevelOption` class shows an example of overriding EnumOption and defining a `LogLevelOption::OptionSet()` method for whenever the log_level option is changed.

This might not help much since typically you would parse command line options before you've initialized various libraries.  But occasionally it's useful.
