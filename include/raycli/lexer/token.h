/*
 * Copyright (C) 2026 Rayleigh Gamma
 * See LICENSE for licensing information.
 */

#ifndef RAYCLI_LEXER_TOKEN_H
#define RAYCLI_LEXER_TOKEN_H

#include <rayutils/diagnostics/span.h>

typedef enum {
	RAYCLI_TOKEN_KIND_IDENTIFIER,
	RAYCLI_TOKEN_KIND_SHORT_OPTION,
	RAYCLI_TOKEN_KIND_LONG_OPTION,
	RAYCLI_TOKEN_KIND_END_OF_LINE,
} RayCLITokenKind;

typedef struct {
	RayCLITokenKind kind;
	char *text;
	RaySpan span;
} RayCLIToken;

#endif /* RAYCLI_LEXER_TOKEN_H */
