//  COPYRIGHT (C) 2022 Microchip with MIT license

/**
 * @file
 * @brief
 * Source file of command line option parsing.
 */

#include <string.h>
#include <iostream>
#include <sstream>
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
 * @return bool - true if argument string is valid
 */
bool CmdLineOption::ParseValueWithError( const char *s, std::ostream &error_message ) {
    if (ParseValue(s))
        return true;
    error_message << "error parsing '" << s << "'\n";
    error_message << " for option '" << name << "'\n";
    error_message << " option description: " << usage_message << "\n";
    return false;
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
BoolOption::BoolOption( bool default_value, const char *_name, const char *_usage_message ) : CmdLineOption(_name, _usage_message), value(default_value), _default_value(default_value) {
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
 * @return bool - true if argument string is valid
 */
bool IntOption::ParseValue( const char *s ) {
    char *temp;
    value = parse_int(s,&temp);
    if (*temp != 0) return false;
    /* printf("%s=%d (%x)\n",name,option_field,option_field); */
    return true;
}

/**
 * @brief
 *   Parse a command line option
 *
 * @param[in] s - command line argument string
 * @param[in] error_message - error message
 *
 * @return bool - true if argument string is valid
 */
bool IntOption::ParseValueWithError( const char *s, std::ostream &error_message ) {
    if (ParseValue(s))
        return true;
    error_message << "error parsing '" << s << "'\n";
    error_message << " for int option '" << name << "'\n";
    error_message << " option description: " << usage_message << "\n";
    return false;
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
 * @return bool - true if option was valid.
 */
bool UintOption::ParseValue( const char *s ) {
    char *temp;
    value = parse_uint(s,&temp);
    if (*temp != 0) return false;
    /* printf("%s=%d (%x)\n",name,option_field,option_field); */
    return true;
}

/**
 * @brief
 *   Parse a command line option
 *
 * @param[in] s - command line argument string
 * @param[in] error_message - error message
 *
 * @return bool - true if argument string is valid
 */
bool UintOption::ParseValueWithError( const char *s, std::ostream &error_message ) {
    if (ParseValue(s))
        return true;
    error_message << "error parsing '" << s << "'\n";
    error_message << " for uint option '" << name << "'\n";
    error_message << " option description: " << usage_message << "\n";
    return false;
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
 * @return bool - true if option was valid.
 */
bool Int64Option::ParseValue( const char *s ) {
    char *temp;
    value = parse_int64(s,&temp);
    if (*temp != 0) return false;
    /* printf("%s=%d (%x)\n",name,option_field,option_field); */
    return true;
}

/**
 * @brief
 *   Parse a command line option
 *
 * @param[in] s - command line argument string
 * @param[in] error_message - error message
 *
 * @return bool - true if argument string is valid
 */
bool Int64Option::ParseValueWithError( const char *s, std::ostream &error_message ) {
    if (ParseValue(s))
        return true;
    error_message << "error parsing '" << s << "'\n";
    error_message << " for int64 option '" << name << "'\n";
    error_message << " option description: " << usage_message << "\n";
    return false;
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
 * @return bool - true if option was valid.
 */
bool Uint64Option::ParseValue( const char *s ) {
    char *temp;
    value = parse_uint64(s,&temp);
    if (*temp != 0) return false;
    /* printf("%s=%d (%x)\n",name,option_field,option_field); */
    return true;
}

/**
 * @brief
 *   Parse a command line option
 *
 * @param[in] s - command line argument string
 * @param[in] error_message - error message
 *
 * @return bool - true if argument string is valid
 */
bool Uint64Option::ParseValueWithError( const char *s, std::ostream &error_message ) {
    if (ParseValue(s))
        return true;
    error_message << "error parsing '" << s << "'\n";
    error_message << " for uint64 option '" << name << "'\n";
    error_message << " option description: " << usage_message << "\n";
    return false;
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
 * @return bool - true if option was valid.
 */
bool BoolOption::ParseValue( const char *s ) {
    if ((my_stricmp(s,"") == 0) ||
        (my_stricmp(s,"1") == 0) ||
        (my_stricmp(s,"on") == 0) ||
        (my_stricmp(s,"yes") == 0) ||
        (my_stricmp(s,"true") == 0)) {
        value = true;
        return true;
    }
    if ((my_stricmp(s,"0") == 0) ||
        (my_stricmp(s,"no") == 0) ||
        (my_stricmp(s,"off") == 0) ||
        (my_stricmp(s,"false") == 0)) {
        value = false;
        return true;
    }
    return false;
}

/**
 * @brief
 *   Parse a command line option
 *
 * @param[in] s - command line argument string
 * @param[in] error_message - error message
 *
 * @return bool - true if argument string is valid
 */
bool BoolOption::ParseValueWithError( const char *s, std::ostream &error_message ) {
    if (ParseValue(s))
        return true;
    error_message << "error parsing '" << s << "'\n";
    error_message << " for bool option '" << name << "'\n";
    error_message << " option description: " << usage_message << "\n";
    error_message << "valid values are 0,1,no,yes,off,on,false,true\n";
    return false;
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
 * @return bool - true if a match was found
 */
static bool find_enum(std::vector<value_str_t> *enum_list, const char *s, uint32_t *value ) {
    for (std::vector<value_str_t>::const_iterator it = enum_list->begin(); it != enum_list->end(); ++it) {
        value_str_t value_string = *(it);
        if (my_stricmp(value_string.str,s)==0) {
            *value = value_string.value;
            return true;
        }
    }
    return false;
}


/**
 * @brief
 *   parse the enumeration
 *
 * @param[in] s - enumeration to parse
 *
 * @return bool - true if enumeration was valid.
 */
bool EnumOption::ParseValue( const char *s ) {
    if (find_enum(&enum_list_,s,&value)) {
        return true;
    }
    char *temp;
    value = parse_int(s,&temp);
    if (*temp == 0) return true;
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
    return false; // never gets here
}

/**
 * @brief
 *   parse the enumeration
 *
 * @param[in] s - enumeration to parse.
 * @param[out] error_message - std::ostream for error message if value was unable to be parsed
 *
 * @return bool - true if enumeration was valid, false and error message in 'error_message'
 */
bool EnumOption::ParseValueWithError(const char *s, std::ostream &error_message)
{
    if (find_enum(&enum_list_,s,&value))
    {
        return true;
    }
    char *temp;
    value = parse_int(s,&temp);
    if (*temp == 0)
    {
        return true;
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
    return false;
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
 * @return bool - true if option was valid.
 */
bool IntRangeOption::ParseValue( const char *s ) {
    char *temp;
    start_value = parse_int(s,&temp);
    s = temp;
    if (*s == '+') {
        s++;
        size = parse_int(s,&temp);
        if (*temp != 0) return false;

        end_value = start_value + size;

        /* printf("%s=%d (%x)\n",name,option_field,option_field); */
        return true;
    }
    if (*s != '.') return false;
    s++;
    if (*s != '.') return false;
    s++;
    end_value = parse_int(s,&temp);
    if (*temp != 0) return false;

    size = end_value - start_value;

    /* printf("%s=%d (%x)\n",name,option_field,option_field); */
    return true;
}

/**
 * @brief
 *   Parse a command line option
 *
 * @param[in] s - command line argument string
 * @param[in] error_message - error message
 *
 * @return bool - true if argument string is valid
 */
bool IntRangeOption::ParseValueWithError( const char *s, std::ostream &error_message ) {
    if (ParseValue(s))
        return true;
    error_message << "error parsing '" << s << "'\n";
    error_message << " for IntRange option '" << name << "'\n";
    error_message << " option description: " << usage_message << "\n";
    error_message << "range formats are:\n";
    error_message << "   start..end e.g. 0xd00380..0xd00388\n";
    error_message << "   start+count e.g. 0xd00380+2 (that's 0xd00380,0xd00384)\n";
    return false;
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
    this->is_list = true;
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
 * @return bool - true if option was valid.
 */
bool IntListOption::ParseValue( const char *s ) {
    char *temp;
    int32_t value;
    value = parse_int(s,&temp);
    if (*temp == 0) {
        value_list_.push_back(value);
        mask |= 1<<value;
        string_list_.push_back(s);
        return true;
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
        if (*temp != 0) return false;

        end_value = start_value + size * step;

        for (i=start_value;i<end_value;i+=step) {
            value_list_.push_back(i);
            mask |= 1<<i;
        }

        string_list_.push_back(s);
        /* printf("%s=%d (%x)\n",name,option_field,option_field); */
        return true;
    }
    if (*s != '.') return false;
    s++;
    if (*s != '.') return false;
    s++;
    end_value = parse_int(s,&temp);
    if (*temp != 0) return false;

    size = end_value - start_value;
    for (i=start_value;i<=end_value;i+=default_step) {
        value_list_.push_back(i);
        mask |= 1<<i;
    }

    string_list_.push_back(s);
    /* printf("%s=%d (%x)\n",name,option_field,option_field); */
    return true;
}

/**
 * @brief
 *   Parse a command line option
 *
 * @param[in] s - command line argument string
 * @param[in] error_message - error message
 *
 * @return bool - true if argument string is valid
 */
bool IntListOption::ParseValueWithError( const char *s, std::ostream &error_message ) {
    if (ParseValue(s))
        return true;
    error_message << "error parsing '" << s << "'\n";
    error_message << " for IntList option '" << name << "'\n";
    error_message << " option description: " << usage_message << "\n";
    error_message << "list formats are:\n";
    if (default_step == 0x4)
    {
        error_message << "   start..end       e.g. " << name << " 0xd00380..0xd00388\n";
        error_message << "   start+count      e.g. " << name << " 0xd00380+2 (that's 0xd00380 0xd00384)\n";
        error_message << "   start+count/skip e.g. " << name << " 0xd00380+2/0x20 (that's 0xd00380 0xd003A0) \n";
    }
    else
    {
        error_message << "   start..end       e.g. " << name << " 0..10\n";
        error_message << "   start+count      e.g. " << name << " 5+2 (that's 5 6)\n";
        error_message << "   start+count/skip e.g. " << name << " 11+3/100 (that's 11 111 211) \n";
    }
    return false;
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
    this->is_option_free_list = true;
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
    this->is_list = true;
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
 * @return bool - true if option was valid.
 */
bool StringListOption::ParseValue( const char *s ) {
    string_list_.push_back(s);
    return true;
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
 * @return bool - true if option was valid.
 */
bool DoubleOption::ParseValue( const char *s ) {
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
        return false;
    } else {
        return true;
    }
}

/**
 * @brief
 *   Parse a command line option
 *
 * @param[in] s - command line argument string
 * @param[in] error_message - error message
 *
 * @return bool - true if argument string is valid
 */
bool DoubleOption::ParseValueWithError( const char *s, std::ostream &error_message ) {
    if (ParseValue(s))
        return true;
    error_message << "error parsing '" << s << "'\n";
    error_message << " for Double option '" << name << "'\n";
    error_message << " option description: " << usage_message << "\n";
    error_message << "double is parsed with strtod, or numerator/denominator\n";
    error_message << "   " << name << "=" << "11/20\n";
    return false;
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
 * @return bool - true if option was valid.
 */
bool StringOption::ParseValue( const char *s ) {
    value = s;
    return true;
}

/**
* @brief
*   display a usage message to stdout
*/
void CmdLineOptions::Usage()
 {
    uint32_t max_len = 0;
    for (std::vector<CmdLineOption*>::const_iterator it = _option_list.begin(); it != _option_list.end(); ++it) {

        CmdLineOption *option = *(it);
        uint32_t len = strlen(option->name);
        if (len > max_len) {
            max_len = len;
        }
    }
    for (std::vector<CmdLineOption*>::const_iterator it = _option_list.begin(); it != _option_list.end(); ++it) {
        CmdLineOption *option = *(it);
        if (*option->name == 0) {
            printf("%s\n",option->usage_message);
        } else {
            // print option name left justified in the first column
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
    for (std::vector<CmdLineOption*>::const_iterator it = _option_list.begin(); it != _option_list.end(); ++it) {

        CmdLineOption *option = *(it);
        uint32_t len = strlen(option->name);
        if (len > max_len) {
            max_len = len;
        }
    }
    for (std::vector<CmdLineOption*>::const_iterator it = _option_list.begin(); it != _option_list.end(); ++it) {
        CmdLineOption *option = *(it);
        if (*option->name == 0) {
            error_message << option->usage_message << "\n";
        } else {
            // print option name left justified in the first column
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
    for (std::vector<CmdLineOption*>::const_iterator it = _option_list.begin(); it != _option_list.end(); ++it) {
        CmdLineOption *option = *(it);
        option->Reset();
    }
    std::vector<const char *>::const_iterator it;
    for (it = _tokens_allocated_by_ParseString.begin(); it != _tokens_allocated_by_ParseString.end(); ++it) {
        const char *arg = *it;
        free((void *)arg);
    }
    _tokens_allocated_by_ParseString.clear();
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
        _tokens_allocated_by_ParseString.push_back(arg); /* note: this is not used,... just saved to avoid valgrind from detecting a memory leak */
        argv_vector.push_back((char *)arg);

        token = strtok(NULL, " ");
    }
    free(argv_str);

    /* parse options with created argc/argv */
    CmdLineOptions::ParseOptions(argv_vector.size(),&(argv_vector[0]));
}


/**
* @brief
*   returns true if the string matches a valid command line option
*
* @param[in] s - string
* @return true if 's' matches a command line option, false otherwise.
*/
bool CmdLineOptions::MatchesAnOption( const char *s )
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
    for (std::vector<CmdLineOption*>::const_iterator it = _option_list.begin(); it != _option_list.end(); ++it) {
        CmdLineOption *option = *(it);
        if (strcmp(token,option->name) == 0) {
            return true;
        }
    }
    return false;
}

/**
* @brief
*   an extern "C" callable version of CmdLineOptions::ParseOptions(argc,argv)
*
* @param[in] argc - number of arguments
* @param[in] argv - argument strings
*/
extern "C" void cmd_line_options_parse_options( int argc, const char **argv )
{
    CmdLineOptions::ParseOptions(argc,argv);
}

/**
* @brief
*   Static class menthod to wrap calls to singleton to parse options
*
* @param[in] argc - number of arguments
* @param[in] argv - argument strings
*/
void CmdLineOptions::ParseOptions( int argc, const char **argv ) {
    CmdLineOptions::GetInstance()->ParseOptionsInternal(argc,argv);
}

/**
* @brief
*   parse command line options
*
* @param[in] argc - number of arguments
* @param[in] argv - argument strings
*/
void CmdLineOptions::ParseOptionsInternal( int argc, const char **argv ) {
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
        // printf("\033]0;%s\007",window_title.c_str());
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
        bool matched = false;
        for (std::vector<CmdLineOption*>::const_iterator it = _option_list.begin(); it != _option_list.end(); ++it) {
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
                            if (MatchesAnOption(argv[i]))
                            {
                                i--;
                                break;
                            }
                            if (!option->ParseValue (argv[i])) {
                                // if the next option doesn't match an option,
                                // return an error message.
                                if (!MatchesAnOption(argv[i])) {
                                    std::stringstream error_message; 
                                    printf("error parsing list item '%s'\n",argv[i]);
                                    if (!option->ParseValueWithError(argv[i],error_message)) {
                                        printf("%s",error_message.str().c_str());
                                    }
                                    Usage();
                                }
                            }
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
                option->is_set = true;
                matched = true;
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
        is_set = true;
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
    _option_list.push_back(option);
}

/**
* @brief
*   parse command line options or generate error message
*
* @param[in] argc - number of arguments
* @param[in] argv - argument strings
* @param[in] error_message - error message
*
* @return true if successful, false otherwise.
*/
bool CmdLineOptions::ParseOptionsOrError( int argc, const char **argv, std::ostream &error_message ) 
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
        bool matched = false;
        for (std::vector<CmdLineOption*>::const_iterator it = _option_list.begin(); it != _option_list.end(); ++it) {
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
                        if (!option->ParseValueWithError(argv[i],error_message)) {
                            // if the next option doesn't match an option,
                            // return an error message.
                            if (!MatchesAnOption(argv[i]))
                            {
                                error_message << "error parsing \"" << argv[i] << "\"" << "\n";
                                return false;
                            }
                            /* start parsing arguments again at 'i',... so back i up one... */
                            i--;
                            break;
                        }
                    }
                    option->EndOfList();
                } else {
                    if (!option->ParseValueWithError(val_str,error_message)) {
                        error_message << "error parsing \"" << argv[i] << "\"" << "\n";
                        return false;
                    }
                }
                option->OptionSet();
                option->is_set = true;
                matched = true;
            }
        }
        if (!matched) {
            error_message << "no match for option \"" << token << "\"" << "\n";
            ShowUsage(error_message);
            return false;
        }
    }
    return true;
}

CmdLineOptions::~CmdLineOptions()
{
    std::vector<const char *>::const_iterator it;
    for (it = _tokens_allocated_by_ParseString.begin(); it != _tokens_allocated_by_ParseString.end(); ++it) {
        const char *arg = *it;
        free((void *)arg);
    }
    _tokens_allocated_by_ParseString.clear();
}