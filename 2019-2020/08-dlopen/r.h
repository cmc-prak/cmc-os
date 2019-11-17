#pragma once

#define INTERFACE_VERSION 1

struct RandomContext;

struct RandomOperations
{
    void (*init)(struct RandomContext *);
    void (*destroy)(struct RandomContext *);
    int  (*rand_n)(struct RandomContext *, int n);
};

struct RandomContext
{
    const struct RandomOperations *ops;
};

struct RandomContext *create_stdlib_random();
struct RandomContext *create_urandom_random();

#define CREATE_FUNC_NAME(name,v) create_##name##_random_v##v
