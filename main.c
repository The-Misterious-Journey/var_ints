#include <stdio.h>
#include <stdint.h>

#define UINT16_PREF 253
#define UINT32_PREF 254
#define UINT64_PREF 255

void serialize(uint64_t val, FILE* fd) {
    if (val < UINT16_PREF) {
        fputc(val, fd);
    } else if (val < UINT16_MAX) {
        fputc(UINT16_PREF, fd);
        fwrite(&val, sizeof(uint16_t), 1, fd);
    } else if (val < UINT32_MAX) {
        fputc(UINT32_PREF, fd);
        fwrite(&val, sizeof(uint32_t), 1, fd);
    } else {
        fputc(UINT64_PREF, fd);
        fwrite(&val, sizeof(uint64_t), 1, fd);
    }
}

void deserialize(uint64_t* val, FILE* fd) {
    const uint8_t size = fgetc(fd);
    if (size < UINT16_PREF) {
        *val = size;
    } else if (size == UINT16_PREF) {
        fread(val, sizeof(uint16_t), 1, fd);
    } else if (size == UINT32_PREF) {
        fread(val, sizeof(uint32_t), 1, fd);
    } else {
        fread(val, sizeof(uint64_t), 1, fd);
    }
}

int main() {
    uint64_t write_value = UINT64_MAX;
    FILE* fdw = fopen("ser", "wb");
    serialize(write_value, fdw);
    fclose(fdw);

    uint64_t read_value = 0;
    FILE* fdr = fopen("ser", "rb");
    deserialize(&read_value, fdr);
    fclose(fdr);

    printf("write value: %llu\n", write_value);
    printf("read value: %llu\n", read_value);
    return 0;
}
