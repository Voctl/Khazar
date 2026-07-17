#pragma once

#include "typint.h"

struct shell_command {
	const char *name;
	void (*handler)(char *args);
};

U0 shell(U0);
