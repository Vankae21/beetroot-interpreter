#include "include/beetroot.h"
#include "include/scanner.h"

bool had_error = false;

void run_file(const char* path) {
	FILE* file = fopen(path, "r");
	if (file) {
		fseek(file, 0, SEEK_END);
		uint16_t len = ftell(file);
		char* content = calloc(len, sizeof(char));
		fseek(file, 0, SEEK_SET);
		if (content) {
			fread(content, sizeof(char), len, file);
		}
		fclose(file);
		run(content);
		free(content);
	} else {
		printf("Error: no file found whose name is %s\n", path);
		had_error = true;
	}
}

void run_prompt() {
	while (1) {
		printf(">> ");
		char line[100];
		if (!fgets(line, sizeof(line), stdin)) {
			printf("\n");
			break;
		}
		line[strlen(line) - 1] = '\0';
		run(line);
	}
}

void run(const char* source) {
	// Scan tokens
//	Scanner* scanner = init_scanner(source);
	// tokens = scantokens();
	// foreach tokens print token
	Scanner* scanner = init_scanner(source);
	scan_tokens(scanner);
	for (int i = 0; i < scanner->token_size; i++) {
		printf("type: %s, value: %s\n", token_type_name(scanner->tokens[i]->type), scanner->tokens[i]->lexeme);
	}
	free(scanner);
}

void error(int line, const char* message) {
	report(line, "", message);
}

void report(int line, const char* where, const char* message) {
	char buffer[100];
	sprintf(buffer, "[line %d] Error %s: %s", line, where, message);
	printf("%s\n", buffer);
	had_error = true;
}