export module math;

export double fact(int n)
{
    return n == 0 ? 1 : n * fact(n-1);
}

