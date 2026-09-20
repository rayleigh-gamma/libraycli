/*
 * Copyright (C) 2026 Rayleigh Gamma
 * See LICENSE for licensing information.
 */

#ifndef RAYCLI_PARSER_PARSER_H
#define RAYCLI_PARSER_PARSER_H

#include "raycli/parser/cmdline/cmdline.h"

typedef struct {
	RayCLITokenStack *tokens;
	size_t index;
} RayCLIParser;

RayCLIParser raycli_parser_make(RayCLITokenStack *const tokens);
RayBool raycli_parser_parse(RayCLIParser *const parser, RayCLICmdline *cmdline);

#endif /* RAYCLI_PARSER_PARSER_H */
