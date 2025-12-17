#include "../src/queue.h"
#include <assert.h>
#include <stdio.h>

int
main()
{
        queue_t *q = queue_create(10);
        assert(q != NULL);

        /* Test enqueue */
        assert(queue_enqueue(q, 1) == 0);
        assert(queue_enqueue(q, 2) == 0);
        assert(queue_size(q) == 2);
        assert(!queue_is_empty(q));

        /* Test dequeue */
        int out = 0;
        assert(queue_dequeue(q, &out));
        assert(out == 1);
        assert(queue_dequeue(q, &out));
        assert(out == 2);
        assert(queue_is_empty(q));

        /* Test underflow */
        assert(!queue_dequeue(q, &out));

        /* Test overflow */
        for (int i = 0; i < 10; ++i) {
                assert(queue_enqueue(q, i) == 0);
        }
        assert(queue_is_full(q));
        assert(queue_enqueue(q, 99) == -1); /* overflow */

        queue_destroy(q);
        printf("All tests passed.\n");
        return 0;
}
