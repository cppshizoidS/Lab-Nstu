#include "my_lib.h"


size_t
replace(char *str, const char *old_pair, const char *new_pair)
{
	size_t replaces = 0;

	for (size_t ltr = 0; str[ltr + 1] != '\0'; ltr++)
		{
			if ((str[ltr] == old_pair[0]) && (str[ltr + 1] == old_pair[1]))
				{
					str[ltr]     = new_pair[0];
					str[ltr + 1] = new_pair[1];

					ltr++;
					replaces++;
				}
		}

	return replaces;
}
