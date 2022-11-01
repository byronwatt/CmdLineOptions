#include "cmd_line_options.h"
#include <inttypes.h>

static BoolOption option_some_bool(false,"some_bool","testing bool option");

class SomeEnumOption: public EnumOption {
public:
    SomeEnumOption( uint32_t default_value, const char *_name, const char *_usage_message ) : EnumOption(default_value,_name,_usage_message) {
        AddEnum(0,"zero","often enums are used to select which test to run");
        AddEnum(1,"one","and you have blurbs for what the tests do");
        AddEnum(2,"two","you get the idea");
        AddEnum(3,"three","each enum");
        AddEnum(4,"four","has a usage message");
        SetFromEnvironmentVariable();
    }
} ;

static SomeEnumOption option_some_enum(0,"some_enum","testing some_enum");

static IntOption option_some_int(0,"some_int","testing some_int");

static UintOption option_some_uint(0,"some_uint","testing some_uint");

static Int64Option option_some_int64(0,"some_int64","testing some_int64");

static Uint64Option option_some_uint64(0,"some_uint64","testing some_uint64");

static IntRangeOption option_some_intrange("some_intrange","testing some_intrange");

static IntListOption option_some_intList("some_intList:","testing some_intList");

static StringListOption option_some_stringlist("some_stringlist:","testing some_stringlist");

// quite a mouthful,... same as string list, but stop if you see another valid option
static OptionFreeStringListOption option_optionfreestringlist("optionfreestringlist:","testing optionfreestringlist (valid option terminates list)");

static DoubleOption option_some_double(0,"some_double","testing some_double");

static StringOption option_some_string("default","some_string","testing some_string");

void option_test()
{
    if (option_some_bool.is_set)
    {
        printf("option_some_bool.is_set\n");
        printf("option_some_bool.value = %s\n",option_some_bool.value?"true":"false");
    }
    if (option_some_enum.is_set)
    {
        printf("option_some_enum.is_set\n");
        printf("option_some_enum.value = %d (\"%s\")\n",
               option_some_enum.value,
               option_some_enum.GetString(option_some_enum.value));
    }
    if (option_some_int.is_set)
    {
        printf("option_some_int.is_set\n");
        printf("option_some_int.value = %d\n",option_some_int.value);
    }
    if (option_some_uint.is_set)
    {
        printf("option_some_uint.is_set\n");
        printf("option_some_uint.value = %u (0x%x)\n",option_some_uint.value,option_some_uint.value);
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
        printf("option_some_intrange.start_value = %d\n",option_some_intrange.start_value);
        printf("option_some_intrange.end_value = %d\n",option_some_intrange.end_value);
    }
    if (option_some_intList.is_set)
    {
        printf("option_some_intList.is_set\n");
        printf("option_some_intList:");
        for (std::vector<int32_t>::const_iterator it = option_some_intList.value_list_.begin(); it != option_some_intList.value_list_.end(); ++it) {
            printf(" %d",*it);
        }
        printf("\n");
    }
    if (option_some_stringlist.is_set)
    {
        printf("option_some_stringlist.is_set\n");
        printf("option_some_stringlist:");
        for (std::vector<const char *>::const_iterator it = option_some_stringlist.string_list_.begin(); it != option_some_stringlist.string_list_.end(); ++it) {
            printf(" %s",*it);
        }
        printf("\n");
    }
    if (option_optionfreestringlist.is_set)
    {
        printf("option_optionfreestringlist.is_set\n");
        printf("option_optionfreestringlist:");
        for (std::vector<const char *>::const_iterator it = option_optionfreestringlist.string_list_.begin(); it != option_optionfreestringlist.string_list_.end(); ++it) {
            printf(" %s",*it);
        }
        printf("\n");
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
