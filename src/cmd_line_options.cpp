//  COPYRIGHT (C) Microchip with MIT license

/**
 * @file
 * @brief
 * Source file of command line option parsing.
 */

#include <string.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "cmd_line_options.h"


/**
 * @brief
 *   constructor
 *
 * @param[in] _name - option name
 * @param[in] _usage_message - option usage message
 */
CmdLineOption::CmdLineOption(const char *_name, const char *_usage_message) :
    name(_name),
    usage_message(_usage_message),
    is_set(),
    is_list(),
    is_option_free_list(),
    is_bool()
{
    // add this option to a global list of options.
    CmdLineOptions::GetInstance()->AddOption(this);
}


/**
 * @brief
 *   Parse a command line option
 *
 * @param[in] s - command line argument string
 * @param[in] error_message - error message
 *
 * @return bool8_t - true if argument string is valid
 */
bool8_t CmdLineOption::ParseValueWithError( const char *s, std::ostream &error_message ) {
    if (ParseValue(s))
        return TRUE;
    error_message << "error parsing '" << s << "'\n";
    error_message << " for option '" << name << "'\n";
    error_message << " option description: " << usage_message << "\n";
    return FALSE;
}

/**
 * @brief
 *   Return the singleton instance
 * @return
 *   CmdLineOptions * - the singleton instance.
 */
CmdLineOptions* CmdLineOptions::GetInstance()
{
    static CmdLineOptions instance;
    return &instance;
}

/**
 * @brief
 *   default end of list method
 */
void CmdLineOption::EndOfList()
{
}

/**
 * @brief
 *   reset value to default
 */
void CmdLineOption::Reset()
{
    is_set = false;
}

/**
 * @brief
 *   called whenever an option is set
 *   you can override this method with custom code per option.
 */
void CmdLineOption::OptionSet()
{
}

/**
 * @brief
 *   constructor
 *
 * @param[in] default_value - default value if not specified on command line
 * @param[in] _name - option name
 * @param[in] _usage_message - option usage message
 */
BoolOption::BoolOption( bool8_t default_value, const char *_name, const char *_usage_message ) : CmdLineOption(_name, _usage_message), value(default_value), _default_value(default_value) {
    SetFromEnvironmentVariable();
}



/**
 * @brief
 *   reset value to default
 */
void BoolOption::Reset()
{
    is_set = false;
    value = _default_value;
}

/**
 * @brief
 *   searches for 'search' and replaces it with 'replace' in the string 'subject'
 *
 * @param[in] subject - source string
 * @param[in] search - search substring
 * @param[in] replace - replace substring
 *
 * @return std::string - resulting string
 */
static std::string ReplaceString(std::string subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

/**
 * @brief
 *   constructor
 *
 * @param[in] _name - option name
 * @param[in] _usage_message - option usage message
 */
AliasOption::AliasOption( const char *_name, const char *_usage_message ) : CmdLineOption(_name, _usage_message) {
    SetFromEnvironmentVariable();
}

/**
 * @brief
 *   Parse a command line option
 *
 * @param[in] s - command line argument string
 *
 * @return bool8_t - true if argument string is valid
 */
bool8_t AliasOption::ParseValue( const char *s ) {
    char *token;
    char *argv_str = strdup(usage_message);

    printf("%s %s is an alias for: '%s'\n",name,s,argv_str);
    /* get the first token */
    token = strtok(argv_str, " ");

    alias_argv.push_back((char *)name);

    /* walk through other tokens */
    while( token != NULL )
    {
        char *arg = strdup(token);
        alias_argv.push_back((char *)arg); /* note: this is not used,... just saved to avoid valgrind from detecting a memory leak */

        token = strtok(NULL, " ");
    }
    free(argv_str);

    /* parse options with alias'd argc/argv */
    CmdLineOptions::GetInstance()->ParseOptions(alias_argv.size(),&(alias_argv[0]));

    return TRUE;
}

/**
 * @brief
 *   constructor
 *
 * @param[in] _name - option name
 * @param[in] _usage_message - option usage message
 */
AliasListOption::AliasListOption( const char *_name, const char *_usage_message ) : IntListOption(_name, _usage_message) {
    SetFromEnvironmentVariable();
}

/**
 * @brief
 *   called when all list items have been processed
 */
void AliasListOption::EndOfList() {
    /*char *token;*/
    std::string with_replacements = usage_message;
    std::string full_string;
    uint32_t i = 0;

    /* replace %0 %1 %2 with corresponding list element */
    for (std::vector<const char *>::const_iterator it = string_list_.begin(); it != string_list_.end(); ++it,i++) {
        if (i != 0) {
            full_string += " ";
        }
        full_string = full_string + *(it);
        char search_string[10];
        sprintf(search_string,"%%%d",i);
        with_replacements = ReplaceString(with_replacements,search_string,*(it));
    }
    /* replace %* with full list of arguments */
    with_replacements = ReplaceString(with_replacements,"%*",full_string);

    printf("%s %s is an alias for: '%s'\n",name,full_string.c_str(),with_replacements.c_str());

    CmdLineOptions::GetInstance()->ParseString(with_replacements.c_str());
}

/**
 * @brief
 *   constructor
 *
 * @param[in] default_value - default value if not specified on command line
 * @param[in] _name - option name
 * @param[in] _usage_message - option usage message
 */
IntOption::IntOption(int32_t default_value, const char *_name, const char *_usage_message ) : CmdLineOption(_name, _usage_message), value(default_value),_default_value(default_value) {
    SetFromEnvironmentVariable();
}

/**
 * @brief
 *   reset value to default
 */
void IntOption::Reset()
{
    is_set = false;
    value = _default_value;
}

/**
 * @brief
 *   Parse an integer from a string handles leading '0x'
 *
 * @param[in] s - command line argument string
 * @param[in] temp - pointer to first non-integer character in the string
 *
 * @return int32_t - integer value
 */
int32_t parse_int( const char *s, char **temp = NULL )
{
    int32_t value;
    if (strncmp(s,"0x",2) == 0) {
      value = strtol(s+2,temp,16);
    } else {
      value = strtol(s,temp,10);
    }
    return value;
}

/**
 * @brief
 *   Parse a command line option
 *
 * @param[in] s - command line argument string
 *
 * @return bool8_t - true if argument string is valid
 */
bool8_t IntOption::ParseValue( const char *s ) {
    char *temp;
    value = parse_int(s,&temp);
    if (*temp != 0) return FALSE;
    /* printf("%s=%d (%x)\n",name,option_field,option_field); */
    return TRUE;
}

/**
 * @brief
 *   Parse a command line option
 *
 * @param[in] s - command line argument string
 * @param[in] error_message - error message
 *
 * @return bool8_t - true if argument string is valid
 */
bool8_t IntOption::ParseValueWithError( const char *s, std::ostream &error_message ) {
    if (ParseValue(s))
        return TRUE;
    error_message << "error parsing '" << s << "'\n";
    error_message << " for int option '" << name << "'\n";
    error_message << " option description: " << usage_message << "\n";
    return FALSE;
}

/**
 * @brief
 *   constructor
 *
 * @param[in] default_value - default value if not specified on command line
 * @param[in] _name - option name
 * @param[in] _usage_message - option usage message
 */
UintOption::UintOption(uint32_t default_value, const char *_name, const char *_usage_message ) : CmdLineOption(_name, _usage_message), value(default_value),_default_value(default_value) {
    SetFromEnvironmentVariable();
}

/**
 * @brief
 *   reset value to default
 */
void UintOption::Reset()
{
    is_set = false;
    value = _default_value;
}

/**
 * @brief
 *   Parse an integer from a string handles leading '0x'
 *
 * @param[in] s - command line argument string
 * @param[in] temp - pointer to first non-integer character in the string
 *
 * @return int32_t - integer value
 */
uint32_t parse_uint( const char *s, char **temp = NULL )
{
    uint32_t value;
    if (strncmp(s,"0x",2) == 0) {
      value = strtoul(s+2,temp,16);
    } else {
      value = strtoul(s,temp,10);
    }
    return value;
}

/**
 * @brief
 *   parse the command line option
 *
 * @param[in] s - default value if not specified on command line
 *
 * @return bool8_t - TRUE if option was valid.
 */
bool8_t UintOption::ParseValue( const char *s ) {
    char *temp;
    value = parse_uint(s,&temp);
    if (*temp != 0) return FALSE;
    /* printf("%s=%d (%x)\n",name,option_field,option_field); */
    return TRUE;
}

/**
 * @brief
 *   Parse a command line option
 *
 * @param[in] s - command line argument string
 * @param[in] error_message - error message
 *
 * @return bool8_t - true if argument string is valid
 */
bool8_t UintOption::ParseValueWithError( const char *s, std::ostream &error_message ) {
    if (ParseValue(s))
        return TRUE;
    error_message << "error parsing '" << s << "'\n";
    error_message << " for uint option '" << name << "'\n";
    error_message << " option description: " << usage_message << "\n";
    return FALSE;
}

/**
 * @brief
 *   constructor
 *
 * @param[in] default_value - default value if not specified on command line
 * @param[in] _name - option name
 * @param[in] _usage_message - option usage message
 */
Int64Option::Int64Option(int64_t default_value, const char *_name, const char *_usage_message ) : CmdLineOption(_name, _usage_message), value(default_value),_default_value(default_value) {
    SetFromEnvironmentVariable();
}

/**
 * @brief
 *   reset value to default
 */
void Int64Option::Reset()
{
    is_set = false;
    value = _default_value;
}

/**
 * @brief
 *   Parse an integer from a string handles leading '0x'
 *
 * @param[in] s - command line argument string
 * @param[in] temp - pointer to first non-integer character in the string
 *
 * @return int32_t - integer value
 */
static int64_t parse_int64( const char *s, char **temp = NULL )
{
    int64_t value;
    if (strncmp(s,"0x",2) == 0) {
      value = strtoll(s+2,temp,16);
    } else {
      value = strtoll(s,temp,10);
    }
    return value;
}

/**
 * @brief
 *   parse the command line option
 *
 * @param[in] s - default value if not specified on command line
 *
 * @return bool8_t - TRUE if option was valid.
 */
bool8_t Int64Option::ParseValue( const char *s ) {
    char *temp;
    value = parse_int64(s,&temp);
    if (*temp != 0) return FALSE;
    /* printf("%s=%d (%x)\n",name,option_field,option_field); */
    return TRUE;
}

/**
 * @brief
 *   Parse a command line option
 *
 * @param[in] s - command line argument string
 * @param[in] error_message - error message
 *
 * @return bool8_t - true if argument string is valid
 */
bool8_t Int64Option::ParseValueWithError( const char *s, std::ostream &error_message ) {
    if (ParseValue(s))
        return TRUE;
    error_message << "error parsing '" << s << "'\n";
    error_message << " for int64 option '" << name << "'\n";
    error_message << " option description: " << usage_message << "\n";
    return FALSE;
}

/**
 * @brief
 *   constructor
 *
 * @param[in] default_value - default value if not specified on command line
 * @param[in] _name - option name
 * @param[in] _usage_message - option usage message
 */
Uint64Option::Uint64Option(uint64_t default_value, const char *_name, const char *_usage_message ) : CmdLineOption(_name, _usage_message), value(default_value),_default_value(default_value) {
    SetFromEnvironmentVariable();
}

/**
 * @brief
 *   reset value to default
 */
void Uint64Option::Reset()
{
    is_set = false;
    value = _default_value;
}

/**
 * @brief
 *   Parse an integer from a string handles leading '0x'
 *
 * @param[in] s - command line argument string
 * @param[in] temp - pointer to first non-integer character in the string
 *
 * @return int32_t - integer value
 */
static uint64_t parse_uint64( const char *s, char **temp = NULL )
{
    uint64_t value;
    if (strncmp(s,"0x",2) == 0) {
      value = strtoull(s+2,temp,16);
    } else {
      value = strtoull(s,temp,10);
    }
    return value;
}

/**
 * @brief
 *   parse the command line option
 *
 * @param[in] s - default value if not specified on command line
 *
 * @return bool8_t - TRUE if option was valid.
 */
bool8_t Uint64Option::ParseValue( const char *s ) {
    char *temp;
    value = parse_uint64(s,&temp);
    if (*temp != 0) return FALSE;
    /* printf("%s=%d (%x)\n",name,option_field,option_field); */
    return TRUE;
}

/**
 * @brief
 *   Parse a command line option
 *
 * @param[in] s - command line argument string
 * @param[in] error_message - error message
 *
 * @return bool8_t - true if argument string is valid
 */
bool8_t Uint64Option::ParseValueWithError( const char *s, std::ostream &error_message ) {
    if (ParseValue(s))
        return TRUE;
    error_message << "error parsing '" << s << "'\n";
    error_message << " for uint64 option '" << name << "'\n";
    error_message << " option description: " << usage_message << "\n";
    return FALSE;
}

/**
 * @brief
 *   case insensitive string compare
 *
 * @param[in] s1 - string 1
 * @param[in] s2 - string 2
 *
 * @return int - same as strcmp
 */
static int
my_stricmp (const char *s1, const char *s2)
{
   if (s1 == NULL) return s2 == NULL ? 0 : -(*s2);
   if (s2 == NULL) return *s1;

   char c1, c2;
   while ((c1 = tolower (*s1)) == (c2 = tolower (*s2)))
   {
     if (*s1 == '\0') break;
     ++s1; ++s2;
   }

   return c1 - c2;
}

/**
 * @brief
 *   parse the command line option
 *
 * @param[in] s - default value if not specified on command line
 *
 * @return bool8_t - TRUE if option was valid.
 */
bool8_t BoolOption::ParseValue( const char *s ) {
    if ((my_stricmp(s,"") == 0) ||
        (my_stricmp(s,"1") == 0) ||
        (my_stricmp(s,"on") == 0) ||
        (my_stricmp(s,"yes") == 0) ||
        (my_stricmp(s,"true") == 0)) {
        value = TRUE;
        return TRUE;
    }
    if ((my_stricmp(s,"0") == 0) ||
        (my_stricmp(s,"no") == 0) ||
        (my_stricmp(s,"off") == 0) ||
        (my_stricmp(s,"false") == 0)) {
        value = FALSE;
        return TRUE;
    }
    return FALSE;
}

/**
 * @brief
 *   Parse a command line option
 *
 * @param[in] s - command line argument string
 * @param[in] error_message - error message
 *
 * @return bool8_t - true if argument string is valid
 */
bool8_t BoolOption::ParseValueWithError( const char *s, std::ostream &error_message ) {
    if (ParseValue(s))
        return TRUE;
    error_message << "error parsing '" << s << "'\n";
    error_message << " for bool option '" << name << "'\n";
    error_message << " option description: " << usage_message << "\n";
    error_message << "valid values are 0,1,no,yes,off,on,false,true\n";
    return FALSE;
}

/**
 * @brief
 *   constructor
 *
 * @param[in] default_value - default value if not specified on command line
 * @param[in] _name - option name
 * @param[in] _usage_message - option usage message
 */
EnumOption::EnumOption( uint32_t default_value, const char *_name, const char *_usage_message ) : CmdLineOption(_name, _usage_message), value(default_value),_default_value(default_value) {
}

/**
 * @brief
 *   reset value to default
 */
void EnumOption::Reset()
{
    is_set = false;
    value = _default_value;
}

/**
 * @brief
 *   add an enumeration
 *
 * @param[in] _value - enum value
 * @param[in] _str - enum string
 * @param[in] _usage_message - usage message
 */
void EnumOption::AddEnum(uint32_t _value, const char *_str, const char *_usage_message)
{
    value_str_t value_string;
    value_string.value = _value;
    value_string.str = _str;
    value_string.usage_message = _usage_message;
    enum_list_.push_back(value_string);
}

/**
 * @brief
 *   find an enumeration
 *
 * @param[in] enum_list - vector of enumerations
 * @param[in] s - string to look for
 * @param[out] value - value the corresponds to the specified string
 *
 * @return bool8_t - TRUE if a match was found
 */
static bool8_t find_enum(std::vector<value_str_t> *enum_list, const char *s, uint32_t *value ) {
    for (std::vector<value_str_t>::const_iterator it = enum_list->begin(); it != enum_list->end(); ++it) {
        value_str_t value_string = *(it);
        if (my_stricmp(value_string.str,s)==0) {
            *value = value_string.value;
            return TRUE;
        }
    }
    return FALSE;
}


/**
 * @brief
 *   parse the enumeration
 *
 * @param[in] s - enumeration to parse
 *
 * @return bool8_t - TRUE if enumeration was valid.
 */
bool8_t EnumOption::ParseValue( const char *s ) {
    if (find_enum(&enum_list_,s,&value)) {
        return TRUE;
    }
    char *temp;
    value = parse_int(s,&temp);
    if (*temp == 0) return TRUE;
    printf("unknown %s \"%s\"\n",name,s);
    printf("valid enumerations are: \n");
    uint32_t max_len = 0;
    for (std::vector<value_str_t>::const_iterator it = enum_list_.begin(); it != enum_list_.end(); ++it) {
        value_str_t value_string = *(it);
        uint32_t len = strlen(value_string.str);
        if (len > max_len)
            max_len = len;
    }
    for (std::vector<value_str_t>::const_iterator it = enum_list_.begin(); it != enum_list_.end(); ++it) {
        value_str_t value_string = *(it);
        printf("  %*s 0x%02x %s\n",-max_len,value_string.str,value_string.value,value_string.usage_message);
    }
    exit(-1);
    return FALSE; // never gets here
}

/**
 * @brief
 *   parse the enumeration
 *
 * @param[in] s - enumeration to parse.
 * @param[out] error_message - std::ostream for error message if value was unable to be parsed
 *
 * @return bool8_t - TRUE if enumeration was valid, FALSE and error message in 'error_message'
 */
bool8_t EnumOption::ParseValueWithError(const char *s, std::ostream &error_message)
{
    if (find_enum(&enum_list_,s,&value))
    {
        return TRUE;
    }
    char *temp;
    value = parse_int(s,&temp);
    if (*temp == 0)
    {
        return TRUE;
    }

    // save format flags
    std::ios_base::fmtflags f( error_message.flags() );
    error_message << "unknown "
        << name 
        << " \""
        << s 
        << "\"" 
        << "\n";
    error_message << "valid enumerations are: " << "\n";
    uint32_t max_len = 0;
    for (std::vector<value_str_t>::const_iterator it = enum_list_.begin(); it != enum_list_.end(); ++it) {
        value_str_t value_string = *(it);
        uint32_t len = strlen(value_string.str);
        if (len > max_len)
            max_len = len;
    }
    for (std::vector<value_str_t>::const_iterator it = enum_list_.begin(); it != enum_list_.end(); ++it) {
        value_str_t value_string = *(it);
        error_message << "  "
            << std::left << std::setw(max_len) << value_string.str
            << " "
            << value_string.usage_message << "\n";
    }
    error_message.flags( f );
    return FALSE;
}



/**
 * @brief
 *   returns the string associated with that value. (defined by AddEnum())
 *
 * @param[in] x - value
 *
 * @return string
 */
const char *EnumOption::GetString( uint32_t x ) {
    for (std::vector<value_str_t>::const_iterator it = enum_list_.begin(); it != enum_list_.end(); ++it) {
        value_str_t value_string = *(it);
        if (value_string.value == x) {
            return value_string.str;
        }
    }
    return "undefined";
}

/**
 * @brief
 *   constructor
 *
 * @param[in] default_value - default value if not specified on command line
 * @param[in] _name - option name
 * @param[in] _usage_message - option usage message
 */
LogLevelOption::LogLevelOption( uint32_t default_value, const char *_name, const char *_usage_message ) : EnumOption(default_value,_name,_usage_message) {
    AddEnum(0,"DEBUG");
    AddEnum(1,"INFO");
    AddEnum(2,"WARNING");
    AddEnum(3,"ERROR");
    AddEnum(4,"CRITICAL");
    SetFromEnvironmentVariable();
}



/**
 * @brief
 *   called whenever log level level option is set.
 */
void LogLevelOption::OptionSet()
{
    EnumOption::OptionSet();

    // maybe the code doesn't want to use option_log_level.value and instead
    // has an api that should be called to set the log level.
    // e.g. perhaps you want to make a function call with the command line log level:
    // log_level_set( value )
}

/**
 * @brief
 *   constructor
 *
 * @param[in] _name - name of option
 * @param[in] _usage_message - usage message
 */
EnumPairOption::EnumPairOption( const char *_name, const char *_usage_message ) : 
  CmdLineOption(_name, _usage_message),
  first_value(),
  second_value()
{
}

/**
 * @brief
 *   reset value to default
 */
void EnumPairOption::Reset()
{
    is_set = false;
}

/**
 * @brief
 *   constructor
 *
 * @param[in] _first_value - enumeration value
 * @param[in] _str - associated string
 * @param[in] _usage_message - option usage message
 */
void EnumPairOption::AddFirstEnum(uint32_t _first_value, const char *_str, const char *_usage_message)
{
    value_str_t value_string;
    value_string.value = _first_value;
    value_string.str = _str;
    value_string.usage_message = _usage_message;
    first_enum_list_.push_back(value_string);
}

/**
 * @brief
 *   constructor
 *
 * @param[in] _first_value - enumeration value
 * @param[in] _second_value - enumeration value
 * @param[in] _str - associated string
 * @param[in] _usage_message - option usage message
 */
void EnumPairOption::AddPairEnum(uint32_t _first_value, uint32_t _second_value, const char *_str, const char *_usage_message)
{
    value_pair_str_t value_pair_string;
    value_pair_string.first_value = _first_value;
    value_pair_string.second_value = _second_value;
    value_pair_string.str = _str;
    value_pair_string.usage_message = _usage_message;
    pair_enum_list_.push_back(value_pair_string);
}

/**
 * @brief
 *   parse the command line option
 *
 * @param[in] s - default value if not specified on command line
 *
 * @return bool8_t - TRUE if option was valid.
 */
bool8_t EnumPairOption::ParseValue( const char *s ) {
    uint32_t max_len;
    for (std::vector<value_pair_str_t>::const_iterator it = pair_enum_list_.begin(); it != pair_enum_list_.end(); ++it) {
        value_pair_str_t value_pair_string = *(it);
        if (my_stricmp(value_pair_string.str,s)==0) {
            first_value = value_pair_string.first_value;
            second_value = value_pair_string.second_value;
            return TRUE;
        }
    }

    const char *dot = strchr(s,'.');
    if (dot != NULL) {
        uint32_t dot_pos = dot - s;
        if (dot_pos < 100) {
            char first_str[101];
            char *temp;
            strncpy(first_str,s,dot_pos);
            first_str[dot_pos]=0;
            printf("looking for value of '%s'\n",first_str);
            if (find_enum(&first_enum_list_,first_str,&first_value)) {
                temp = (char *)"";
            } else {
                first_value = parse_int(first_str,&temp);
            }
            if (*temp == 0) {
                dot++;
                printf("parsing second value from '%s'\n",dot);
                second_value = parse_int(dot,&temp);
                if (*temp == 0) {
                    return TRUE;
                }
            }
        }
    }

    printf("unknown %s \"%s\"\n",name,s);
    printf("valid enumerations are:\n");
    max_len = 0;
    for (std::vector<value_str_t>::const_iterator it = first_enum_list_.begin(); it != first_enum_list_.end(); ++it) {
        value_str_t value_string = *(it);
        uint32_t len = strlen(value_string.str);
        if (len > max_len)
            max_len = len;
    }
    for (std::vector<value_pair_str_t>::const_iterator it = pair_enum_list_.begin(); it != pair_enum_list_.end(); ++it) {
        value_pair_str_t value_pair_string = *(it);
        uint32_t len = strlen(value_pair_string.str);
        if (len > max_len)
            max_len = len;
    }
    for (std::vector<value_str_t>::const_iterator it = first_enum_list_.begin(); it != first_enum_list_.end(); ++it) {
        value_str_t value_string = *(it);
        printf("  %*s  0x%02x.x %s\n",-max_len,value_string.str,value_string.value,value_string.usage_message);
    }
    printf("\n");

    for (std::vector<value_pair_str_t>::const_iterator it = pair_enum_list_.begin(); it != pair_enum_list_.end(); ++it) {
        value_pair_str_t value_pair_string = *(it);
        printf("  %*s  0x%02x.%d %s\n",-max_len,value_pair_string.str,value_pair_string.first_value,value_pair_string.second_value,value_pair_string.usage_message);
    }
    exit(-1);
    return FALSE; // never gets here
}

/**
 * @brief
 *   constructor
 *
 * @param[in] _name - option name
 * @param[in] _usage_message - option usage message
 */
IntRangeOption::IntRangeOption( const char *_name, const char *_usage_message )
 : CmdLineOption(_name, _usage_message),
   start_value(),
   end_value(),
   size()
{
    SetFromEnvironmentVariable();
}

/**
 * @brief
 *   reset value to default
 */
void IntRangeOption::Reset()
{
    is_set = false;
}

/**
 * @brief
 *   parse the command line option
 *
 * @param[in] s - default value if not specified on command line
 *
 * @return bool8_t - TRUE if option was valid.
 */
bool8_t IntRangeOption::ParseValue( const char *s ) {
    char *temp;
    start_value = parse_int(s,&temp);
    s = temp;
    if (*s == '+') {
        s++;
        size = parse_int(s,&temp);
        if (*temp != 0) return FALSE;

        end_value = start_value + size;

        /* printf("%s=%d (%x)\n",name,option_field,option_field); */
        return TRUE;
    }
    if (*s != '.') return FALSE;
    s++;
    if (*s != '.') return FALSE;
    s++;
    end_value = parse_int(s,&temp);
    if (*temp != 0) return FALSE;

    size = end_value - start_value;

    /* printf("%s=%d (%x)\n",name,option_field,option_field); */
    return TRUE;
}

/**
 * @brief
 *   Parse a command line option
 *
 * @param[in] s - command line argument string
 * @param[in] error_message - error message
 *
 * @return bool8_t - true if argument string is valid
 */
bool8_t IntRangeOption::ParseValueWithError( const char *s, std::ostream &error_message ) {
    if (ParseValue(s))
        return TRUE;
    error_message << "error parsing '" << s << "'\n";
    error_message << " for IntRange option '" << name << "'\n";
    error_message << " option description: " << usage_message << "\n";
    error_message << "range formats are:\n";
    error_message << "   start..end e.g. 0xd00380..0xd00388\n";
    error_message << "   start+count e.g. 0xd00380+2 (that's 0xd00380,0xd00384)\n";
    return FALSE;
}

/**
 * @brief
 *   constructor
 *
 * @param[in] _name - option name
 * @param[in] _usage_message - option usage message
 * @param[in] _default_step - default step size for ranges (e.g. integer lists that are register addresses use a default step of 4)
 */
IntListOption::IntListOption( const char *_name, const char *_usage_message, uint32_t _default_step ) : CmdLineOption(_name, _usage_message) {
    this->is_list = TRUE;
    this->default_step = _default_step;
    this->mask = 0;
    SetFromEnvironmentVariable();
}

/**
* @brief end of list parsing
*/
void IntListOption::EndOfList() {
}

/**
* @brief end of list parsing
*/
void IntListOption::Reset() {
    CmdLineOption::Reset();
    value_list_.clear();
    string_list_.clear();
}



/**
 * @brief
 *   parse the command line option
 *
 * @param[in] s - default value if not specified on command line
 *
 * @return bool8_t - TRUE if option was valid.
 */
bool8_t IntListOption::ParseValue( const char *s ) {
    char *temp;
    int32_t value;
    value = parse_int(s,&temp);
    if (*temp == 0) {
        value_list_.push_back(value);
        mask |= 1<<value;
        string_list_.push_back(s);
        return TRUE;
    }
    int32_t start_value = value;
    int32_t end_value;
    int32_t size;
    int32_t i;
    s = temp;
    if (*s == '+') {
        s++;
        size = parse_int(s,&temp);
        uint32_t step = default_step;
        if (*temp == '/') {
            s = temp;
            s++;
            step = parse_int(s,&temp);
        }
        if (*temp != 0) return FALSE;

        end_value = start_value + size * step;

        for (i=start_value;i<end_value;i+=step) {
            value_list_.push_back(i);
            mask |= 1<<i;
        }

        string_list_.push_back(s);
        /* printf("%s=%d (%x)\n",name,option_field,option_field); */
        return TRUE;
    }
    if (*s != '.') return FALSE;
    s++;
    if (*s != '.') return FALSE;
    s++;
    end_value = parse_int(s,&temp);
    if (*temp != 0) return FALSE;

    size = end_value - start_value;
    for (i=start_value;i<=end_value;i+=default_step) {
        value_list_.push_back(i);
        mask |= 1<<i;
    }

    string_list_.push_back(s);
    /* printf("%s=%d (%x)\n",name,option_field,option_field); */
    return TRUE;
}

/**
 * @brief
 *   Parse a command line option
 *
 * @param[in] s - command line argument string
 * @param[in] error_message - error message
 *
 * @return bool8_t - true if argument string is valid
 */
bool8_t IntListOption::ParseValueWithError( const char *s, std::ostream &error_message ) {
    if (ParseValue(s))
        return TRUE;
    error_message << "error parsing '" << s << "'\n";
    error_message << " for IntList option '" << name << "'\n";
    error_message << " option description: " << usage_message << "\n";
    error_message << "list formats are:\n";
    if (default_step == 0x4)
    {
        error_message << "   start..end e.g. " << name << "=0xd00380..0xd00388\n";
        error_message << "   start+count e.g. " << name << "=0xd00380+2 (that's 0xd00380,0xd00384)\n";
        error_message << "   start+count/skip e.g. " << name << "=0xd00380+2/0x20 (that's 0xd00380,0xd003A0) \n";
    }
    else
    {
        error_message << "   start..end e.g. " << name << "=0..10\n";
        error_message << "   start+count e.g. " << name << "=5+2 (that's 5,6)\n";
        error_message << "   start+count/skip e.g. " << name << "=11+3/100 (that's 11,111,211) \n";
    }
    return FALSE;
}



/**
 * @brief
 *   add a value to an integer list
 *
 * @param[in] value - value to add
 */
void IntListOption::AddValue( const int32_t value ) {
    this->value_list_.push_back(value);
    this->mask |= 1<<value;
}

/**
 * @brief
 *   same as a StringList, but stop if you find another command line option.
 *
 * @param[in] _name - option name
 * @param[in] _usage_message - option usage message
 */
OptionFreeStringListOption::OptionFreeStringListOption( const char *_name, const char *_usage_message ) : StringListOption(_name, _usage_message) {
    this->is_option_free_list = TRUE;
    SetFromEnvironmentVariable();
}

/**
 * @brief
 *   constructor
 *
 * @param[in] _name - option name
 * @param[in] _usage_message - option usage message
 */
StringListOption::StringListOption( const char *_name, const char *_usage_message ) : CmdLineOption(_name, _usage_message) {
    this->is_list = TRUE;
    SetFromEnvironmentVariable();
}

/**
* @brief end of list parsing
*/
void StringListOption::EndOfList() {
}

/**
* @brief end of list parsing
*/
void StringListOption::Reset() {
    CmdLineOption::Reset();
    string_list_.clear();
}



/**
 * @brief
 *   parse the command line option
 *
 * @param[in] s - default value if not specified on command line
 *
 * @return bool8_t - TRUE if option was valid.
 */
bool8_t StringListOption::ParseValue( const char *s ) {
    string_list_.push_back(s);
    return TRUE;
}

/**
 * @brief
 *   constructor
 *
 * @param[in] default_value - default value if not specified on command line
 * @param[in] _name - option name
 * @param[in] _usage_message - option usage message
 */
DoubleOption::DoubleOption( double default_value, const char *_name, const char *_usage_message ) : CmdLineOption(_name, _usage_message), value(default_value),_default_value(default_value) {
    SetFromEnvironmentVariable();
}

/**
 * @brief
 *   reset value to default
 */
void DoubleOption::Reset()
{
    is_set = false;
    value = _default_value;
}

/**
 * @brief
 *   parse the command line option
 *
 * @param[in] s - default value if not specified on command line
 *
 * @return bool8_t - TRUE if option was valid.
 */
bool8_t DoubleOption::ParseValue( const char *s ) {
    char *temp = NULL;
    value = strtod(s,&temp);
    if (*temp == '/') {
        temp++;
        double denominator = strtod(temp,&temp);
        double numerator = value;
        value = numerator / denominator;
        printf("setting %s to %g/%g = %g\n",name,numerator,denominator,value);
    }

    if (*temp != 0) {
        return FALSE;
    } else {
        return TRUE;
    }
}

/**
 * @brief
 *   Parse a command line option
 *
 * @param[in] s - command line argument string
 * @param[in] error_message - error message
 *
 * @return bool8_t - true if argument string is valid
 */
bool8_t DoubleOption::ParseValueWithError( const char *s, std::ostream &error_message ) {
    if (ParseValue(s))
        return TRUE;
    error_message << "error parsing '" << s << "'\n";
    error_message << " for Double option '" << name << "'\n";
    error_message << " option description: " << usage_message << "\n";
    error_message << "double is parsed with strtod, or numerator/denominator\n";
    error_message << "   " << name << "=" << "11/20\n";
    return FALSE;
}

/**
 * @brief
 *   constructor
 *
 * @param[in] default_value - default value if not specified on command line
 * @param[in] _name - option name
 * @param[in] _usage_message - option usage message
 */
StringOption::StringOption( const char *default_value, const char *_name, const char *_usage_message ) : CmdLineOption(_name, _usage_message), value(default_value),_default_value(default_value) {
    SetFromEnvironmentVariable();
}

/**
 * @brief
 *   reset value to default
 */
void StringOption::Reset()
{
    is_set = false;
    value = _default_value;
}

/**
 * @brief
 *   parse the command line option
 *
 * @param[in] s - default value if not specified on command line
 *
 * @return bool8_t - TRUE if option was valid.
 */
bool8_t StringOption::ParseValue( const char *s ) {
    value = s;
    return TRUE;
}

/**
* @brief
*   display a usage message to stdout
*/
void CmdLineOptions::Usage()
 {
    uint32_t max_len = 0;
    for (std::vector<CmdLineOption*>::const_iterator it = option_list_.begin(); it != option_list_.end(); ++it) {
        CmdLineOption *option = *(it);
        uint32_t len = strlen(option->name);
        if (len > max_len) max_len = len;
    }
    for (std::vector<CmdLineOption*>::const_iterator it = option_list_.begin(); it != option_list_.end(); ++it) {
        CmdLineOption *option = *(it);
        if (*option->name == 0) {
            printf("%s\n",option->usage_message);
        } else {
            printf("  %*s - %s\n",-max_len,option->name,option->usage_message);
        }
    }
    exit(-1);
}



/**
* @brief
*   display a usage message to ostream
*
* @param[out] error_message - output stream to format error message.
*/
void CmdLineOptions::ShowUsage(std::ostream &error_message)
{
    std::ios_base::fmtflags f( error_message.flags() );
    uint32_t max_len = 0;
    for (std::vector<CmdLineOption*>::const_iterator it = option_list_.begin(); it != option_list_.end(); ++it) {
        CmdLineOption *option = *(it);
        uint32_t len = strlen(option->name);
        if (len > max_len) max_len = len;
    }
    for (std::vector<CmdLineOption*>::const_iterator it = option_list_.begin(); it != option_list_.end(); ++it) {
        CmdLineOption *option = *(it);
        if (*option->name == 0) {
            error_message << option->usage_message << "\n";
        } else {
            error_message << "  " << std::left << std::setw(max_len) << option->name
                << " " << option->usage_message
                << "\n";
        }
    }
    error_message.flags( f );
}


/**
* @brief
*   reset options to default
*/
void CmdLineOptions::Reset()
 {
    for (std::vector<CmdLineOption*>::const_iterator it = option_list_.begin(); it != option_list_.end(); ++it) {
        CmdLineOption *option = *(it);
        option->Reset();
    }
    std::vector<const char *>::const_iterator it;
    for (it = tokens_allocated_by_ParseString.begin(); it != tokens_allocated_by_ParseString.end(); ++it) {
        const char *arg = *it;
        free((void *)arg);
    }
    tokens_allocated_by_ParseString.clear();
}

/**
* @brief
*   parse a space separated list of arguments as if they were specified as command line arguments
*
* @param[in] argv_string - space separated list of command line arguments
*/
void CmdLineOptions::ParseString(const char *argv_string)
{
    char *argv_str = strdup(argv_string);
    char *token;
    std::vector<const char *> argv_vector;

    /* get the first token */
    token = strtok(argv_str, " ");

    argv_vector.push_back("parse_string");

    /* walk through other tokens */
    while( token != NULL )
    {
        char *arg = strdup(token);
        tokens_allocated_by_ParseString.push_back(arg); /* note: this is not used,... just saved to avoid valgrind from detecting a memory leak */
        argv_vector.push_back((char *)arg);

        token = strtok(NULL, " ");
    }
    free(argv_str);

    /* parse options with alias'd argc/argv */
    CmdLineOptions::GetInstance()->ParseOptions(argv_vector.size(),&(argv_vector[0]));
}


/**
* @brief
*   returns true if the string matches a valid command line option
*
* @param[in] s - string
* @return true if 's' matches a command line option, false otherwise.
*/
bool8_t CmdLineOptions::MatchesAnOption( const char *s )
{
    char token[100];
    char *equals;

    /* make the '-' optional */
    if (s[0] == '-') {
        s++;
        if (s[0] == '-') {
            s++;
        }
    }
    strncpy(token,s,99);
    equals = strchr(token,'=');
    if (equals != NULL) {
        *equals=0;
    }
    for (std::vector<CmdLineOption*>::const_iterator it = option_list_.begin(); it != option_list_.end(); ++it) {
        CmdLineOption *option = *(it);
        if (strcmp(token,option->name) == 0) {
            return TRUE;
        }
    }
    return false;
}

/**
* @brief
*   an extern "C" callable version of CmdLineOptions::GetInstance->ParseOptions(argc,argv)
*
* @param[in] argc - number of arguments
* @param[in] argv - argument strings
*/
extern "C" void cmd_line_options_parse_options( int argc, const char **argv )
{
    CmdLineOptions::GetInstance()->ParseOptions(argc,argv);
}

/**
* @brief
*   parse command line options
*
* @param[in] argc - number of arguments
* @param[in] argv - argument strings
*/
void CmdLineOptions::ParseOptions( int argc, const char **argv ) {
    int i;
    if (strcmp(argv[0],"parse_string") != 0)
    {
        std::string window_title;
        if (strrchr(argv[0],'/') != NULL)
            window_title = strrchr(argv[0],'/')+1;
        else
            window_title = argv[0];
        for (i = 1;i<argc;i++) {
            const char *s = argv[i];
            window_title += " ";
            window_title += s;
        }
        // modify the xterm window title to match the program name
        printf("\033]0;%s\007",window_title.c_str());
    }
    for (i = 1;i<argc;i++) {
        const char *s = argv[i];
        char token[100];
        const char *val_str;
        char *equals;

        /* make the '-' optional */
        if (s[0] == '-') {
            s++;
            if (s[0] == '-') {
                s++;
            }
        }
        strncpy(token,s,99);
        equals = strchr(token,'=');
        if (equals == NULL) {
            val_str = (char *)"";
        } else {
            *equals=0;
            equals++;
            val_str = &s[equals-token];
        }
        bool8_t matched = FALSE;
        for (std::vector<CmdLineOption*>::const_iterator it = option_list_.begin(); it != option_list_.end(); ++it) {
            CmdLineOption *option = *(it);
            if (strcmp(token,option->name) == 0) {
                if (option->is_list) {
                    for (i=i+1;i<argc;i++) {
                        // for OptionFreeStringList, terminate the list
                        // if you find something that looks like another command line option
                        if (option->is_option_free_list)
                        {
                            if (MatchesAnOption(argv[i]))
                            {
                                i--;
                                break;
                            }
                        }
                        if (!option->ParseValue(argv[i])) {
                            /* start parsing arguments again at 'i',... so back i up one... */
                            i--;
                            break;
                        }
                    }
                    option->EndOfList();
                } else {
                    if (!option->ParseValue(val_str)) {
                        printf("error parsing '%s'\n",argv[i]);
                        Usage();
                    }
                }
                option->OptionSet();
                option->is_set = TRUE;
                matched = TRUE;
            }
        }
        if (!matched) {
            printf("no match for '%s'\n",token);
            Usage();
        }
    }
}

/**
* @brief
*   check the environment variable project_name
*
* Unfortunately, this cannot be called from the base class constructor,
* because the virtual function ParseValue() is not initialized until the derived class is constructed.
*/
void CmdLineOption::SetFromEnvironmentVariable()
{
    char env_name[100];
    snprintf(env_name,sizeof(env_name),"PROJECT_NAME_%s",name);
    char *env_value = getenv(env_name);
    if (env_value == NULL) {
        // also look for the uppercase version of the option
        for (char *s = env_name;*s!=0;s++)
        {
            if (islower(*s))
            {
                *s = toupper(*s);
            }
        }
        env_value = getenv(env_name);
    }
    if (env_value != NULL) {
        printf("setting %s to \"%s\" (from environment variable %s)\n",name,env_value,env_name);
        if (!ParseValue(env_value)) {
            printf("error parsing '%s'\n",env_value);
            CmdLineOptions::GetInstance()->Usage();
        }
        OptionSet();
        is_set = TRUE;
    }
}

/**
* @brief
*   add an option to the global list of options
*
* @param[in] option - option to add
*/
void CmdLineOptions::AddOption(CmdLineOption *option)
{
    option_list_.push_back(option);
}

/**
* @brief
*   parse command line options or generate error message
*
* @param[in] argc - number of arguments
* @param[in] argv - argument strings
* @param[in] error_message - error message
*
* @return TRUE if successful, FALSE otherwise.
*/
bool8_t CmdLineOptions::ParseOptionsOrError( int argc, const char **argv, std::ostream &error_message ) 
{
    for (int i = 0;i<argc;i++) {
        const char *s = argv[i];
        char token[100];
        const char *val_str;
        char *equals;

        /* make the '-' optional */
        if (s[0] == '-') {
            s++;
        }
        if (s[0] == '-') {
            s++;
        }
        strncpy(token,s,99);
        equals = strchr(token,'=');
        if (equals == NULL) {
            val_str = (char *)"";
        } else {
            *equals=0;
            equals++;
            val_str = &s[equals-token];
        }
        bool8_t matched = FALSE;
        for (std::vector<CmdLineOption*>::const_iterator it = option_list_.begin(); it != option_list_.end(); ++it) {
            CmdLineOption *option = *(it);
            if (strcmp(token,option->name) == 0) {
                if (option->is_list) {
                    for (i=i+1;i<argc;i++) {
                        if (!option->ParseValue(argv[i])) {
                            /* start parsing arguments again at 'i',... so back i up one... */
                            i--;
                            break;
                        }
                    }
                    option->EndOfList();
                } else {
                    if (!option->ParseValueWithError(val_str,error_message)) {
                        error_message << "error parsing \"" << argv[i] << "\"" << "\n";
                        return FALSE;
                    }
                }
                option->OptionSet();
                option->is_set = TRUE;
                matched = TRUE;
            }
        }
        if (!matched) {
            error_message << "no match for option \"" << token << "\"" << "\n";
            ShowUsage(error_message);
            return FALSE;
        }
    }
    return TRUE;
}

CmdLineOptions::~CmdLineOptions()
{
    std::vector<const char *>::const_iterator it;
    for (it = tokens_allocated_by_ParseString.begin(); it != tokens_allocated_by_ParseString.end(); ++it) {
        const char *arg = *it;
        free((void *)arg);
    }
    tokens_allocated_by_ParseString.clear();
}

/* global options */
/// verbosity command line option
LogLevelOption option_log_level( 1, "log_level", "set SW APPLIB logging level (0=DEBUG,1=INFO,2=WARNING,3=ERROR,4=CRITICAL)");
