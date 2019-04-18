// Rodrigo Custodio

#include <stdio.h>
#include <stdlib.h>


static inline  void w_char(FILE *file, char c)
{
	fwrite(&c, 1, 1, file);
}

static inline  void w_word(FILE *file, short h)
{
	fwrite(&h, 2, 1, file);
}

static inline  void w_dword(FILE * file, long l)
{
	fwrite(&l, 4, 1, file);
}

static void buffer_free(int **buffer, int allocated)
{
	for (int i = 0; i < allocated; i++) {
		free(buffer[i]);
	}
	free(buffer);
}

int bmp_load(const char *filename, int ***buffer, long *width, long *height)
{
	if (*buffer != NULL || width == NULL || height == NULL) {
		return 0;
	}
	*width = 0;
	*height = 0;
	FILE *file;
	file = fopen(filename, "rbe");
	if (file == NULL) {
		return 0;
	}
	fseek(file, 10, SEEK_SET);
	long header_s = 0;
	if (fread(&header_s, 4, 1, file) == 0) {
		fclose(file);
		return 0;
	}
	if(fseek(file, 4, SEEK_CUR) != 0) {
		fclose(file);
		return 0;
	}
	if (fread(width, 4, 1, file) == 0) {
		fclose(file);
		return 0;
	}
	if (fread(height, 4, 1, file) == 0) {
		fclose(file);
		return 0;
	}
	fseek(file, header_s, SEEK_SET);
	*buffer = malloc((*height) * sizeof(int *));
	for (int i = 0; i < *height; i++) {
		(*buffer)[i] = malloc((*width) * sizeof(int));
		for (int j = 0; j < *width; j++) {
			char color[3];
			if (fread(color, 1, 3, file) != 3) {
				buffer_free(*buffer, i);
				fclose(file);
				return 0;
			}
			(*buffer)[i][j] = (color[2] << 16U) + (color[1] << 8U) +
				    color[0];
		}
	}
	/* fseek(file, 2, SEEK_SET); */
		
	fclose(file);
	return 1;
}

int bmp_write(const char *filename, int *fb, int width, int height)
{

	FILE *file;
	file = fopen(filename, "wbe");
	if (file == NULL) {
		return -1;
	}
	// Image
	w_char(file, 'B');
	w_char(file, 'M');
	w_dword(file, 54L + width * height * 3L);
	w_dword(file, 0L);
	w_dword(file, 54L);


	// Image
	w_dword(file, 40L);
	w_dword(file, (long) width);
	w_dword(file, (long) height);
	w_word(file, 1);
	w_word(file, 24);
	w_dword(file, 0L);
	w_dword(file, width * height * 3L);
	w_dword(file, 0L);
	w_dword(file, 0L);
	w_dword(file, 0L);
	w_dword(file, 0L);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			fwrite(&fb[y * width + x], 3, 1, file);
		}
	}

	fclose(file);
	return 1;
}

