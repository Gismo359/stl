int Main()
{
    std::Vector<int> v;
    v.reserve(4);

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);

    int sum = 0;
    v.reverse::iter(int test)
    {
        sum += test;
    }
    then::reverse::iter(int test)
    {
        sum += test;
    }
    then::iter(int test)
    {
        sum += test;
    }
    then::iter(int test)
    {
        sum += test;
    };

    char test_1[200];
    to_chars(&test_1[0], &test_1[200], 17.29f);

    char test_2[200];
    to_chars(&test_2[0], &test_2[200], 17.29f, 10);

    char test_3[200];
    to_chars(&test_3[0], &test_3[200], 17.29);

    char test_4[200];
    to_chars(&test_4[0], &test_4[200], 17.29, 10);

    return test_1[0] + test_2[0] + test_3[0] + test_4[0];
}