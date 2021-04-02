#include <stddef.h>
#include <stdint.h>
#include "stivale2.h"

namespace stivale {
	void *get_tag(stivale2_struct *stivale, uint64_t id) {
		stivale2_tag *current_tag = (stivale2_tag*)stivale->tags;
		for (;;) {
			if (current_tag == NULL) {
				return NULL;
			}

			if (current_tag->identifier == id) {
				return current_tag;
			}

			current_tag = (stivale2_tag*)current_tag->next;
		}
	}
};