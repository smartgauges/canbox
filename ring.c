#include "ring.h"

void ring_init(struct ring_t *ring, uint8_t *buf, ring_size_t size)
{
	ring->data = buf;
	ring->size = size;
	ring->begin = 0;
	ring->end = 0;
	ring->overflow = 0;
}

int32_t ring_write_ch(struct ring_t *ring, uint8_t ch)
{
	if (((ring->end + 1) % ring->size) != ring->begin) {

		ring->data[ring->end++] = ch;
		ring->end %= ring->size;

		return (uint32_t)ch;
	}

	ring->overflow++;

	return -1;
}

int32_t ring_write(struct ring_t *ring, uint8_t *data, ring_size_t size)
{
	int32_t i;

	for (i = 0; i < size; i++) {

		if (ring_write_ch(ring, data[i]) < 0)
			return -i;
	}

	return i;
}

uint8_t ring_read_ch(struct ring_t *ring, uint8_t *ch)
{
	if (ring->begin != ring->end) {

		*ch = ring->data[ring->begin++];
		ring->begin %= ring->size;

		return 1;
	}

	return 0;
}

uint32_t ring_get_overflow(struct ring_t *ring)
{
	return ring->overflow;
}

