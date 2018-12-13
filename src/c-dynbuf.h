#pragma once

/**
 * Dynamic Buffer Implementation in Standard ISO-C11
 * This library provides a dynamic memory buffer abstraction
 * which provides appending, insertion and partial/total removal
 * without worrying about memory management.
 */

#ifdef __cpluscplus
extern "C" {
#endif

typedef struct CDynBuf CDynBuf;

/**
 * struct CDynBuf - Dynamic buffer object
 * @p:
 * @len:
 */
struct CDynBuf {
	unsigned char	*p;
	size_t			len;
	size_t			_size;		/* inner data */
};

#define C_DYNBUF_INIT(_buf) { .p = NULL, ._size = 0, .len = 0 }

int c_dynbuf_new(CDynBuf **bufp);
void c_dynbuf_free(CDynBuf *buf);
void c_dynbuf_init(CDynBuf *buf);
void c_dynbuf_deinit(CDynBuf *buf);

int c_dynbuf_append(CDynBuf *buf, const void *data, size_t len);
int c_dynbuf_insert(CDynBuf *buf, off_t pos, const void *data, size_t len);
int c_dynbuf_write(CDynBuf *buf, off_t pos, const void *data, size_t len);

int c_dynbuf_append_c(CDynBuf *buf, int c, size_t n);
int c_dynbuf_insert_c(CDynBuf *buf, off_t pos, int c, size_t n);
int c_dynbuf_write_c(CDynBuf *buf, off_t pos, int c, size_t n);

void c_dynbuf_remove(CDynBuf *buf, off_t pos, size_t len);
void c_dynbuf_clear(CDynBuf *buf);

#ifdef __cpluscplus
}
#endif