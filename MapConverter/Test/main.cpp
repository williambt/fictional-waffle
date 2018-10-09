#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <fstream>

int width;
int height;

bool* searched;

float remap(float val, float min, float max, float nmin, float nmax)
{
	return (val - min) / (max - min) * (nmax - nmin) + nmin;
}

struct Rect
{
	float left, top, right, bottom;

	Rect(float left, float top, float right, float bottom) : left(left), top(top), right(right), bottom(bottom) {}
	Rect() : left(0), top(0), right(0), bottom(0) {}
};

Rect wb = Rect(-10, 10, 10, -10);

struct Colour
{
	unsigned char r, g, b;

	Colour(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {}
	Colour(unsigned char val) : r(val), g(val), b(val) {}
	Colour() : r(0), g(0), b(0) {}


	static Colour grey;
	static Colour white;
	static Colour yellow;

	bool operator==(const Colour& other) const { return other.r == r && other.g == g && other.b == b; }
	bool operator!=(const Colour& other) const { return other.r != r || other.g != g || other.b != b; }
};
Colour Colour::grey = Colour(137);
Colour Colour::white = Colour(255);
Colour Colour::yellow = Colour(255, 218, 0);


int gi(int x, int y)
{
	return (y * width + x);
}

struct vec2
{
	int x, y;
	vec2(int x, int y) : x(x), y(y) {}
	vec2() : x(0), y(0) {}
};

void markSearched(vec2 beg, vec2 end)
{
	for (int i = beg.x; i <= end.x; ++i)
	{
		for (int j = beg.y; j <= end.y; ++j)
		{
			searched[gi(i, j)] = true;
		}
	}
}

Colour* ucToColour(unsigned char* pixels, int width, int height)
{
	Colour* res = (Colour*)malloc(width * height * sizeof(Colour));
	for (unsigned int i = 0; i < width * height * 3; i += 3)
	{
		res[i / 3].r = pixels[i];
		res[i / 3].g = pixels[i+1];
		res[i / 3].b = pixels[i+2];
	}

	return res;
}

std::ofstream file;

int main()
{
	int comp;
	unsigned char* stbPixels = stbi_load("test2.png", &width, &height, &comp, 3);
	if ((float)width / (float)height != 16.0f / 9.0f)
	{
		std::cout << "Wrong ratio!" << std::endl;
		stbi_image_free(stbPixels);
		return 1;
	}

	float ratio = (float)width / (float)height;
	wb.left *= ratio;
	wb.right *= ratio;

	file.open("test2.mlo", std::ofstream::out | std::ofstream::trunc);

	Colour* pixels = ucToColour(stbPixels, width, height);

	searched = (bool*)malloc(width*height * sizeof(bool));
	memset(searched, false, width*height * sizeof(bool));

	int x = 0, y = 0;
	vec2 start;
	vec2 end;
	while (true)
	{
		if (y > height)
			break;

		if (pixels[gi(x, y)] == Colour::grey && !searched[gi(x, y)])
		{
			start = vec2(x, y);
			int i = x;
			for (; i < width && pixels[gi(i, y)] != Colour::white && !searched[gi(i, y)]; ++i)
			{
			}
			end = vec2(i - 1, height);
			for (i = x; i <= end.x; ++i)
			{
				int j = y;
				for (; j < height && pixels[gi(i, j)] != Colour::white; ++j)
				{
				}
				if (j - 1 < end.y)
					end.y = j - 1;
			}

			file << "ground " << remap(start.x, 0, width, wb.left, wb.right) << " " << remap(start.y, 0, height, wb.top, wb.bottom) << " " << remap(end.x, 0, width, wb.left, wb.right) << " " << remap(end.y, 0, height, wb.top, wb.bottom) << "\n";
			markSearched(start, end);
			if (end.x - start.x == width)
			{
				x = 0;
				y = end.y + 1;
			}
			else if (end.x == width - 1)
			{
				++y;
				x = 0;
			}
			else ++x;

			continue;
		}

		else if (pixels[gi(x, y)] == Colour::yellow && !searched[gi(x, y)])
		{
			start = vec2(x, y);
			int i = x;
			for (; i < width && pixels[gi(i, y)] != Colour::white && !searched[gi(i, y)]; ++i)
			{
			}
			end = vec2(i - 1, height);
			for (i = x; i <= end.x; ++i)
			{
				int j = y;
				for (; j < height && pixels[gi(i, j)] != Colour::white; ++j)
				{
				}
				if (j - 1 < end.y)
					end.y = j - 1;
			}

			file << "box " << remap(start.x, 0, width, wb.left, wb.right) << " " << remap(start.y, 0, height, wb.top, wb.bottom) << " " << remap(end.x, 0, width, wb.left, wb.right) << " " << remap(end.y, 0, height, wb.top, wb.bottom) << "\n";
			markSearched(start, end);
			if (end.x - start.x == width)
			{
				x = 0;
				y = end.y + 1;
			}
			else if (end.x == width - 1)
			{
				++y;
				x = 0;
			}
			else ++x;

			continue;
		}

		++x;
		if (x >= width)
		{
			x = 0;
			++y;
		}
	}

	std::cout << "FINISHED! \nPress ENTER to exit!" << std::endl;
	std::cin.get();

	file.close();

	free(searched);

	return 0;
}
