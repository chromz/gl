#include <stdio.h>
#include <stdlib.h>


inline static void w_char(FILE *file, char c)
{
	fwrite(&c, 1, 1, file);
}

inline static void w_word(FILE *file, short h)
{
	fwrite(&h, 2, 1, file);
}

inline static void w_dword(FILE * file, long l)
{
	fwrite(&l, 4, 1, file);
}

int bmp_write(int **fb, int width, int height, char *filename)
{

	FILE *file;
	file = fopen(filename, "wb");
	if (file == NULL) {
		return -1;
	}
	// Image
	w_char(file, 'B');
	w_char(file, 'M');
	w_dword(file, 54l + width * height * 3l);
	w_dword(file, 0l);
	w_dword(file, 54l);


	// Image
	w_dword(file, 40l);
	w_dword(file, (long) width);
	w_dword(file, (long) height);
	w_word(file, 1);
	w_word(file, 24);
	w_dword(file, 0l);
	w_dword(file, width * height * 3l);
	w_dword(file, 0l);
	w_dword(file, 0l);
	w_dword(file, 0l);
	w_dword(file, 0l);

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			fwrite(&fb[y][x], 1, 1, file);
		}
	}

	fclose(file);
	return 1;
}

