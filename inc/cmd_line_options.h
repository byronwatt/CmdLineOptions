//  COPYRIGHT (C) Microchip with MIT license

/**
* @file
* @brief
*   This file parses command line options.
*/

#include <ostream>
#include <vector>

extern "C" void cmd_line_options_parse_options( int argc, const char **argv );

/**
 * @brief
 *  structure defined a register address and mask
 *  used by AddrMaskOption & AddrMaskListOption
 */
typedef struct {
    uint32_t addr; ///< register address
    uint32_t mask; ///< field mask
} addr_and_mask_t;

/**
 * @brief class used for parsing command line options
 */
class CmdLineOption {
public:
    CmdLineOption(const char *_name, const char *_usage_message);
    virtual ~CmdLineOption() {};
    /// pure virtual function
    virtual bool8_t ParseValue( const char *s ) = 0;
    virtual bool8_t ParseValueWithError(const char *s, std::ostream &error_message);
    void SetFromEnvironmentVariable();
    virtual void EndOfList();
    virtual void Reset();
    virtual void OptionSet();
    const char *name; ///< name of the option
    const char *usage_message; ///< usage message for the option
    bool8_t is_set; ///< was the option set on the command line
    bool8_t is_list; ///< does the option take a list of parameters.
    bool8_t is_option_free_list; ///< should the list terminate if a token looks like a command line option.
    bool8_t is_bool; ///< is this option a 'bool' option which does not need an '='
};

/**
 * @brief
 *  Boolean command line options
 */
class BoolOption: public CmdLineOption {
public:
    BoolOption( bool8_t default_value, const char *_name, const char *_usage_message ) ;
    virtual bool8_t ParseValue( const char *s );
    virtual bool8_t ParseValueWithError(const char *s, std::ostream &error_message);
    virtual void Reset();
    bool8_t value; ///< boolean value
    bool8_t _default_value; ///< boolean value
} ;

/**
 * @brief
 *   a command line parameter this is an alias for other command line options
 */
class AliasOption: public CmdLineOption {
public:
    virtual bool8_t ParseValue(const char *s);
    AliasOption(const char *_name,const char *_usage_message);
    std::vector<const char *> alias_argv; ///< equivalent list of parameters for this alias
};

/**
 * @brief
 *   structure used to define string -> value enumerations.
 */
typedef struct {
    uint32_t value; ///< value of an enumeration
    const char *str; ///< string of an enumeration
    const char *usage_message; ///< usage message for this enumeration.
} value_str_t;

/**
 * @brief
 *   an integer command line option with string equivalents.
 */
class EnumOption: public CmdLineOption {
public:
    EnumOption( uint32_t default_value, const char *_name, const char *_usage_message ) ;
    virtual bool8_t ParseValue( const char *s );
    virtual bool8_t ParseValueWithError(const char *s, std::ostream &error_message);
    const char *GetString( uint32_t value );
    void AddEnum(uint32_t value, const char *str, const char *usage_message="");
    virtual void Reset();
    uint32_t value; ///< integer value
    uint32_t _default_value; ///< integer value
    std::vector<value_str_t> enum_list_; ///< list of string value pairs
};


/**
* @brief
*   structure that defines a pair of values
*/
typedef struct {
    uint32_t first_value; ///< first value in a range
    uint32_t second_value; ///< second value in a range
    const char *str; ///< command line string?
    const char *usage_message; ///< usage message for the pair
} value_pair_str_t;

/**
 * @brief
 *   an integer pair command line option with string equivalents.
 */
class EnumPairOption: public CmdLineOption {
public:
    EnumPairOption( const char *_name, const char *_usage_message ) ;
    virtual bool8_t ParseValue( const char *s );
    virtual void Reset();
    void AddFirstEnum(uint32_t value, const char *str, const char *usage_message="");
    void AddPairEnum(uint32_t first_value, uint32_t second_value, const char *str, const char *usage_message="");

    uint32_t first_value; ///< first value in the pair
    uint32_t second_value; ///< second value in the pair
    std::vector<value_str_t> first_enum_list_; ///< enumeration for the first value
    std::vector<value_pair_str_t> pair_enum_list_; ///< enumeation for the second value
} ;

/**
 * @brief
 *   an enum option for the logging level.
 */
class LogLevelOption: public EnumOption {
public:
    LogLevelOption( uint32_t default_value, const char *_name, const char *_usage_message ) ;
    virtual void OptionSet();
};

/**
* @brief
*  signed integer command line option
*/
class IntOption: public CmdLineOption {
public:
    IntOption( int32_t default_value, const char *_name, const char *_usage_message ) ;
    virtual bool8_t ParseValue( const char *s );
    virtual bool8_t ParseValueWithError(const char *s, std::ostream &error_message);
    virtual void Reset();
    int32_t value; ///< signed integer value
    int32_t _default_value; ///< signed integer value
} ;

/**
* @brief
*  unsigned integer command line option
*/
class UintOption: public CmdLineOption {
public:
    UintOption( uint32_t default_value, const char *_name, const char *_usage_message ) ;
    virtual bool8_t ParseValue( const char *s );
    virtual bool8_t ParseValueWithError(const char *s, std::ostream &error_message);
    virtual void Reset();
    uint32_t value; ///< unsigned integer value
    uint32_t _default_value; ///< unsigned integer value
} ;

/**
* @brief
*  signed 64 bit integer command line option
*/
class Int64Option: public CmdLineOption {
public:
    Int64Option( int64_t default_value, const char *_name, const char *_usage_message ) ;
    virtual bool8_t ParseValue( const char *s );
    virtual bool8_t ParseValueWithError(const char *s, std::ostream &error_message);
    virtual void Reset();
    int64_t value; ///< signed 64 bit integer value
    int64_t _default_value; ///< signed 64 bit integer value
} ;

/**
* @brief
*  unsigned signed 64 bit integer command line option
*/
class Uint64Option: public CmdLineOption {
public:
    Uint64Option( uint64_t default_value, const char *_name, const char *_usage_message ) ;
    virtual bool8_t ParseValue( const char *s );
    virtual bool8_t ParseValueWithError(const char *s, std::ostream &error_message);
    virtual void Reset();
    uint64_t value; ///< unsigned 64 bit integer value
    uint64_t _default_value; ///< unsigned 64 bit integer value
} ;

/**
* @brief
*  integer range command line option
*/
class IntRangeOption: public CmdLineOption {
public:
    IntRangeOption( const char *_name, const char *_usage_message ) ;
    virtual bool8_t ParseValue( const char *s );
    virtual bool8_t ParseValueWithError(const char *s, std::ostream &error_message);
    virtual void Reset();
    int32_t start_value; ///< start of a range
    int32_t end_value; ///< end of a range
    int32_t size; ///< size of the range
} ;

/**
* @brief
*   list of integers command line option
*/
class IntListOption: public CmdLineOption {
public:
    IntListOption( const char *_name, const char *_usage_message, uint32_t _default_step=4 ) ;
    virtual bool8_t ParseValue( const char *s );
    virtual bool8_t ParseValueWithError(const char *s, std::ostream &error_message);
    virtual void AddValue( int32_t value );
    virtual void Reset();
    virtual void EndOfList();
    std::vector<int32_t> value_list_; ///< list of values
    std::vector<const char *> string_list_; ///< list of strings
    int32_t default_step; ///< step size
    uint32_t mask; ///< if the value list is a list of integers from 0..31,... mask is which bits are set.
} ;

/**
* @brief
*   list of integers command line option
*/
class StringListOption: public CmdLineOption {
public:
    StringListOption( const char *_name, const char *_usage_message ) ;
    virtual bool8_t ParseValue( const char *s );
    virtual void Reset();
    virtual void EndOfList();
    std::vector<const char *> string_list_; ///< list of strings
} ;

/**
* @brief
*   list of integers command line option, but list ends if you see another command line option.
*/
class OptionFreeStringListOption: public StringListOption {
public:
    OptionFreeStringListOption( const char *_name, const char *_usage_message ) ;
} ;

/**
* @brief
*   alias command line option
*/
class AliasListOption: public IntListOption {
public:
    AliasListOption(const char *_name,const char *_usage_message);
    virtual void EndOfList();
    std::vector<const char *> alias_argv; ///< equivalent command line option for the alias.
};

/**
* @brief
*  double command line option
*/
class DoubleOption: public CmdLineOption {
public:
    DoubleOption( double default_value, const char *_name, const char *_usage_message );
    virtual bool8_t ParseValue( const char *s );
    virtual bool8_t ParseValueWithError(const char *s, std::ostream &error_message);
    virtual void Reset();
    double value; ///< double command line value
    double _default_value; ///< double command line value
} ;

/**
* @brief
*  string command line option
*/
class StringOption: public CmdLineOption {
public:
    StringOption( const char *default_value, const char *_name, const char *_usage_message );
    virtual bool8_t ParseValue( const char *s );
    virtual void Reset();
    const char * value; ///< string command line option
    const char * _default_value; ///< string command line option
} ;

/**
* @brief
*  signed integer command line option
*/
class AddrMaskOption: public CmdLineOption {
public:
    AddrMaskOption( const char *_name, const char *_usage_message ) ;
    virtual bool8_t ParseValue( const char *s );
    virtual void Reset();
    addr_and_mask_t addr_and_mask; ///< address and mask
} ;

/**
* @brief
*   not a real command line option,.. just a way to add an extra line into the usage message to help delineate options.
*/
class OptionGroup: public CmdLineOption {
public:
    /// not a real command line option
    virtual bool8_t ParseValue(const char *s) {
        return FALSE;
    }
    /// constructor
    OptionGroup(const char *_usage_message) : CmdLineOption("",_usage_message) {
    }
};

/**
* @brief
*   singleton integer range command line option
*/
class CmdLineOptions {
public:
    ~CmdLineOptions();
    /// singleton
    static CmdLineOptions* GetInstance();
    /// add an option to the list of valid command line options
    void AddOption(CmdLineOption *option);
    void Usage();
    void ShowUsage(std::ostream &error_message);
    void Reset();
    void ParseOptions( int argc, const char **argv );
    bool8_t ParseOptionsOrError( int argc, const char **argv, std::ostream &error_message );
    void ParseString(const char *argv_string);
    bool8_t MatchesAnOption( const char *s );

    std::vector<const char*> tokens_allocated_by_ParseString; ///< extra strings created by ParseString
    std::vector<CmdLineOption*> option_list_; ///< list of valid command line options
};

int32_t parse_int( const char *s, char **temp);
uint32_t parse_uint( const char *s, char **temp);

/// verbosity command line option
extern LogLevelOption option_log_level;

