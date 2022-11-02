//  COPYRIGHT (C) 2022 Microchip with MIT license

/**
 * @file
 * @brief
 *   This file parses command line options.
 */

#include <ostream>
#include <stdint.h>
#include <vector>

extern "C" void cmd_line_options_parse_options(int argc, const char **argv);

/**
 * @brief
 *  structure defined a register address and mask
 *  used by AddrMaskOption & AddrMaskListOption
 */
typedef struct
{
    uint32_t addr; ///< register address
    uint32_t mask; ///< field mask
} addr_and_mask_t;

/**
 * @brief class used for parsing command line options
 */
class CmdLineOption
{
  public:
    CmdLineOption(const char *_name, const char *_usage_message);
    virtual ~CmdLineOption(){};
    /// pure virtual function
    virtual bool ParseValue(const char *s) = 0;
    virtual bool ParseValueWithError(const char *s, std::ostream &error_message);
    void SetFromEnvironmentVariable();
    virtual void EndOfList();
    virtual void Reset();
    virtual void OptionSet();
    const char *name;          ///< name of the option
    const char *usage_message; ///< usage message for the option
    bool is_set;               ///< was the option set on the command line
    bool is_list;              ///< does the option take a list of parameters.
    bool is_option_free_list;  ///< should the list terminate if a token looks like a command line option.
    bool is_bool;              ///< is this option a 'bool' option which does not need an '='
};

/**
 * @brief
 *  Boolean command line options
 */
class BoolOption : public CmdLineOption
{
  public:
    BoolOption(bool default_value, const char *_name, const char *_usage_message);
    virtual bool ParseValue(const char *s);
    virtual bool ParseValueWithError(const char *s, std::ostream &error_message);
    virtual void Reset();
    bool value;          ///< boolean value
    bool _default_value; ///< boolean value
};

/**
 * @brief
 *   structure used to define string -> value enumerations.
 */
typedef struct
{
    uint32_t value;            ///< value of an enumeration
    const char *str;           ///< string of an enumeration
    const char *usage_message; ///< usage message for this enumeration.
} value_str_t;

/**
 * @brief
 *   an integer command line option with string equivalents.
 */
class EnumOption : public CmdLineOption
{
  public:
    EnumOption(uint32_t default_value, const char *_name, const char *_usage_message);
    virtual bool ParseValue(const char *s);
    virtual bool ParseValueWithError(const char *s, std::ostream &error_message);
    const char *GetString(uint32_t value);
    void AddEnum(uint32_t value, const char *str, const char *usage_message = "");
    virtual void Reset();
    uint32_t value;                      ///< integer value
    uint32_t _default_value;             ///< integer value
    std::vector<value_str_t> enum_list_; ///< list of string value pairs
};

/**
 * @brief
 *  signed integer command line option
 */
class IntOption : public CmdLineOption
{
  public:
    IntOption(int32_t default_value, const char *_name, const char *_usage_message);
    virtual bool ParseValue(const char *s);
    virtual bool ParseValueWithError(const char *s, std::ostream &error_message);
    virtual void Reset();
    int32_t value;          ///< signed integer value
    int32_t _default_value; ///< signed integer value
};

/**
 * @brief
 *  unsigned integer command line option
 */
class UintOption : public CmdLineOption
{
  public:
    UintOption(uint32_t default_value, const char *_name, const char *_usage_message);
    virtual bool ParseValue(const char *s);
    virtual bool ParseValueWithError(const char *s, std::ostream &error_message);
    virtual void Reset();
    uint32_t value;          ///< unsigned integer value
    uint32_t _default_value; ///< unsigned integer value
};

/**
 * @brief
 *  signed 64 bit integer command line option
 */
class Int64Option : public CmdLineOption
{
  public:
    Int64Option(int64_t default_value, const char *_name, const char *_usage_message);
    virtual bool ParseValue(const char *s);
    virtual bool ParseValueWithError(const char *s, std::ostream &error_message);
    virtual void Reset();
    int64_t value;          ///< signed 64 bit integer value
    int64_t _default_value; ///< signed 64 bit integer value
};

/**
 * @brief
 *  unsigned signed 64 bit integer command line option
 */
class Uint64Option : public CmdLineOption
{
  public:
    Uint64Option(uint64_t default_value, const char *_name, const char *_usage_message);
    virtual bool ParseValue(const char *s);
    virtual bool ParseValueWithError(const char *s, std::ostream &error_message);
    virtual void Reset();
    uint64_t value;          ///< unsigned 64 bit integer value
    uint64_t _default_value; ///< unsigned 64 bit integer value
};

/**
 * @brief
 *  integer range command line option
 */
class IntRangeOption : public CmdLineOption
{
  public:
    IntRangeOption(const char *_name, const char *_usage_message);
    virtual bool ParseValue(const char *s);
    virtual bool ParseValueWithError(const char *s, std::ostream &error_message);
    virtual void Reset();
    int32_t start_value; ///< start of a range
    int32_t end_value;   ///< end of a range
    int32_t size;        ///< size of the range
};

/**
 * @brief
 *   list of integers command line option
 */
class IntListOption : public CmdLineOption
{
  public:
    IntListOption(const char *_name, const char *_usage_message, uint32_t _default_step = 1);
    virtual bool ParseValue(const char *s);
    virtual bool ParseValueWithError(const char *s, std::ostream &error_message);
    virtual void AddValue(int32_t value);
    virtual void Reset();
    virtual void EndOfList();
    std::vector<int32_t> value_list_;       ///< list of values
    std::vector<const char *> string_list_; ///< list of strings
    int32_t default_step;                   ///< step size
    uint32_t mask; ///< if the value list is a list of integers from 0..31,... mask is which bits are set.
};

/**
 * @brief
 *   list of integers command line option
 */
class StringListOption : public CmdLineOption
{
  public:
    StringListOption(const char *_name, const char *_usage_message);
    virtual bool ParseValue(const char *s);
    virtual void Reset();
    virtual void EndOfList();
    std::vector<const char *> string_list_; ///< list of strings
};

/**
 * @brief
 *   list of integers command line option, but list ends if you see another command line option.
 */
class OptionFreeStringListOption : public StringListOption
{
  public:
    OptionFreeStringListOption(const char *_name, const char *_usage_message);
};

/**
 * @brief
 *  double command line option
 */
class DoubleOption : public CmdLineOption
{
  public:
    DoubleOption(double default_value, const char *_name, const char *_usage_message);
    virtual bool ParseValue(const char *s);
    virtual bool ParseValueWithError(const char *s, std::ostream &error_message);
    virtual void Reset();
    double value;          ///< double command line value
    double _default_value; ///< double command line value
};

/**
 * @brief
 *  string command line option
 */
class StringOption : public CmdLineOption
{
  public:
    StringOption(const char *default_value, const char *_name, const char *_usage_message);
    virtual bool ParseValue(const char *s);
    virtual void Reset();
    const char *value;          ///< string command line option
    const char *_default_value; ///< string command line option
};

/**
 * @brief
 *   not a real command line option,.. just a way to add an extra line into the usage message to help delineate options.
 */
class OptionGroup : public CmdLineOption
{
  public:
    /// not a real command line option
    virtual bool ParseValue(const char *)
    {
        return false;
    }
    /// constructor
    OptionGroup(const char *_usage_message) : CmdLineOption("", _usage_message)
    {
    }
};

/**
 * @brief
 *   singleton integer range command line option
 */
class CmdLineOptions
{
  public:
    ~CmdLineOptions();
    /// singleton
    static CmdLineOptions *GetInstance();
    /// add an option to the list of valid command line options
    void AddOption(CmdLineOption *option);
    void Usage();
    void ShowUsage(std::ostream &error_message);
    void Reset();
    static void ParseOptions(int argc, const char **argv);
    bool ParseOptionsOrError(int argc, const char **argv, std::ostream &error_message);
    void ParseString(const char *argv_string);
    bool MatchesAnOption(const char *s);

  private:
    void ParseOptionsInternal(int argc, const char **argv);

    std::vector<const char *> _tokens_allocated_by_ParseString; ///< extra strings created by ParseString
    std::vector<CmdLineOption *> _option_list;                  ///< list of valid command line options
};

int32_t parse_int(const char *s, char **temp);
uint32_t parse_uint(const char *s, char **temp);
