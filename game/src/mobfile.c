#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "ceio.h"
#include "cestr.h"
#include "byteorder.h"
#include "mobfile.h"

enum {
	MOB_SIGNATURE = 0xa000
};

static const uint64_t MAIN_BLOCK_QUEST_SIGNATURE = 0x80000d000;
static const uint64_t MAIN_BLOCK_ZONAL_SIGNATURE = 0x80000c000;

static const uint32_t TEXT_BLOCK_SIGNATURE = 0xacceeccb;
//static const uint64_t OBJECT_BLOCK_SIGNATURE = 0xb000;

/*static const uint64_t OBJECT_BLOCK_UNIT_SIGNATURE = 0xbbbb0000;
static const uint64_t OBJECT_BLOCK_OBJECT_SIGNATURE = 0xb001;
static const uint64_t OBJECT_BLOCK_LEVER_SIGNATURE = 0xbbac0000;
static const uint64_t OBJECT_BLOCK_TRAP_SIGNATURE = 0xbbab0000;
static const uint64_t OBJECT_BLOCK_FLAME_SIGNATURE = 0xbbbf;
static const uint64_t OBJECT_BLOCK_PARTICLE1_SIGNATURE = 0xaa01;
static const uint64_t OBJECT_BLOCK_PARTICLE2_SIGNATURE = 0xcc01;
static const uint64_t OBJECT_BLOCK_PARTICLE3_SIGNATURE = 0xdd01;*/

struct mobfile {
	io_callbacks callbacks;
	void* client_data;
};

static void decrypt_script(char* buf, uint32_t key, int32_t size)
{
	for (int i = 0; i < size; ++i) {
		key += (((((key * 13) << 4) + key) << 8) - key) * 4 + 2531011;
		buf[i] ^= key >> 16;
	}
}

static mobfile* open_callbacks(io_callbacks callbacks,
								void* client_data, const char* name)
{
	name = name;

	mobfile* mob = calloc(1, sizeof(mobfile));
	if (NULL == mob) {
		return NULL;
	}

	uint32_t signature;
	if (1 != (callbacks.read)(&signature, sizeof(uint32_t), 1, client_data)) {
		mobfile_close(mob);
		return NULL;
	}

	le2cpu32s(&signature);
	if (MOB_SIGNATURE != signature) {
		mobfile_close(mob);
		return NULL;
	}

	int32_t main_block_length;
	if (1 != (callbacks.read)(&main_block_length, sizeof(int32_t), 1, client_data)) {
		mobfile_close(mob);
		return NULL;
	}

	le2cpu32s((uint32_t*)&main_block_length);
	printf("main_block_length: %d\n", main_block_length);

	uint64_t main_block_type;
	if (1 != (callbacks.read)(&main_block_type, sizeof(uint64_t), 1, client_data)) {
		mobfile_close(mob);
		return NULL;
	}

	le2cpu64s(&main_block_type);
	printf("main_block_type: %#llx\n", main_block_type);
	if (MAIN_BLOCK_QUEST_SIGNATURE != main_block_type &&
			MAIN_BLOCK_ZONAL_SIGNATURE != main_block_type) {
		mobfile_close(mob);
		return NULL;
	}

	do {
		uint32_t signature;
		if (1 != (callbacks.read)(&signature, sizeof(uint32_t), 1, client_data)) {
			mobfile_close(mob);
			return NULL;
		}

		le2cpu32s(&signature);
		printf("signature: %u %#x\n", signature, signature);

		int32_t length;
		if (1 != (callbacks.read)(&length, sizeof(int32_t), 1, client_data)) {
			mobfile_close(mob);
			return NULL;
		}

		le2cpu32s((uint32_t*)&length);
		printf("length: %d\n", length);

		if (TEXT_BLOCK_SIGNATURE == signature) {
			int32_t script_key;
			(callbacks.read)(&script_key, sizeof(int32_t), 1, client_data);

			le2cpu32s((uint32_t*)&script_key);
			printf("script_key: %d\n", script_key);

			char* data = malloc(length - 4 - 4 - 4);
			(callbacks.read)(data, 1, length - 4 - 4 - 4, client_data);
			decrypt_script(data, script_key, length - 4 - 4 - 4);
			for (int i = 0; i < length - 4 - 4 - 4; ++i) {
				printf("%c", data[i]);
			}
			printf("\n");
			free(data);
		} else {
			char* data = malloc(length - 4 - 4);
			(callbacks.read)(data, 1, length - 4 - 4, client_data);
			free(data);
		}
	} while(main_block_length != (callbacks.tell)(client_data));

	mob->callbacks = callbacks;
	mob->client_data = client_data;

	return mob;
}

mobfile* mobfile_open(const char* path)
{
	FILE* file = fopen(path, "rb");
	if (NULL == file) {
		return NULL;
	}

	const char* name = strrpbrk(path, "\\/");
	if (NULL == name) {
		name = path;
	} else {
		++name;
	}

	mobfile* mob = open_callbacks(IO_CALLBACKS_FILE, file, name);
	if (NULL == mob) {
		fclose(file);
		return NULL;
	}

	return mob;
}

int mobfile_close(mobfile* mob)
{
	if (NULL == mob) {
		return 0;
	}

	if (NULL != mob->callbacks.close && NULL != mob->client_data) {
		(mob->callbacks.close)(mob->client_data);
	}

	free(mob);

	return 0;
}

void mobfile_debug_print(mobfile* mob)
{
	mob = mob;
}