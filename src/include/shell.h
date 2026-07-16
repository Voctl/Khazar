#pragma once

struct shell_command {
	const char *name;
	void (*handler)(char *args);
};

