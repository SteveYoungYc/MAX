#include <input_manager.h>

void ringBufferInit(RingBuffer* buffer) {
    buffer->len = RING_BUFF_LEN;
    buffer->read = 0;
    buffer->write = 0;
    memset(buffer->inputEvents, 0, sizeof(buffer->inputEvents));
    pthread_mutex_init(&buffer->mutex, NULL);
    pthread_cond_init(&buffer->cond, NULL);
}

void ringBufferExit(RingBuffer* buffer) {
    pthread_mutex_destroy(&buffer->mutex);
    pthread_cond_destroy(&buffer->cond);
}

int isBufferFull(RingBuffer* buffer) {
    return ((buffer->write + 1) % buffer->len) == buffer->read;
}

int isBufferEmpty(RingBuffer* buffer) {
    return buffer->write == buffer->read;
}

int putToBuffer(RingBuffer* buffer, InputEvent* event) {
    if (isBufferFull(buffer)) {
        return -1;
    }
    buffer->inputEvents[buffer->write] = *event;
    buffer->write = (buffer->write + 1) % buffer->len;
    return 0;
}

int getFromBuffer(RingBuffer* buffer, InputEvent* event) {
    if (isBufferEmpty(buffer)) {
        return -1;
    }
    *event = buffer->inputEvents[buffer->read];
    buffer->read = (buffer->read + 1) % buffer->len;
    return 0;
}
