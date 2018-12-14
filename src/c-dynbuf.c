/*
 * Dynamic Buffer Implementation
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include "c-dynbuf.h"

#define _public_ __attribute__((__visibility__("default")))

#define BUFSZ_INIT			((size_t)32)
#define BUFSZ_THRESHOLD		((size_t)10 * 1024 * 1024)
#define BUFSZ_INCREMENT		((size_t)1 * 1024 * 1024)

static int _expand_buf_to(CDynBuf *buf, int datasz) {
	int newsz;
	unsigned char *newp;

	newsz = buf->_size;
	if (newsz < BUFSZ_INIT)
		newsz = BUFSZ_INIT;
	while (newsz < datasz) {
		if (newsz < BUFSZ_THRESHOLD)
			newsz *= 2;
		else
			newsz += BUFSZ_INCREMENT;
	}

	if (newsz > buf->_size) {
		newp = malloc(newsz);
		if (newp == NULL)
			return -ENOMEM;

		if (buf->p != NULL) {
			memcpy(newp, buf->p, buf->len);
			free(buf->p);
		}

		buf->p = newp;
		buf->_size = newsz;
	}

	return 0;
}

static void _inbuf_memcpy(CDynBuf *buf, off_t dst_pos, off_t src_pos, size_t len) {
	size_t i;
	for (i = 0; i < len; i++) {
		buf->p[dst_pos + i] = buf->p[src_pos + i];
	}
}

static void _inbuf_memcpy_reverse(CDynBuf *buf, off_t dst_pos, off_t src_pos, size_t len) {
	if (len > 0) {
		size_t i = len - 1;
		while (1) {
			buf->p[dst_pos + i] = buf->p[src_pos + i];
			if (i == 0)
				break;
			i--;
		}
	}
}


/**
 * c_dynbuf_init() - initialize dynamic buffer object
 * @buf:                  dynamic buffer object to operate on
 * 
 * This initializes a dynamic buffer object that the caller allocated. This is 
 * equivalent to assigning C_DYNBUF_INIT to the object.
 */
_public_ void c_dynbuf_init(CDynBuf *buf) {
	assert(buf != NULL);

	*buf = (CDynBuf)C_DYNBUF_INIT(*buf);			/* fixme */
}

/**
 * c_dynbuf_deinit() - deinitialize dynamic buffer object
 * @buf:                  dynamic buffer object to operate on
 * 
 */
_public_ void c_dynbuf_deinit(CDynBuf *buf) {
	assert(buf != NULL);

	if (buf->p != NULL)
		free(buf->p);

	c_dynbuf_init(buf);
}

/**
 * c_dynbuf_new() - XXX
 *
 * Returns 0.
 */
_public_ int c_dynbuf_new(CDynBuf **bufp) {
	CDynBuf *buf = NULL;

	assert(bufp != NULL);

	buf = malloc(sizeof(*buf));
	if (buf == NULL)
		return -ENOMEM;

	c_dynbuf_init(buf);

	*bufp = buf;
	buf = NULL;
	return 0;
}

/**
 * c_dynbuf_free() - XXX
 */
_public_ void c_dynbuf_free(CDynBuf *buf) {
	if (buf == NULL)
		return;

	c_dynbuf_deinit(buf);
	free(buf);
}

/**
 * c_dynbuf_append() - Append `len` bytes from `data` to dynamic buffer
 * @data:
 * @len:
 *
 * Returns 0 or -1.
 */
_public_ int c_dynbuf_append(CDynBuf *buf, const void *data, size_t len) {
	int r;

	assert(buf != NULL);
	assert(data != NULL);

	r = _expand_buf_to(buf, buf->len + len);
	if (r < 0)
		return r;

	memcpy(buf->p + buf->len, data, len);
	buf->len += len;

	return 0;
}

/**
 * c_dynbuf_append_c() - XXX
 * @buf:                  dynamic buffer object to operate on
 * @c:
 * @n:
 */
_public_ int c_dynbuf_append_c(CDynBuf *buf, int c, size_t n) {
	int r;

	assert(buf != NULL);

	r = _expand_buf_to(buf, buf->len + n);
	if (r < 0)
		return r;

	memset(buf->p + buf->len, c, n);
	buf->len += n;

	return 0;
}

/**
 * c_dynbuf_insert() - Insert `len` bytes from `data` to dynamic buffer, starting at `pos`
 * @pos:
 * @data:
 * @len:
 *
 * Returns 0 or -1.
 */
_public_ int c_dynbuf_insert(CDynBuf *buf, off_t pos, const void *data, size_t len) {
	int r;

	assert(buf != NULL);
	assert(pos >= 0 && pos <= buf->len);
	assert(data != NULL);

	if (len == 0)
		return 0;

	r = _expand_buf_to(buf, buf->len + len);
	if (r < 0)
		return r;

	_inbuf_memcpy_reverse(buf, pos + len, pos, buf->len - pos);

	memcpy(buf->p + pos, data, len);
	buf->len += len;

	return 0;
}

/**
 * c_dynbuf_insert_c() - XXX
 * @buf:                  dynamic buffer object to operate on
 * @pos:
 * @c:
 * @n:
 */
_public_ int c_dynbuf_insert_c(CDynBuf *buf, off_t pos, int c, size_t n) {
	int r;

	assert(buf != NULL);
	assert(pos >= 0 && pos <= buf->len);

	if (n == 0)
		return 0;

	r = _expand_buf_to(buf, buf->len + n);
	if (r < 0)
		return r;

	_inbuf_memcpy_reverse(buf, pos + n, pos, buf->len - pos);

	memset(buf->p + pos, c, n);
	buf->len += n;

	return 0;
}

/**
 * c_dynbuf_remove() - Remove `len` bytes from dynamic buffer, starting at `pos`
 * @buf:                  dynamic buffer object to operate on
 * @pos:
 * @len:
 */
_public_ void c_dynbuf_remove(CDynBuf *buf, off_t pos, size_t len) {
	assert(buf != NULL);
	assert(pos >= 0 && pos <= buf->len && pos + len <= buf->len);

	if (len == 0)
		return;

	_inbuf_memcpy(buf, pos, pos + len, buf->len - (pos + len));

	buf->len -= len;
	return;
}

/**
 * c_dynbuf_write() - Write `len` bytes from `data` to dynamic buffer, starting at `pos`
 * @pos:
 * @data:
 * @len:
 *
 * Returns 0 or -1.
 */
_public_ int c_dynbuf_write(CDynBuf *buf, off_t pos, const void *data, size_t len) {
	int r;

	assert(buf != NULL);
	assert(pos >= 0 && pos <= buf->len);
	assert(data != NULL);

	r = _expand_buf_to(buf, pos + len);
	if (r < 0)
		return r;

	memcpy(buf->p + pos, data, len);
	if (pos + len > buf->len)
		buf->len = pos + len;

	return 0;
}

/**
 * c_dynbuf_write_c() - XXX
 * @buf:                  dynamic buffer object to operate on
 * @pos:
 * @c:
 * @n:
 */
_public_ int c_dynbuf_write_c(CDynBuf *buf, off_t pos, int c, size_t n) {
	int r;

	assert(buf != NULL);
	assert(pos >= 0 && pos <= buf->len);

	r = _expand_buf_to(buf, pos + n);
	if (r < 0)
		return r;

	memset(buf->p + pos, c, n);
	if (pos + n > buf->len)
		buf->len = pos + n;

	return 0;
}

/**
 * c_dynbuf_expand() - XXXX
 * @buf:                  dynamic buffer object to operate on
 * @len:
 */
_public_ int c_dynbuf_expand(CDynBuf *buf, size_t len) {
	int r;

	assert(buf != NULL);

	r = _expand_buf_to(buf, buf->len + len);
	if (r < 0)
		return r;

	buf->len += len;
	return 0;
}

/**
 * c_dynbuf_shrink() - XXXX
 * @buf:                  dynamic buffer object to operate on
 * @len:
 */
_public_ void c_dynbuf_shrink(CDynBuf *buf, size_t len) {
	assert(buf != NULL);
	assert(len <= buf->len);

	buf->len -= len;
	return;
}

/**
 * c_dynbuf_clear() - Remove all data from dynamic buffer, allowing overwrite without realocation.
 * @buf:                  dynamic buffer object to operate on
 */
_public_ void c_dynbuf_clear(CDynBuf *buf) {
	assert(buf != NULL);

	buf->len = 0;
	return;
}
