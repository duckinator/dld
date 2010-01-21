#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>

#include "dout.h"

#define STRINGIFY(x) #x

struct sectionptr {
	void *buf;
	unsigned int len;
};

struct sectionptr *readfile(char const *file)
{
	FILE *fd;
	uint32_t len;
	struct sectionptr *ret;
	void *buf;
	
	fd = fopen(file, "r");
	if (fd == NULL) {
		fprintf(stderr, "readfile: could not open, errno=%u\n", errno);
		exit(-1);
	}
	if (fseek(fd, 0, SEEK_END) == -1){
		fprintf(stderr, "readfile: could not seek(end), errno=%u\n", errno);
		exit(-1);
	}
	len = ftell(fd);
	if (len == -1) {
		fprintf(stderr, "readfile: could not ftell, errno=%u\n", errno);
		exit(-1);
	}
	if (fseek(fd, 0, SEEK_SET) == -1){
		fprintf(stderr, "readfile: could not seek(set), errno=%u\n", errno);
		exit(-1);
	}
	
	buf = malloc(len);
	ret = malloc(sizeof(struct sectionptr));
	if (buf == NULL) {
		fprintf(stderr, "readfile: could not allocate %u bytes for buffer\n", len);
		exit(-1);
	}
	if (ret == NULL) {
		fprintf(stderr, "readfile: could not allocate "
			STRINGIFY(sizeof(struct sectionptr)) " bytes for buffer\n");
		exit(-1);
	}

	if (fread(buf, len, 1, fd) != 1) {
		fprintf(stderr, "could not read %u bytes, errno=%u", len, errno);
		exit(-1);
	}
	if (fclose(fd) == EOF) {
		fprintf(stderr, "could not close %lu, errno=%u", (size_t)fd, errno);
		exit(-1);
	}
	
	ret->buf = buf;
	ret->len = len;
	
	return ret;
}

void writefile(void *out, uint32_t outlen, char *name)
{
	FILE *fd;
	
	fd = fopen(name, "w");
	if (fd == NULL) {
		fprintf(stderr, "writefile: could not open, errno=%u\n", errno);
		exit(-1);
	}
	
	if (fwrite(out, outlen, 1, fd) != 1) {
		fprintf(stderr, "could not write %u bytes, errno=%u", outlen, errno);
		exit(-1);
	}
	if (fclose(fd) == EOF) {
		fprintf(stderr, "could not close %lu, errno=%u", (size_t)fd, errno);
		exit(-1);
	}
}

int main (int argc, char const *argv[])
{
	struct sectionptr *text;
	uint32_t start;
	void *out;
	uint32_t outlen;
	struct dout_hdr *hdr;
	
	if (argc < 3) {
		fprintf(stderr, "usage: dld [text] [start]\n");
		exit(-1);
	}
	
	text = readfile(argv[1]);
	start = strtod(argv[2], NULL);
	
	outlen = sizeof(struct dout_hdr) + text->len;
	out = malloc(outlen);
	hdr = out;
	hdr->magic = DOUT_MAGIC;
	hdr->loc = 0x1000000; /* hardcoded to 1 MB */
	hdr->start = start;
	hdr->len = outlen;
	hdr->text = sizeof(struct dout_hdr);
	hdr->textloc = 0x0;
	hdr->textlen = text->len;
	hdr->data = 0x0;
	hdr->dataloc = 0x0;
	hdr->datalen = 0x0;
	hdr->bss = 0x0;
	hdr->bssloc = 0x0;
	hdr->bsslen = 0x0;
	hdr->symbols = 0x0;
	
	memcpy(out+sizeof(struct dout_hdr), text->buf, text->len);

	free(text->buf);
	free(text);
	
	writefile(out, outlen, "d.out");
	return 0;
}
