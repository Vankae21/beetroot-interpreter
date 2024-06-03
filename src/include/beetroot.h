#ifndef BEETROOT_H
#define BEETROOT_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

extern bool had_error;

void run_file(const char* path);
void run_prompt();
void run(const char* source);
void error(int line, const char* message);
void report(int line, const char* where, const char* message);

#endif