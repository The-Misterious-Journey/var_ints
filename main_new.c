#include <stdio.h>
#include <stdint.h>

void serialize(uint64_t val, FILE* fd) {
    uint8_t tmp[(sizeof(val) * 8 + 6) / 7];
    int len = 0;
    while (1) {
        tmp[len] = (val & 0x7f) | (len ? 0x80 : 0);
        if (val < 0x7f) {
            break;
        }

        val = val >> 7;
        ++len;
    }

    do {
        fputc(tmp[len], fd);
    } while (len--);
}

void deserialize(uint64_t* val, FILE* fd) {
    uint64_t res = 0;
    while (1) {
        uint8_t byte = fgetc(fd);
        res = (res << 7) | (byte & 0x7f);
        if (!(byte & 0x80)) {
            *val = res;
            break;
        }
    }
}

int main() {
    uint64_t write_num = UINT64_MAX;
    printf("number to write = %llu\n", write_num);
    FILE* fdw = fopen("ser_new", "wb");
    serialize(write_num, fdw);
    fclose(fdw);

    FILE* fdr = fopen("ser_new", "rb");
    uint64_t read_num = 0;
    deserialize(&read_num, fdr);
    fclose(fdr);
    printf("number from file = %llu\n", read_num);
    return 0;
}
