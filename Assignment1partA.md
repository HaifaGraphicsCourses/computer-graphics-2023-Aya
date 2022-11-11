# Contents

1. [ Line drawing ](#line-drawing)
2. [ Sanity check 1 result](#sanity-check-1-result)
3. [ Drawing ](#drawing)
4. [ Circle drawing ](#circle-drawing)
5. [ Sanity check 2 result](#sanity-check-2-result)
<a name="line-drawing"></a>
## Line drawing
### Line drawing - Bresenham's Algorithm

```c++
void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	// TODO: Implement bresenham algorithm
	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	int x1=p1.x, x2=p2.x, y1=p1.y, y2=p2.y, dx, dy, sx, sy, error, e2;
	dx = abs(x2 - x1); 
	sx = x1 < x2 ? 1 : -1;
	dy = (-1)*abs(y2 - y1);
	sy = y1 < y2 ? 1 : -1;
	error = dx + dy;
	while (true)
	{
		PutPixel(x1, y1, color);
		if (x1 == x2 && y1 == y2)
			break;
		e2 = 2 * error;
		if (e2 >= dy)
		{
			if (x1 == x2) 
				break;
			error += dy;
			x1 += sx;
		}
		if (e2 <= dx)
		{
			if (y1 == y2)
				break;
			error += dx;
			y1 += sy;
		}
	}
}
```
### Rendering the lines

```c++
void Renderer::Render(const Scene& scene)
{
	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;
	// Drawing line - sanity check 
	int x0 = half_width,y0 = half_height, x1, y1, r = 500,a = 60;
	for (int i = 0; i < a; i++) {
		x1 = x0 + r * sin((2 * M_PI * i) / a);
		y1 = y0 + r * cos((2 * M_PI * i) / a);
		DrawLine(glm::ivec2(x0, y0), glm::ivec2(x1, y1), glm::vec3(1, 0, 2));
	}
}
```
### Picture of the results
![image](https://user-images.githubusercontent.com/83645233/201371508-7735b434-1bb7-478c-bcca-a272e0bc745b.png)
