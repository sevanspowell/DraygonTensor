namespace ds_com
{
template <size_t N>
size_t GetArraySize(char (&a)[N])
{
    return N;
}
}
