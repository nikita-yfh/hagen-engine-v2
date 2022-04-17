#include "String.hpp"

// MurmurHash2
hash_t String::Hash(const char *data) {
	uint32_t len = strlen(data);

	const uint32_t seed = 0x57FC36BE;
	const uint32_t m = 0x5bd1e995;
	const int r = 24;
	hash_t h = seed ^ len;

	while(len >= 4) {
		uint32_t k = *(uint32_t *)data;

		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		data += 4;
		len -= 4;
	}

	switch(len) {
	case 3: h ^= data[2] << 16;
	case 2: h ^= data[1] << 8;
	case 1: h ^= data[0];
	        h *= m;
	};

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}

String String::Format(const char *format, ...){
	va_list args;
	va_start(args, format);
	char buf[1024];
	vsnprintf(buf, 1024, format, args);
	va_end(args);
	return String(buf);
}
