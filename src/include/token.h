#ifndef TOKEN_H
#define TOKEN_H

#include <stdint.h>
typedef enum {
	// Single char tokens
	LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
	COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

	// One or two char tokens
	BANG, BANG_EQUAL,
	EQUAL, EQUAL_EQUAL,
	GREATER, GREATER_EQUAL,
	LESS, LESS_EQUAL,

	// Literals
	IDENTIFIER, STRING, NUMBER,

	// Keywords
	AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
	PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

	EOF_TOKEN
} type_token;

typedef struct {
	type_token type;
	char* lexeme;
	uint16_t line;
} Token;

Token* init_token(int type, char* lexeme, uint16_t line);
char* token_type_name(type_token type);

#endif