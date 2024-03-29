# Contents

1. [ Line drawing ](#line-drawing)
2. [ Sanity check 1 result](#sanity-check-1-result)
3. [ Our drawing ](#drawing)
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
	glm::ivec2 p1(150, 150);
	glm::ivec2 p2(200, 200);
	glm::ivec2 p3(500, 150);
	glm::ivec2 p4(150, 500);
	glm::ivec2 p6(250, 450);
	glm::ivec2 p7(450, 250);
	glm::ivec2 p8(350, 100);
	glm::vec3 color1(0, 120, 0);
	glm::vec3 color2(2, 0, 0);
	glm::vec3 color3(2, 0.8, 0.243);
	glm::vec3 color4(0.21, 0.78, 0.12);
	glm::vec3 color5(0.9, 0.52, 10);
	glm::vec3 color6(0.8, 0.1, 0.578);
	DrawLine(p1, p2, color1);
	DrawLine(p1, p3, color2);
	DrawLine(p1, p4, color3);
	DrawLine(p1, p6, color4);
	DrawLine(p1, p7, color5);
	DrawLine(p1, p8, color6);
 }
 ```
 ### Picture of the result
 ![image](https://user-images.githubusercontent.com/83645233/201435962-3c91ef40-2663-4428-a804-e62440441b3f.png)
<a name="sanity-check-1-result"></a>
## Sanity check 1 result

### Rendering the sanity check 1
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
### Picture of the result
![image](https://user-images.githubusercontent.com/83645233/201371508-7735b434-1bb7-478c-bcca-a272e0bc745b.png)
<a name="drawing"></a>
## Our drawing
### Code
 ```c++
void Renderer::Render(const Scene& scene)
{
	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;
	//Our Drawing - Heart CG (Computer Graphics)
	glm::ivec2 pa(200, 100);
	glm::ivec2 pb(50, 200);
	glm::ivec2 pc(50, 400);
	glm::ivec2 pd(200,350);
	glm::ivec2 pe(350, 400);
	glm::ivec2 pf(350, 200);
	glm::ivec2 pg(150, 400);
	glm::ivec2 ph(250, 400);
	glm::vec3 color1(139, 0, 0);
	DrawLine(pa, pb, color1);
	DrawLine(pb, pc, color1);
	DrawLine(pc, pg, color1);
	DrawLine(pg, pd, color1);
	DrawLine(pd, ph, color1);
	DrawLine(ph, pe, color1);
	DrawLine(pe, pf, color1);
	DrawLine(pf, pa, color1);
	glm::vec3 color2(0, 0, 0);
	glm::ivec2 p1(400, 200);
	glm::ivec2 p2(500, 200);
	glm::ivec2 p3(400, 400);
	glm::ivec2 p4(500, 400);
	DrawLine(p1, p2, color2);
	DrawLine(p1, p3, color2);
	DrawLine(p3, p4, color2);
	glm::ivec2 p5(550, 200);
	glm::ivec2 p6(650, 200);
	glm::ivec2 p7(550, 400);
	glm::ivec2 p8(650, 400);
	glm::ivec2 p9(650, 300);
	glm::ivec2 p10(620, 300);
	DrawLine(p5, p6, color2);
	DrawLine(p5, p7, color2);
	DrawLine(p7, p8, color2);
	DrawLine(p6, p9, color2);
	DrawLine(p10, p9, color2);
}
```
### Picture of the result
![image](https://user-images.githubusercontent.com/95486750/201492743-b5a7f632-6026-4cbb-968f-e269f7722650.png)

<a name="circle-drawing"></a>
## Circle drawing
### Draw circle function
```c++
void Renderer::DrawCircle(const glm::ivec2& p1, double r, const glm::vec3& color)
{
	for (int i = p1.x - r; i <= p1.x + r; i++) {
		PutPixel(i, sqrt(r * r - (i - p1.x) * (i - p1.x)) + p1.y, color);
		PutPixel(i, (sqrt(r * r - (i - p1.x) * (i - p1.x)) + p1.y) - 2 * ((sqrt(r * r - (i - p1.x) * (i - p1.x)) + p1.y) - p1.y), color);
	}
	for (int i = p1.y - r; i <= p1.y + r; i++) {
		PutPixel(sqrt(r * r - (i - p1.y) * (i - p1.y)) + p1.x, i, color);
		PutPixel((sqrt(r * r - (i - p1.y) * (i - p1.y)) + p1.x) - 2 * ((sqrt(r * r - (i - p1.y) * (i - p1.y)) + p1.x) - p1.x), i, color);
	}
}
```
<a name="sanity-check-2-result"></a>
## Sanity check 2 result
### Rendering the circles
```c++
void Renderer::Render(const Scene& scene)
{
	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;
	// Drawing the 2 circles
	glm::ivec2 p1(100, 170);
	glm::ivec2 p2(160, 190);
	glm::vec3 color1(0, 1, 200);
	glm::vec3 color2(1, 0, 3);
	DrawCircle(p1, 110, color1);
	DrawCircle(p2, 80, color2);
}
```
### Picture of the result
![image](https://user-images.githubusercontent.com/83645233/201440618-cf8dd061-02d3-4081-82e1-fa6f5537c792.png)
