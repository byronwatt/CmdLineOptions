#include "cmd_line_options.h"
#include <inttypes.h>

static BoolOption option_some_Bool(false,"some_bool","testing bool option");

static AliasOption option_some_alias("some_alias","some_bool=true some_enum=two some_int=3 some_double=4");

class SomeEnumOption: public EnumOption {
public:
    SomeEnumOption( uint32_t default_value, const char *_name, const char *_usage_message ) : EnumOption(default_value,_name,_usage_message) {
        AddEnum(0,"zero");
        AddEnum(1,"one");
        AddEnum(2,"two");
        AddEnum(3,"three");
        AddEnum(4,"four");
        SetFromEnvironmentVariable();
    }
} ;

static SomeEnumOption option_some_enum(0,"some_enum","testing some_enum");

//static EnumPairOption option_some_enumPair("some_enumPair","testing some_enumPair");

static IntOption option_some_int(0,"some_int","testing some_int");

static UintOption option_some_uint(0,"some_uint","testing some_uint");

static Int64Option option_some_int64(0,"some_int64","testing some_int64");

static Uint64Option option_some_uint64(0,"some_uint64","testing some_uint64");

static IntRangeOption option_some_intrange("some_intrange","testing some_intrange");

static IntListOption option_some_intList("some_intList:","testing some_intList");

static StringListOption option_some_stringlist("some_stringlist:","testing some_stringlist");

static DoubleOption option_some_double(0,"some_double","testing some_double");

static StringOption option_some_string("default","some_string","testing some_string");

void option_test()
{
    if (option_some_Bool.is_set)
    {
        printf("option_some_Bool.is_set\n");
        printf("option_some_Bool.value = %s\n",option_some_Bool.value?"true":"false");
    }
    if (option_some_alias.is_set)
    {
        printf("option_some_alias.is_set\n");
    }
    if (option_some_enum.is_set)
    {
        printf("option_some_enum.is_set\n");
        printf("option_some_enum.value = %d\n",option_some_enum.value);
    }
    //if (option_some_enumPair.is_set)
    //{
    //    printf("option_some_enumPair.is_set\n");
    //}
    if (option_some_int.is_set)
    {
        printf("option_some_int.is_set\n");
        printf("option_some_int.value = %d\n",option_some_int.value);
    }
    if (option_some_uint.is_set)
    {
        printf("option_some_uint.is_set\n");
        printf("option_some_uint.value = %u (%x)\n",option_some_uint.value,option_some_uint.value);
    }
    if (option_some_int64.is_set)
    {
        printf("option_some_int64.is_set\n");
        printf("option_some_int64.value = %" PRId64 " (0x%" PRIx64 ")\n",option_some_int64.value,option_some_int64.value);
    }
    if (option_some_uint64.is_set)
    {
        printf("option_some_uint64.is_set\n");
        printf("option_some_uint64.value = %" PRIu64 " (0x%" PRIx64 ")\n",option_some_uint64.value,option_some_uint64.value);
    }
    if (option_some_intrange.is_set)
    {
        printf("option_some_intrange.is_set\n");
        //printf("option_some_intrange.value = \n",option_some_intrange.value);
    }
    if (option_some_intList.is_set)
    {
        printf("option_some_intList.is_set\n");
        //printf("option_some_intList.value = \n",option_some_intList.value);
    }
    if (option_some_stringlist.is_set)
    {
        printf("option_some_stringlist.is_set\n");
        //printf("option_some_stringlist.value = \n",option_some_stringlist.value);
    }
    if (option_some_double.is_set)
    {
        printf("option_some_double.is_set\n");
        printf("option_some_double.value = %g\n",option_some_double.value);
    }
    if (option_some_string.is_set)
    {
        printf("option_some_string.is_set\n");
        printf("option_some_string.value = \"%s\"\n",option_some_string.value);
    }
}
