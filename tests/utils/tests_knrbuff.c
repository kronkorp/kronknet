#include <kronklab/kronklab.h>
#include "kronknet/macros/types.h"
#include <kronknet/utils/rbuff/rbuff.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

Test(ring_buffer, invalid_size)
{
    knRBuff *buff = knRBuff_create(255);
    Assert(buff == NULL, "Buffer creation should fail for non-power-of-2 size");
}

Test(ring_buffer, valid_size)
{
    knRBuff *buff = knRBuff_create(256);
    AssertNotNull(buff, "Buffer should be successfully created");
    knRBuff_destroy(buff);
}

Test(ring_buffer, state_empty)
{
    knRBuff *buff = knRBuff_create(256);

    Assert(knRBuff_isEmpty(buff) == knTrue, "Buffer should be empty upon creation");
    AssertEq(knRBuff_usage(buff), (size_t)0, "Usage should be 0");
    // DEBUG: One byte for full or not
    AssertEq(knRBuff_remaining(buff) + 1, (size_t)256, "Remaining size should be equal to total size");

    knRBuff_destroy(buff);
}

Test(ring_buffer, valid_push_pop)
{
    knRBuff *buff = knRBuff_create(256);
    const char *src = "hello kronknet";
    char dest[32] = {0};
    size_t len = strlen(src);

    ssize_t pushed = knRBuff_push(buff, (const uint8_t *)src, len);
    AssertEq(pushed, (ssize_t)len, "Should successfully push all bytes");
    Assert(knRBuff_isEmpty(buff) == knFalse, "Buffer should not be empty after push");

    ssize_t popped = knRBuff_pop(buff, (uint8_t *)dest, len);
    AssertEq(popped, (ssize_t)len, "Should successfully pop all bytes");
    AssertEq(memcmp(src, dest, len), 0, "Popped data should match pushed data");
    Assert(knRBuff_isEmpty(buff) == knTrue, "Buffer should be empty after full pop");

    knRBuff_destroy(buff);
}

Test(ring_buffer, push_overflow)
{
    knRBuff *buff = knRBuff_create(16);
    uint8_t data[20] = {0};

    ssize_t pushed = knRBuff_push(buff, data, 20);
    AssertEq(pushed, (ssize_t)-1, "Push should fail and return -1 when size > remaining");

    knRBuff_destroy(buff);
}

Test(ring_buffer, pop_underflow)
{
    knRBuff *buff = knRBuff_create(16);
    uint8_t src[4] = {1, 2, 3, 4};
    uint8_t dest[8] = {0};

    knRBuff_push(buff, src, 4);

    ssize_t popped = knRBuff_pop(buff, dest, 6);
    AssertEq(popped, (ssize_t)-1, "Pop should fail and return -1 when requesting more bytes than usage");

    knRBuff_destroy(buff);
}

Test(ring_buffer, invalid_args_push_pop)
{
    knRBuff *buff = knRBuff_create(16);
    uint8_t data[4] = {1, 2, 3, 4};

    AssertEq(knRBuff_push(buff, NULL, 4), (ssize_t)-1, "Pushing NULL src should fail");
    AssertEq(knRBuff_pop(buff, NULL, 4), (ssize_t)-1, "Popping to NULL dest should fail");

    knRBuff_destroy(buff);
}

Test(ring_buffer, valid_peek)
{
    knRBuff *buff = knRBuff_create(16);
    const uint8_t src[4] = {42, 43, 44, 45};
    uint8_t dest[4] = {0};

    knRBuff_push(buff, src, 4);

    ssize_t peeked1 = knRBuff_peek(buff, dest, 4);
    AssertEq(peeked1, (ssize_t)4, "Peek should succeed");
    AssertEq(memcmp(src, dest, 4), 0, "Peeked data should match");
    AssertEq(knRBuff_usage(buff), (size_t)4, "Usage should not decrease after a peek");

    memset(dest, 0, 4);
    ssize_t peeked2 = knRBuff_peek(buff, dest, 4);
    AssertEq(peeked2, (ssize_t)4, "Second peek should also succeed");
    AssertEq(memcmp(src, dest, 4), 0, "Peeked data should still match");

    knRBuff_destroy(buff);
}

Test(ring_buffer, peek_underflow_and_invalid)
{
    knRBuff *buff = knRBuff_create(16);
    uint8_t src[2] = {1, 2};
    uint8_t dest[4] = {0};

    knRBuff_push(buff, src, 2);

    AssertEq(knRBuff_peek(buff, dest, 4), (ssize_t)-1, "Peek should fail if requesting more bytes than available");
    AssertEq(knRBuff_peek(buff, NULL, 2), (ssize_t)-1, "Peek to NULL destination should fail");

    knRBuff_destroy(buff);
}

Test(ring_buffer, find_sequence_success)
{
    knRBuff *buff = knRBuff_create(32);
    const uint8_t payload[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    knRBuff_push(buff, payload, sizeof(payload));

    const uint8_t target1[] = {0xCC, 0xDD};
    ssize_t idx1 = knRBuff_find(buff, target1, sizeof(target1));
    AssertEq(idx1, (ssize_t)2, "Sequence {0xCC, 0xDD} should be found at index 2");

    const uint8_t target2[] = {0xAA};
    ssize_t idx2 = knRBuff_find(buff, target2, sizeof(target2));
    AssertEq(idx2, (ssize_t)0, "Sequence {0xAA} should be found at index 0");

    knRBuff_destroy(buff);
}

Test(ring_buffer, find_sequence_not_found)
{
    knRBuff *buff = knRBuff_create(32);
    const uint8_t payload[] = {1, 2, 3, 4, 5};
    knRBuff_push(buff, payload, sizeof(payload));

    const uint8_t target[] = {3, 5};
    ssize_t idx = knRBuff_find(buff, target, sizeof(target));
    AssertEq(idx, (ssize_t)-1, "Should return -1 when sequence is not found");

    const uint8_t long_target[] = {1, 2, 3, 4, 5, 6, 7};
    AssertEq(knRBuff_find(buff, long_target, sizeof(long_target)), (ssize_t)-1, "Should return -1 if target is longer than usage");

    knRBuff_destroy(buff);
}

Test(ring_buffer, circular_wraparound)
{
    knRBuff *buff = knRBuff_create(16);
    uint8_t dummy[12] = {0};
    uint8_t pattern[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    uint8_t output[8] = {0};

    knRBuff_push(buff, dummy, 12);
    knRBuff_pop(buff, dummy, 12);

    ssize_t pushed = knRBuff_push(buff, pattern, 8);
    AssertEq(pushed, (ssize_t)8, "Should successfully push across the buffer boundary");
    AssertEq(knRBuff_usage(buff), (size_t)8, "Usage should be 8");

    const uint8_t sub_pattern[] = {4, 5, 6};
    ssize_t idx = knRBuff_find(buff, sub_pattern, sizeof(sub_pattern));
    Assert(idx != -1, "Should find the sub-pattern even if wrapped");

    ssize_t popped = knRBuff_pop(buff, output, 8);
    AssertEq(popped, (ssize_t)8, "Should successfully pop across the buffer boundary");
    AssertEq(memcmp(pattern, output, 8), 0, "Data integrity maintained after wraparound");

    knRBuff_destroy(buff);
}
