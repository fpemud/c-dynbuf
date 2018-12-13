#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "c-dynbuf.h"

void show(CDynBuf *b) {
	size_t i;

	printf("len: %lu", b->len);

	printf("\t");

	printf("ptr: '");
	if (b->len > 0) {
		for (i = 0; i < b->len; i++) {
			printf("%c", b->p[i]);
		}
	}
	printf("'\n");
}

int main(void) {
	CDynBuf *dynbuf;
	int r;

	// create a dynbuf that expands itself 10 by 10 octets when needed.
	r = c_dynbuf_new(&dynbuf);
	if (r != 0) {
		fprintf(stderr, "c_dynbuf_new failed.\n");
		return 1;
	}
	show(dynbuf);

	// append 3 octets from "stream" to the end of dynbuf
	c_dynbuf_append(dynbuf, "stream", 3);
	show(dynbuf);

	// append a full string to the end of dynbuf
	c_dynbuf_append(dynbuf, "ing", strlen("ing"));
	show(dynbuf);

	// insert "foo" at the start of the buffer
	c_dynbuf_insert(dynbuf, 0, "foo", strlen("foo"));
	show(dynbuf);

	// remove 4 chars at potition 2
	c_dynbuf_remove(dynbuf, 2, 4);
	show(dynbuf);

	// append 3 octets
	c_dynbuf_append_c(dynbuf, 'a', 3);
	show(dynbuf);

	// insert 3 octets
	c_dynbuf_insert_c(dynbuf, 0, 'b', 3);
	show(dynbuf);

	// write 3 octets
	c_dynbuf_write_c(dynbuf, 3, 'c', 5);
	show(dynbuf);

	// free the dynbuf
	c_dynbuf_free(dynbuf);

	return 0;
}
