#pragma once

struct RandomState;

struct RandomOps
{
    void (*destroy)(struct RandomState *);
    double (*next)(struct RandomState *);
};

struct RandomState
{
    const struct RandomOps *ops;
};

//struct RandomState *init_prng_gen(const char *);
//struct RandomState *init_trng_gen(const char *);
