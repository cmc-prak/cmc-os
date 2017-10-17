#pragma once

struct RandomState;

struct RandomOps
{
    void (*destroy)(struct RandomState *st);
    double (*nextDouble)(struct RandomState *st);
};

struct RandomState
{
    const struct RandomOps *ops;
};

//struct RandomState *new_prng_gen(const char *init_data);
