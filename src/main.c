#include <stdio.h>

#include "include/beetroot.h"


int main(int argc, char** argv) {
	if (argc > 2) {
		puts("Usage: beetroot {filename}");
		had_error = true;
	} else if (argc == 2) {
		run_file(argv[1]);
	} else {
		run_prompt();
	}
	return had_error;
}