#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"
#include <stdbool.h>

typedef struct {
	const char* source;
	Token** tokens;
	int token_size;
	int start;
	int current;
	int line;
} Scanner;

Scanner* init_scanner(const char* source);
void scan_tokens(Scanner* scanner);
void scan_token(Scanner* scanner);
bool is_at_end(Scanner* scanner);
char advance(Scanner* scanner);
void add_token(Scanner* scanner, Token* token);
bool match(Scanner* scanner, char expected);
char peek(Scanner* scanner);
char peek_next(Scanner* scanner);
void get_string(Scanner* scanner);
void get_number(Scanner* scanner);
void get_identifier(Scanner* scanner);
bool is_alpha(const char c);
bool is_alphanum(const char c);

#endif