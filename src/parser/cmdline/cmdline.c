#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#include "raycli/extension/rayutils/diagnostics/log.h"
#include "raycli/parser/cmdline/cmdline.h"

RAY_STACK_IMPLEMENTATION(RayCLIOptRule, RayCLIOptRule, raycli_optrule, NULL)

void
raycli_cmdline_destroy(RayCLICmdline *const cmdline)
{
	assert(cmdline != NULL);
	if (cmdline->options != NULL) raycli_token_stack_free(&cmdline->options);
	if (cmdline->arguments != NULL) raycli_token_stack_free(&cmdline->arguments);
}

RayBool
raycli_cmdline_update_options(RayCLITokenStack *const options, RayCLIOptRuleStack *const rules, RayU64 *result)
{
	assert(options != NULL);
	assert(rules != NULL);
	assert(result != NULL);
	
	for (size_t i = 0; i < options->count; ++i)
	{
		const RayCLIToken *current_option = &options->elements[i];
		bool handled = false;

		for (size_t j = 0; j < rules->count; ++j)
		{
			const RayCLIOptRule *rule = &rules->elements[j];

			const bool matches =
				(current_option->kind == RAYCLI_TOKEN_KIND_SHORT_OPTION) ?
				(current_option->text[0] == rule->short_variant) :
				(strcmp(current_option->text, rule->long_variant) == 0);

			*result |= (RayU64) (j + 1) * matches;

			if (matches)
			{
				handled = true;
			}
		}

		if (!handled)
		{
			RAYCLI_LOG_CMDLINE(
				RAY_LOG_METADATA_OPTION_ERROR,
				RAY_LOG_METADATA_CODE(RAYCLI_LOG_METADATA_CODE_SYS_CMDLINE, RAYCLI_LOG_METADATA_CODE_CAT_CMDLINE_IO, RAYCLI_LOG_METADATA_CODE_REA_CMDLINE_INSARGV),
				stderr,
				current_option->span.column,
				"Unknown option `%s'.\n",
				current_option->text
			);

			return false;
		}
	}

	return true;
}
