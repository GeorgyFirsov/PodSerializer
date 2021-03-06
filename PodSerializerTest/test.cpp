#include "pch.h"

// 
// Two fields in structure
// 
struct TwoFields
{
    char field1;
    int  field2;
};
#define TwoFieldsCorrectAnswer 2

//
// Ten fields in structure
// 
struct TenFields
{
    char field1; int field2; int field3; double field4; short field5;
    char field6; int field7; int field8; double field9; short field10;
};
#define TenFieldsCorrectAnswer 10

//
// Struct with enum inside
// 
enum TestEnum1 : unsigned long long
{
    first1, second1
};

enum TestEnum2
{
    first2, second2
};

struct ThreeFieldsWithEnum
{
    char      field1;
    TestEnum1 field2;
    TestEnum2 field3; 
};
#define ThreeFieldsWithEnumCorrectAnswer 3

//
// Struct with nested struct inside
// 
struct Nested
{
    int field1;
    char field2;
};

bool operator==( const Nested& lhs, const Nested& rhs ) {
    return (lhs.field1 == rhs.field1) && (lhs.field2 == rhs.field2);
}

bool operator!=( const Nested& lhs, const Nested& rhs ) {
    return (lhs.field1 != rhs.field1) || (lhs.field2 != rhs.field2);
}

struct ThreeFieldsWithNestedStruct
{
    double field1;
    Nested field2;
    char   field3;
};
#define ThreeFieldsWithNestedStructCorrectAnswer 3

//
// Two levels of nested structs
// 
struct NestedWithNested
{
    char   field1;
    Nested field2;
};

bool operator==( const NestedWithNested& lhs, const NestedWithNested& rhs ) {
    return (lhs.field1 == rhs.field1) && (lhs.field2 == rhs.field2);
}

struct TwoFieldsTwoLevelsOfNestedStructs
{
    long long field1;
    NestedWithNested field2;
};
#define TwoFieldsTwoLevelsOfNestedStructsCorrectAnswer 2

//
// not-POD struct
// 
struct NotPod
{
    char field1;
    std::string field2;
    double field3;
};
#define NotPodCorrectAnswer 3


/************************************************************************************
 * Reflection tests
 */

TEST(GetFieldsCount, Correctness_ExplicitTemplate) 
{
    EXPECT_EQ(
        GetFieldsCount<TwoFields>(),
        TwoFieldsCorrectAnswer
    );

    EXPECT_EQ(
        GetFieldsCount<TenFields>(),
        TenFieldsCorrectAnswer
    );
}

TEST(GetFieldsCount, Correctness_ParameterPassing)
{
    TwoFields two_fields;
    TenFields ten_fields;

    EXPECT_EQ(
        GetFieldsCount( two_fields ),
        TwoFieldsCorrectAnswer
    );

    EXPECT_EQ(
        GetFieldsCount( ten_fields ),
        TenFieldsCorrectAnswer
    );
}

TEST(GetFieldsCount, ContainsEnum)
{
    ThreeFieldsWithEnum three_fields;

    EXPECT_EQ(
        GetFieldsCount( three_fields ),
        ThreeFieldsWithEnumCorrectAnswer
    );
}

TEST(GetFieldsCount, ContainsNested)
{
    ThreeFieldsWithNestedStruct three_fields;

    EXPECT_EQ(
        GetFieldsCount( three_fields ),
        ThreeFieldsWithNestedStructCorrectAnswer
    );
}

TEST(GetFieldsCount, TwoLevelsOfNested)
{
    TwoFieldsTwoLevelsOfNestedStructs two_fields;

    EXPECT_EQ(
        GetFieldsCount( two_fields ),
        TwoFieldsTwoLevelsOfNestedStructsCorrectAnswer
    );
}

TEST(GetTypeIds, Correctness)
{
    constexpr auto ids = GetTypeIds<TwoFields>();

    EXPECT_EQ( ids.Size(),  2  );

    EXPECT_EQ( ids.data[0], 11 );
    EXPECT_EQ( ids.data[1], 8  );
}

TEST(GetTypeIds, ContainsEnum)
{
    constexpr auto ids = GetTypeIds<ThreeFieldsWithEnum>();

    EXPECT_EQ( ids.Size(),   3 );

    EXPECT_EQ( ids.data[0], 11 );
    EXPECT_EQ( ids.data[1],  5 );
    EXPECT_EQ( ids.data[2],  8 );
}

TEST(GetTypeIds, NestedStruct)
{
    constexpr auto ids = GetTypeIds<ThreeFieldsWithNestedStruct>();

    EXPECT_EQ( ids.Size(), 4 );

    EXPECT_EQ( ids.data[0], 16 );
    EXPECT_EQ( ids.data[1],  8 );
    EXPECT_EQ( ids.data[2], 11 );
    EXPECT_EQ( ids.data[3], 11 );
}

TEST(GetTypeIds, TwoLevelsOfNested)
{
    constexpr auto ids = GetTypeIds<TwoFieldsTwoLevelsOfNestedStructs>();

    EXPECT_EQ( ids.Size(), 4 );

    EXPECT_EQ( ids.data[0], 10 );
    EXPECT_EQ( ids.data[1], 11 );
    EXPECT_EQ( ids.data[2],  8 );
    EXPECT_EQ( ids.data[3], 11 );
}

TEST(ToTuple, Correctness)
{
    TwoFields two_fields{ 'a', 4 };

    auto two_tpl = ToTuple( two_fields );

    EXPECT_EQ( types::get<0>( two_tpl ), 'a' );
    EXPECT_EQ( types::get<1>( two_tpl ), 4   );

    TenFields ten_fields{ 'a', 25, 4, 3.14, 0, 'b', 54, 32, 2.71, 9 };

    auto ten_tpl = ToTuple( ten_fields );

    EXPECT_EQ( types::get<0>( ten_tpl ), 'a'  );
    EXPECT_EQ( types::get<1>( ten_tpl ), 25   );
    EXPECT_EQ( types::get<2>( ten_tpl ), 4    );
    EXPECT_EQ( types::get<3>( ten_tpl ), 3.14 );
    EXPECT_EQ( types::get<4>( ten_tpl ), 0    );
    EXPECT_EQ( types::get<5>( ten_tpl ), 'b'  );
    EXPECT_EQ( types::get<6>( ten_tpl ), 54   );
    EXPECT_EQ( types::get<7>( ten_tpl ), 32   );
    EXPECT_EQ( types::get<8>( ten_tpl ), 2.71 );
    EXPECT_EQ( types::get<9>( ten_tpl ), 9    );
}

TEST(ToTuple, ContainsEnum)
{
    ThreeFieldsWithEnum three_fields = { 'a', first1, second2 };

    auto three_tpl = ToTuple( three_fields );

    EXPECT_EQ( types::get<0>( three_tpl ), 'a'     );
    EXPECT_EQ( types::get<1>( three_tpl ), first1  );
    EXPECT_EQ( types::get<2>( three_tpl ), second2 );
}

TEST(ToTuple, NestedStruct)
{
    ThreeFieldsWithNestedStruct three_fields{ 3.14, { 10, 'a' }, 'b' };

    auto three_tpl = ToTuple( three_fields );

    EXPECT_EQ( three_tpl.Size(), 4 );

    EXPECT_EQ( types::get<0>( three_tpl ), 3.14 );
    EXPECT_EQ( types::get<1>( three_tpl ), 10   );
    EXPECT_EQ( types::get<2>( three_tpl ), 'a'  );
    EXPECT_EQ( types::get<3>( three_tpl ), 'b'  );
}

TEST(ToTuple, TwoLevelsOfNested)
{
    TwoFieldsTwoLevelsOfNestedStructs three_fields{ 56, { 'a', { 8, 'b' } } };

    auto three_tpl = ToTuple( three_fields );

    EXPECT_EQ( three_tpl.Size(), 4 );

    EXPECT_EQ( types::get<0>( three_tpl ), 56  );
    EXPECT_EQ( types::get<1>( three_tpl ), 'a' );
    EXPECT_EQ( types::get<2>( three_tpl ), 8   );
    EXPECT_EQ( types::get<3>( three_tpl ), 'b' );
}

TEST(ToStandardTuple, Correctness)
{
    TwoFields two_fields{ 'a', 4 };

    auto two_tpl = ToStandardTuple( two_fields );

    EXPECT_EQ( std::get<0>( two_tpl ), 'a' );
    EXPECT_EQ( std::get<1>( two_tpl ), 4   );

    TenFields ten_fields{ 'a', 25, 4, 3.14, 0, 'b', 54, 32, 2.71, 9 };

    auto ten_tpl = ToStandardTuple( ten_fields );

    EXPECT_EQ( std::get<0>( ten_tpl ), 'a'  );
    EXPECT_EQ( std::get<1>( ten_tpl ), 25   );
    EXPECT_EQ( std::get<2>( ten_tpl ), 4    );
    EXPECT_EQ( std::get<3>( ten_tpl ), 3.14 );
    EXPECT_EQ( std::get<4>( ten_tpl ), 0    );
    EXPECT_EQ( std::get<5>( ten_tpl ), 'b'  );
    EXPECT_EQ( std::get<6>( ten_tpl ), 54   );
    EXPECT_EQ( std::get<7>( ten_tpl ), 32   );
    EXPECT_EQ( std::get<8>( ten_tpl ), 2.71 );
    EXPECT_EQ( std::get<9>( ten_tpl ), 9    );
}

TEST(ToStandardTuple, ContainsEnum)
{
    ThreeFieldsWithEnum three_fields = { 'a', first1, second2 };

    auto three_tpl = ToStandardTuple( three_fields );

    EXPECT_EQ( std::get<0>( three_tpl ), 'a'     );
    EXPECT_EQ( std::get<1>( three_tpl ), first1  );
    EXPECT_EQ( std::get<2>( three_tpl ), second2 );
}

TEST(ToStandardTuple, NestedStruct)
{
    ThreeFieldsWithNestedStruct three_fields{ 3.14, { 10, 'a' }, 'b' };

    auto three_tpl = ToStandardTuple( three_fields );

    EXPECT_EQ( std::get<0>( three_tpl ), 3.14 );
    EXPECT_EQ( std::get<1>( three_tpl ), 10   );
    EXPECT_EQ( std::get<2>( three_tpl ), 'a'  );
    EXPECT_EQ( std::get<3>( three_tpl ), 'b'  );
}

TEST(ToStandardTuple, TwoLevelsOfNested)
{
    TwoFieldsTwoLevelsOfNestedStructs three_fields{ 56, { 'a', { 8, 'b' } } };

    auto three_tpl = ToStandardTuple( three_fields );

    EXPECT_EQ( std::get<0>( three_tpl ), 56  );
    EXPECT_EQ( std::get<1>( three_tpl ), 'a' );
    EXPECT_EQ( std::get<2>( three_tpl ), 8   );
    EXPECT_EQ( std::get<3>( three_tpl ), 'b' );
}

TEST(ToTuplePrecise, TwoFields)
{
    TwoFields two_fields{ 'a', 42 };

    auto two_tpl = ToTuplePrecise( two_fields );

    EXPECT_EQ( types::get<0>( two_tpl ), 'a' );
    EXPECT_EQ( types::get<1>( two_tpl ), 42  );
}

TEST(ToTuplePrecise, ThreeFieldsWithEnum)
{
    ThreeFieldsWithEnum three_fields{ 'a', first1, second2 };

    auto three_tpl = ToTuplePrecise( three_fields );

    EXPECT_EQ( types::get<0>( three_tpl ), 'a'     );
    EXPECT_EQ( types::get<1>( three_tpl ), first1  );
    EXPECT_EQ( types::get<2>( three_tpl ), second2 );
}

TEST(ToTuplePrecise, ThreeFieldsWithNestedStruct)
{
    ThreeFieldsWithNestedStruct three_fields{ 3.14, { 42, 'a' }, 'b' };
    Nested expected_second{ 42, 'a' };

    auto three_tpl = ToTuplePrecise( three_fields );

    EXPECT_EQ( types::get<0>( three_tpl ), 3.14            );
    EXPECT_EQ( types::get<1>( three_tpl ), expected_second );
    EXPECT_EQ( types::get<2>( three_tpl ), 'b'             );
}

TEST(ToTuplePrecise, TwoFieldsTwoLevelsOfNestedStructs)
{
    TwoFieldsTwoLevelsOfNestedStructs two_fields{ 42, { 'a', { -5, 'b' } } };
    NestedWithNested expected_second{ 'a', { -5, 'b' } };

    auto two_tpl = ToTuplePrecise( two_fields );

    EXPECT_EQ( types::get<0>( two_tpl ), 42              );
    EXPECT_EQ( types::get<1>( two_tpl ), expected_second );
}

TEST(ToTuplePrecise, NotPod)
{
    NotPod three_fields{ 'a', "String inside", 3.14 };

    auto three_tpl = ToTuplePrecise( three_fields );

    EXPECT_EQ( types::get<0>( three_tpl ), 'a'             );
    EXPECT_EQ( types::get<1>( three_tpl ), "String inside" );
    EXPECT_EQ( types::get<2>( three_tpl ), 3.14            );
}

TEST(ToStandardTuplePrecise, TwoFields)
{
    TwoFields two_fields{ 'a', 42 };

    auto two_tpl = ToStandardTuplePrecise( two_fields );

    EXPECT_EQ( std::get<0>( two_tpl ), 'a' );
    EXPECT_EQ( std::get<1>( two_tpl ), 42  );
}

TEST(ToStandardTuplePrecise, ThreeFieldsWithEnum)
{
    ThreeFieldsWithEnum three_fields{ 'a', first1, second2 };

    auto three_tpl = ToStandardTuplePrecise( three_fields );

    EXPECT_EQ( std::get<0>( three_tpl ), 'a'     );
    EXPECT_EQ( std::get<1>( three_tpl ), first1  );
    EXPECT_EQ( std::get<2>( three_tpl ), second2 );
}

TEST(ToStandardTuplePrecise, ThreeFieldsWithNestedStruct)
{
    ThreeFieldsWithNestedStruct three_fields{ 3.14, { 42, 'a' }, 'b' };
    Nested expected_second{ 42, 'a' };

    auto three_tpl = ToStandardTuplePrecise( three_fields );

    EXPECT_EQ( std::get<0>( three_tpl ), 3.14            );
    EXPECT_EQ( std::get<1>( three_tpl ), expected_second );
    EXPECT_EQ( std::get<2>( three_tpl ), 'b'             );
}

TEST(ToStandardTuplePrecise, TwoFieldsTwoLevelsOfNestedStructs)
{
    TwoFieldsTwoLevelsOfNestedStructs two_fields{ 42, { 'a', { -5, 'b' } } };
    NestedWithNested expected_second{ 'a', { -5, 'b' } };

    auto two_tpl = ToStandardTuplePrecise( two_fields );

    EXPECT_EQ( std::get<0>( two_tpl ), 42              );
    EXPECT_EQ( std::get<1>( two_tpl ), expected_second );
}

TEST(ToStandardTuplePrecise, NotPod)
{
    NotPod three_fields{ 'a', "String inside", 3.14 };

    auto three_tpl = ToStandardTuplePrecise( three_fields );

    EXPECT_EQ( std::get<0>( three_tpl ), 'a'             );
    EXPECT_EQ( std::get<1>( three_tpl ), "String inside" );
    EXPECT_EQ( std::get<2>( three_tpl ), 3.14            );
}

 
/************************************************************************************
 * Visual stream operators tests
 */

TEST(Operators, ostream)
{
    using namespace io_operators;
    
    std::cout << "It is a visual test.\n" << std::endl;

    TenFields ten_fields{ 'a', 25, 4, 3.14, 0, 'b', 54, 32, 2.71, 9 };

    std::cout << ten_fields << std::endl;
    std::cout << beautiful_struct << ten_fields << no_beautiful_struct << std::endl;
}

TEST(Operators, wostream)
{
    using namespace io_operators;
    
    std::wcout << L"It is a visual test.\n" << std::endl;

    TenFields ten_fields{ 'a', 25, 4, 3.14, 0, 'b', 54, 32, 2.71, 9 };

    std::wcout << ten_fields << std::endl;
    std::wcout << beautiful_struct << ten_fields << no_beautiful_struct << std::endl;
}

TEST(Operators, ostreamNotPod)
{
    using namespace io_operators;
    
    std::cout << "It is a visual test.\n" << std::endl;

    NotPod three_fields{ 'a', "String to print", 3.14 };

    std::cout << three_fields << std::endl;
    std::cout << beautiful_struct << three_fields << no_beautiful_struct << std::endl;
}

TEST(Operators, ostreamNested)
{
    using namespace io_operators;
    
    std::cout << "It is a visual test.\n" << std::endl;

    ThreeFieldsWithNestedStruct three_fields{ 3.14, { 42, 'a' }, 'b' };

    std::cout << three_fields << std::endl;
    std::cout << beautiful_struct << three_fields << no_beautiful_struct << std::endl;
}


/************************************************************************************
 * Serialization tests
 */

TEST(Serialization, Binary)
{
    TwoFields original{ 2, 4 };

    BinarySerializer<TwoFields> serializer;
    BinaryBuffer<TwoFields> buffer;

    EXPECT_TRUE( buffer.IsEmpty() );

    serializer.Serialize( original, buffer );

    EXPECT_FALSE( buffer.IsEmpty() );

    TwoFields loaded{ 0, 0 };

    EXPECT_NE( loaded.field1, original.field1 );
    EXPECT_NE( loaded.field2, original.field2 );

    serializer.Deserialize( loaded, buffer );

    EXPECT_EQ( loaded.field1, original.field1 );
    EXPECT_EQ( loaded.field2, original.field2 );
}
 
TEST(Serialization, BinaryContainsEnum)
{
    ThreeFieldsWithEnum original{ 'a', first1, second2 };

    BinarySerializer<ThreeFieldsWithEnum> serializer;
    BinaryBuffer<ThreeFieldsWithEnum> buffer;

    EXPECT_TRUE( buffer.IsEmpty() );

    serializer.Serialize( original, buffer );

    EXPECT_FALSE( buffer.IsEmpty() );

    ThreeFieldsWithEnum loaded{ 'b', second1, first2 };

    EXPECT_NE( loaded.field1, original.field1 );
    EXPECT_NE( loaded.field2, original.field2 );
    EXPECT_NE( loaded.field3, original.field3 );

    serializer.Deserialize( loaded, buffer );

    EXPECT_EQ( loaded.field1, original.field1 );
    EXPECT_EQ( loaded.field2, original.field2 );
    EXPECT_EQ( loaded.field3, original.field3 );
}

TEST(Serialization, StringStream)
{
    TwoFields original{ 2, 4 };

    StringStreamSerializer<TwoFields> serializer;
    StringStreamBuffer<TwoFields> buffer;

    EXPECT_TRUE( buffer.IsEmpty() );

    serializer.Serialize( original, buffer );

    EXPECT_FALSE( buffer.IsEmpty() );

    TwoFields loaded{ 0, 0 };

    EXPECT_NE( loaded.field1, original.field1 );
    EXPECT_NE( loaded.field2, original.field2 );

    serializer.Deserialize( loaded, buffer );

    EXPECT_EQ( loaded.field1, original.field1 );
    EXPECT_EQ( loaded.field2, original.field2 );
}

TEST(Serialization, NestedStruct)
{
    ThreeFieldsWithNestedStruct original{ 3.14, { 42, 'a' }, 'b' };
    
    StringStreamSerializer<ThreeFieldsWithNestedStruct> serializer;
    StringStreamBuffer<ThreeFieldsWithNestedStruct> buffer;

    EXPECT_TRUE( buffer.IsEmpty() );

    serializer.Serialize( original, buffer );

    EXPECT_FALSE( buffer.IsEmpty() );

    ThreeFieldsWithNestedStruct loaded{ 2.71, { -5, 'c' }, 'd' };

    EXPECT_NE( loaded.field1, original.field1 );
    EXPECT_NE( loaded.field2, original.field2 );

    serializer.Deserialize( loaded, buffer );

    EXPECT_EQ( loaded.field1, original.field1 );
    EXPECT_EQ( loaded.field2, original.field2 );
}

TEST(Serialization, StringStreamContainsEnum)
{
    ThreeFieldsWithEnum original{ 'a', first1, second2 };

    StringStreamSerializer<ThreeFieldsWithEnum> serializer;
    StringStreamBuffer<ThreeFieldsWithEnum> buffer;

    EXPECT_TRUE( buffer.IsEmpty() );

    serializer.Serialize( original, buffer );

    EXPECT_FALSE( buffer.IsEmpty() );

    ThreeFieldsWithEnum loaded{ 'b', second1, first2 };

    EXPECT_NE( loaded.field1, original.field1 );
    EXPECT_NE( loaded.field2, original.field2 );
    EXPECT_NE( loaded.field3, original.field3 );

    serializer.Deserialize( loaded, buffer );

    EXPECT_EQ( loaded.field1, original.field1 );
    EXPECT_EQ( loaded.field2, original.field2 );
    EXPECT_EQ( loaded.field3, original.field3 );
}

TEST(Serialization, StringStreamNotPod)
{
    NotPod original{ 'a', "Serialized\n string", 3.14 };

    StringStreamSerializer<NotPod> serializer;
    StringStreamBuffer<NotPod> buffer;

    EXPECT_TRUE( buffer.IsEmpty() );

    serializer.Serialize( original, buffer );

    EXPECT_FALSE( buffer.IsEmpty() );

    NotPod loaded{ 'b', "Another string", 2.71 };

    EXPECT_NE( loaded.field1, original.field1 );
    EXPECT_NE( loaded.field2, original.field2 );
    EXPECT_NE( loaded.field3, original.field3 );

    serializer.Deserialize( loaded, buffer );

    EXPECT_EQ( loaded.field1, original.field1 );
    EXPECT_EQ( loaded.field2, original.field2 );
    EXPECT_EQ( loaded.field3, original.field3 );
}


/************************************************************************************
 * Typelist tests
 */

TEST(TypeList, Size)
{
    TypeList<double, int, std::string, short> tl;

    EXPECT_EQ( Size( tl ), 4 );

    EmptyTypeList empty;

    EXPECT_EQ( Size( empty ), 0 );
}

TEST(TypeList, TupleType)
{
    TypeList<double, int, std::string, short> tl;
    using ExpectedType = Tuple<double, int, std::string, short>;

    EXPECT_EQ( 
        Identity<decltype( TupleType( tl ) )>{}, 
        Identity<ExpectedType>{} 
    );
}

TEST(TypeList, Apply)
{
    TypeList<double, int, std::string, short> tl;
    using ExpectedType = TypeList<double&, int&, std::string&, short&>;
    using ActualType = decltype( Apply<std::add_lvalue_reference>( tl ) );

    EXPECT_EQ( 
        Identity<ActualType>{}, 
        Identity<ExpectedType>{} 
    );
}


/************************************************************************************
 * Tuple tests
 */

TEST(Tuple, ToStdTuple)
{
    Tuple<int, char, double, std::string> tpl{ 42, 'a', 3.14, std::string("Hello") };
    using ExpectedType = std::tuple<int, char, double, std::string>;

    auto stdTpl = ToStdTuple( tpl );

    EXPECT_EQ( Identity<decltype( stdTpl )>{}, Identity<ExpectedType>{} );
         
    EXPECT_EQ( std::get<0>( stdTpl ), types::get<0>( tpl ) );
    EXPECT_EQ( std::get<1>( stdTpl ), types::get<1>( tpl ) );
    EXPECT_EQ( std::get<2>( stdTpl ), types::get<2>( tpl ) );
    EXPECT_EQ( std::get<3>( stdTpl ), types::get<3>( tpl ) );
}


/************************************************************************************
 * GetTypeList tests
 */

TEST(GetTypeList, Size)
{
    constexpr auto tl1 = GetTypeList<NotPod>();
    EXPECT_EQ( Size( tl1 ), NotPodCorrectAnswer );

    constexpr auto tl2 = GetTypeList<TenFields>();
    EXPECT_EQ( Size( tl2 ), TenFieldsCorrectAnswer );

    constexpr auto tl3 = GetTypeList<ThreeFieldsWithEnum>();
    EXPECT_EQ( Size( tl3 ), ThreeFieldsWithEnumCorrectAnswer );

    constexpr auto tl4 = GetTypeList<ThreeFieldsWithNestedStruct>();
    EXPECT_EQ( Size( tl4 ), ThreeFieldsWithNestedStructCorrectAnswer );

    constexpr auto tl5 = GetTypeList<TwoFieldsTwoLevelsOfNestedStructs>();
    EXPECT_EQ( Size( tl5 ), TwoFieldsTwoLevelsOfNestedStructsCorrectAnswer );
}

TEST(GetTypeList, SizeExplicit)
{
    constexpr TenFields ten_fields{};
    constexpr auto tl1 = GetTypeList( ten_fields );
    EXPECT_EQ( Size( tl1 ), TenFieldsCorrectAnswer );

    constexpr ThreeFieldsWithEnum three_fields_enum{};
    constexpr auto tl2 = GetTypeList( three_fields_enum );
    EXPECT_EQ( Size( tl2 ), ThreeFieldsWithEnumCorrectAnswer );

    constexpr ThreeFieldsWithNestedStruct three_fields_nested{};
    constexpr auto tl3 = GetTypeList( three_fields_nested );
    EXPECT_EQ( Size( tl3 ), ThreeFieldsWithNestedStructCorrectAnswer );

    constexpr TwoFieldsTwoLevelsOfNestedStructs two_fields_nested{};
    constexpr auto tl4 = GetTypeList( two_fields_nested );
    EXPECT_EQ( Size( tl4 ), TwoFieldsTwoLevelsOfNestedStructsCorrectAnswer );
}

TEST(GetTypeList, Correctness)
{
    constexpr auto tl1 = GetTypeList<TwoFields>();

    EXPECT_EQ( type_list::get<0>( tl1 ), Identity<char>{} );
    EXPECT_EQ( type_list::get<1>( tl1 ), Identity<int>{} );

    constexpr auto tl2 = GetTypeList<ThreeFieldsWithNestedStruct>();

    EXPECT_EQ( type_list::get<0>( tl2 ), Identity<double>{} );
    EXPECT_EQ( type_list::get<1>( tl2 ), Identity<Nested>{} );
    EXPECT_EQ( type_list::get<2>( tl2 ), Identity<char>{} );
}

TEST(GetTypeList, NotPod)
{
    constexpr auto tl = GetTypeList<NotPod>();

    EXPECT_EQ( type_list::get<0>( tl ), Identity<char>{} );
    EXPECT_EQ( type_list::get<1>( tl ), Identity<std::string>{} );
    EXPECT_EQ( type_list::get<2>( tl ), Identity<double>{} );
}
