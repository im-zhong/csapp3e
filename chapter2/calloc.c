// 2021/10/14
// zhangzhong
// 2.76

int tmult_ok(int x, int y)
{
    int p = x * y;
    return !x || p / x == y;
}

void *calloc(size_t nmemb, size_t size)
{
    if (!tmult_ok(nmemb, size))
        return NULL;
    
    void *memptr = malloc(nmemb * size);
    if (!memptr)
        return NULL;

    memset(memptr, 0, nmemb * size);
}
