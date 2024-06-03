#include "include/scanner.h"
#include "include/token.h"
#include "include/beetroot.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Scanner* init_scanner(const char* source) {
	Scanner* scanner = calloc(1, sizeof(Scanner));
	scanner->source = source;
	scanner->start = 0;
	scanner->current = 0;
	scanner->line = 1;
	scanner->token_size = 0;
	// tokens null for now
	return scanner;
}

void scan_tokens(Scanner* scanner) {
	while (!is_at_end(scanner)) {
		scanner->start = scanner->current;
		
		scan_token(scanner);
	}

	add_token(scanner, init_token(EOF_TOKEN, "", scanner->line));
}

void scan_token(Scanner* scanner) {
	char c = advance(scanner);
	switch (c) {
		case '(': add_token(scanner, init_token(LEFT_PAREN, "", scanner->line)); break;
		case ')': add_token(scanner, init_token(RIGHT_PAREN, "", scanner->line)); break;
		case '{': add_token(scanner, init_token(LEFT_BRACE, "", scanner->line)); break;
		case '}': add_token(scanner, init_token(RIGHT_BRACE, "", scanner->line)); break;
		case ',': add_token(scanner, init_token(COMMA, "", scanner->line)); break;
		case '.': add_token(scanner, init_token(DOT, "", scanner->line)); break;
		case '-': add_token(scanner, init_token(MINUS, "", scanner->line)); break;
		case '+': add_token(scanner, init_token(PLUS, "", scanner->line)); break;
		case ';': add_token(scanner, init_token(SEMICOLON, "", scanner->line)); break;
		case '*': add_token(scanner, init_token(STAR, "", scanner->line)); break;
		case '!':
			add_token(scanner, init_token(match(scanner, '=') ? BANG_EQUAL : BANG, "", scanner->line));
			break;
		case '=':
			add_token(scanner, init_token(match(scanner, '=') ? EQUAL_EQUAL : EQUAL, "", scanner->line));
			break;
		case '<':
			add_token(scanner, init_token(match(scanner, '=') ? LESS_EQUAL : LESS, "", scanner->line));
			break;
		case '>':
			add_token(scanner, init_token(match(scanner, '=') ? GREATER_EQUAL : GREATER, "", scanner->line));
			break;
		case '/':
			if (match(scanner, '/')) {
				while (peek(scanner) != '\n' && !is_at_end(scanner)) {
					advance(scanner);
				}
				scanner->line++;
			} else if (match(scanner, '*')) {
				while (peek(scanner) != '*' && !is_at_end(scanner)) {
					advance(scanner);
					if (peek(scanner) == '\n') scanner->line++;
				}
				// skip '*' char
				advance(scanner);
				if (peek(scanner) != '/') {
					error(scanner->line, "Unclosed comment block.");
					had_error = true;
					break;
				}
				// skip '/' char
				advance(scanner);
			} else {
				add_token(scanner, init_token(SLASH, "", scanner->line));
			}
			break;
		
		case ' ':
		case '\t':
		case '\r':
			break;

		case '\n':
			scanner->line++;
			break;

		case '"':
			get_string(scanner);
			break;

		default:
			if (isdigit(c)) {
				get_number(scanner);
			} else if (is_alpha(c)) {
				get_identifier(scanner);
			} else {
				error(scanner->line, "Unexpected token."); had_error = true;
			}
			break;
	}
}

bool is_at_end(Scanner* scanner) {
	return scanner->current >= strlen(scanner->source);
}

char advance(Scanner* scanner) {
	scanner->current++;
	return scanner->source[scanner->current - 1];
}

void add_token(Scanner* scanner, Token* token) {
	scanner->token_size++;
	scanner->tokens = realloc(scanner->tokens, scanner->token_size * sizeof(Token*));
	scanner->tokens[scanner->token_size - 1] = token;
}

bool match(Scanner* scanner, char expected) {
	if (is_at_end(scanner))
		return false;
	if (scanner->source[scanner->current] != expected)
		return false;

	scanner->current++;
	return true;
}

char peek(Scanner* scanner) {
	if (is_at_end(scanner)) return '\0';
	return scanner->source[scanner->current];
}

char peek_next(Scanner* scanner) {
	if (scanner->current + 1 >= strlen(scanner->source)) return '\0';
	return scanner->source[scanner->current + 1];
}

void get_string(Scanner* scanner) {
	uint16_t s_len = 0;
	char* s = 0;
	while (peek(scanner) != '"' && !is_at_end(scanner)) {
		if (peek(scanner) != '\n') scanner->line++;
		s_len++;
		s = realloc(s, s_len + 1);
		s[s_len - 1] = advance(scanner);
		s[s_len] = '\0';
	}

	if (is_at_end(scanner)) {
		had_error = true;
		error(scanner->line, "Unterminated string.");
		free(s);
		return;
	}

	// closing "
	advance(scanner);
	add_token(scanner, init_token(STRING, s, scanner->line));
}

void get_number(Scanner* scanner) {

	uint16_t s_len = 1;
	char* s = calloc(s_len + 1, 1);
	s[0] = scanner->source[scanner->current - 1];
	s[1] = '\0';
	while (isdigit(peek(scanner))) {
		s_len++;
		s = realloc(s, s_len + 1);
		s[s_len - 1] = advance(scanner);
		s[s_len] = '\0';
	}
	
	if (peek(scanner) == '.' && isdigit(peek_next(scanner))) {
		s_len++;
		s = realloc(s, s_len + 1);
		s[s_len - 1] = advance(scanner);
		s[s_len] = '\0';
		while (isdigit(peek(scanner))) {
			s_len++;
			s = realloc(s, s_len + 1);
			s[s_len - 1] = advance(scanner);
			s[s_len] = '\0';
		}
	}

	add_token(scanner, init_token(NUMBER, s, scanner->line));
}

void get_identifier(Scanner* scanner) {

	uint16_t s_len = 1;
	char* s = calloc(s_len + 1, 1);
	s[0] = scanner->source[scanner->current - 1];
	s[1] = '\0';
	while (is_alphanum(peek(scanner))) {
		s_len++;
		s = realloc(s, s_len + 1);
		s[s_len - 1] = advance(scanner);
		s[s_len] = '\0';
	}

	type_token type = IDENTIFIER;
	// all strcomps go here //
	if (strcmp(s, "and") == 0) type = AND;
	else if (strcmp(s, "class") == 0) type = CLASS;
	else if (strcmp(s, "else") == 0) type = ELSE;
	else if (strcmp(s, "false") == 0) type = FALSE;
	else if (strcmp(s, "for") == 0) type = FOR;
	else if (strcmp(s, "fun") == 0) type = FUN;
	else if (strcmp(s, "if") == 0) type = IF;
	else if (strcmp(s, "nil") == 0) type = NIL;
	else if (strcmp(s, "or") == 0) type = OR;
	else if (strcmp(s, "print") == 0) type = PRINT;
	else if (strcmp(s, "return") == 0) type = RETURN;
	else if (strcmp(s, "super") == 0) type = SUPER;
	else if (strcmp(s, "this") == 0) type = THIS;
	else if (strcmp(s, "true") == 0) type = TRUE;
	else if (strcmp(s, "var") == 0) type = VAR;
	else if (strcmp(s, "while") == 0) type = WHILE;

	add_token(scanner, init_token(type, s, scanner->line));
}

bool is_alpha(const char c) {
	return ( c >= 'a' && c <= 'z') ||
		   ( c >= 'A' && c <= 'Z') ||
		   (c == '_');
}

bool is_alphanum(const char c) {
	return (c >= 'a' && c <= 'z') ||
		   (c >= 'A' && c <= 'Z') ||
		   (c >= '0' && c <= '9') ||
		   (c == '_');
}