#define RINGBUF_DECL(T, NAME) \
    typedef struct            \
    {                         \
        int size;             \
        int start, end;       \
        T *elements;          \
    } NAME

#define RINGBUF_INIT(BUF, S, T)             \
    {                                       \
        static T static_ringbuf_mem[S + 1]; \
        BUF.elements = static_ringbuf_mem;  \
    }                                       \
    BUF.size = S;                           \
    BUF.start = 0;                          \
    BUF.end = 0;

#define NEXT_START_INDEX(BUF) \
    (((BUF)->start != (BUF)->size) ? ((BUF)->start + 1) : 0)
#define NEXT_END_INDEX(BUF) (((BUF)->end != (BUF)->size) ? ((BUF)->end + 1) : 0)

#define is_ringbuf_empty(BUF) ((BUF)->end == (BUF)->start)
#define is_ringbuf_full(BUF) (NEXT_END_INDEX(BUF) == (BUF)->start)

#define ringbuf_write_peek(BUF) (BUF)->elements[(BUF)->end]
#define ringbuf_write_skip(BUF)                   \
    do                                            \
    {                                             \
        (BUF)->end = NEXT_END_INDEX(BUF);         \
        if (is_ringbuf_empty(BUF))                \
            (BUF)->start = NEXT_START_INDEX(BUF); \
    } while (0)

#define ringbuf_read_peek(BUF) (BUF)->elements[(BUF)->start]
#define ringbuf_read_skip(BUF) (BUF)->start = NEXT_START_INDEX(BUF);

#define ringbuf_write(BUF, ELEMENT)        \
    do                                     \
    {                                      \
        ringbuf_write_peek(BUF) = ELEMENT; \
        ringbuf_write_skip(BUF);           \
    } while (0)

#define ringbuf_read(BUF, ELEMENT)        \
    do                                    \
    {                                     \
        ELEMENT = ringbuf_read_peek(BUF); \
        ringbuf_read_skip(BUF);           \
    } while (0)

#include <assert.h>

RINGBUF_DECL(int, int_buf);

int main()
{
    int_buf my_buf;
    RINGBUF_INIT(my_buf, 2, int);
    assert(is_ringbuf_empty(&my_buf));

    ringbuf_write(&my_buf, 37);
    ringbuf_write(&my_buf, 72);
    assert(!is_ringbuf_empty(&my_buf));

    int first;
    ringbuf_read(&my_buf, first);
    assert(first == 37);

    int second;
    ringbuf_read(&my_buf, second);
    assert(second == 72);

    return 0;
}