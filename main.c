#include "includes.h"
#include "cpu.h"


void read_file(vCPU32* cpu, char *filename)
{
    FILE *file;
    uint8_t *buffer;
    unsigned long fileLen;

    //Open file
    file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Unable to open file %s", filename);
    }
    //Get file length
    fseek(file, 0, SEEK_END);
    fileLen=ftell(file);
    fseek(file, 0, SEEK_SET);

    //Allocate memory
    buffer=(uint8_t *)malloc(fileLen+1);
    if (!buffer) {
        fprintf(stderr, "Memory error!");
        fclose(file);
    }
    //Read file contents into buffer
    fread(buffer, fileLen, 1, file);
    fclose(file);
    // Print file contents in hex
    for (int i=0; i<fileLen; i+=2) {
        if (i%16==0) printf("\n%.8x: ", i);
        printf("%02x%02x ", *(buffer+i), *(buffer+i+1));
    }
    printf("\n");

    memcpy(cpu->bus.dram->mem, buffer, fileLen*sizeof(uint8_t));
    free(buffer);
}

int main() {

}