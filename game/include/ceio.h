#ifndef CE_CEIO_H
#define CE_CEIO_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef struct {
	int (*close)(void* client_data);
	size_t (*read)(void* data, size_t size, size_t n, void* client_data);
	size_t (*write)(const void* data, size_t size, size_t n, void* client_data);
	int (*seek)(long int offset, int whence, void* client_data);
	long int (*tell)(void* client_data);
} io_callbacks;

extern const io_callbacks IO_CALLBACKS_FILE;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CE_CEIO_H */