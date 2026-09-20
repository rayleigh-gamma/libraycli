#ifndef RAYCLI_PARSER_CMDLINE_CMDLINE_H
#define RAYCLI_PARSER_CMDLINE_CMDLINE_H

#include "raycli/lexer/lexer.h"
#include "raycli/parser/cmdline/optrule.h"

RAY_STACK_DECLARATION(RayCLIOptRule, RayCLIOptRule, raycli_optrule)

typedef struct {
	RayCLITokenStack *general_options;
	RayCLIToken *command;
	RayCLITokenStack *options;
	RayCLITokenStack *arguments;
} RayCLICmdline;

void raycli_cmdline_destroy(RayCLICmdline *const cmdline);
RayBool raycli_cmdline_update_options(RayCLITokenStack *const options, RayCLIOptRuleStack *const rules, RayU64 *result);

#endif /* RAYCLI_PARSER_CMDLINE_CMDLINE_H */
