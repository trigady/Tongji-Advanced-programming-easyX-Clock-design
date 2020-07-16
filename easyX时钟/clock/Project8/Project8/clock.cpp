#include <iostream>
#include <iomanip>
#include <graphics.h>
#include <math.h>
#include <conio.h>
#include <time.h>
using namespace std;
#define pi 3.1415926

struct tm t;  //注意查阅文档

struct pix {
	int color = 0x000000;
}pixx[640][480];

struct rgb {
	short r;
	short g;
	short b;
};

struct dot {
	int x;
	int y;
};

struct reco {
	int h;
	int m;
	int s;
}time_las;

struct reco_line {
	dot a;
	dot b;
};

struct {
	reco_line shi;
	reco_line fen;
	reco_line miao;
}lines;

struct {
	int color[7] = {
		0x222272,0x222272,0x222272,0x222272,0x222272,0x222272,0x222272
	};
	reco_line s[7];
}shu[4];

struct {
	dot pos;
	int color = 0x222272;
}maohao[2];

void drawpixx(int x, int y, int xend, int yend) {
	for (int i = x; i < xend; i++) {
		for (int j = y; j < yend; j++) {
			putpixel(i, j, pixx[i][j].color);
		}
	}
}

void goget(int a[], int x, int y, int sizex, int sizey, const int ll) {
	rgb s, b, c, d;
	int ans, __;
	for (int i = x; i < x + sizex; i++) {
		for (int j = y; j < y + sizey; j++) {
			s.r = a[2 * (i - x) * ll + 2 * (j - y)] / 65536;
			s.g = a[2 * (i - x) * ll + 2 * (j - y)] / 256 % 256;
			s.b = a[2 * (i - x) * ll + 2 * (j - y)] % 256;
			b.r = a[(2 * (i - x) + 1) * ll + 2 * (j - y)] / 65536;
			b.g = a[(2 * (i - x) + 1) * ll + 2 * (j - y)] / 256 % 256;
			b.b = a[(2 * (i - x) + 1) * ll + 2 * (j - y)] % 256;
			c.r = a[2 * (i - x) * ll + 2 * (j - y) + 1] / 65536;
			c.g = a[2 * (i - x) * ll + 2 * (j - y) + 1] / 256 % 256;
			c.b = a[2 * (i - x) * ll + 2 * (j - y) + 1] % 256;
			d.r = a[(2 * (i - x) + 1) * ll + 2 * (j - y) + 1] / 65536;
			d.g = a[(2 * (i - x) + 1) * ll + 2 * (j - y) + 1] / 256 % 256;
			d.b = a[(2 * (i - x) + 1) * ll + 2 * (j - y) + 1] % 256;

			__ = s.r + b.r + c.r + d.r;
			__ = __ / 4;
			ans = __;
			ans = ans * 256;

			__ = s.g + b.g + c.g + d.g;
			__ = __ / 4;
			ans = ans + __;
			ans = ans * 256;

			__ = s.b + b.b + c.b + d.b;
			__ = __ / 4;
			ans = ans + __;

			pixx[i][j].color = ans;
		}
	}
}

void circle_(int x, int y, int r) {							//这里画外圆轮廓采用了超采样方法
	const int width = 2;
	const int ll = 420;
	int a, b, c, d;
	int temp[176400];
	for (int i = 0; i < r * 4 + 20; i++) {
		for (int j = 0; j < r * 4 + 20; j++) {
			a = i - 2 * r - 10;
			a = a * a;
			b = j - 2 * r - 10;
			b = b * b;
			c = 2 * r - width * 2;
			c = c * c;
			d = 2 * r + width * 2;
			d = d * d;
			if (a + b > c&&a + b < d) {
				temp[i*ll + j] = 0xffffff;
			}
			else if (a + b <= c){
				a = x - r - 5 + i / 2;
				b = y - r - 5 + j / 2;
				temp[i*ll + j] = getpixel(a, b);
			}
			else {
				temp[i*ll + j] = 0x0f1f2f;
			}
		}
	}
	goget(temp, x - r - 5, y - r - 5, 2 * r + 10, 2 * r + 10, ll);
}

void line_(dot a, dot b, int width, int color) {			//这里画直线采用了距离场方法
	if (a.x < b.x) {
		line_(b, a, width, color);
		return;
	}
	const int ll = 130;
	int temp[16900];
	int p;
	double g, h, q, w, r;
	double k;
	if (a.x != b.x) {
		k = (double)(a.y - b.y) / (double)(a.x - b.x);
	}
	else {
		k = 1.797e308;
	}
	double k_;
	if (a.y != b.y) {
		k_ = -1 / k;
	}
	else {
		k_ = 1.797e308;
	}
	rgb fin, target, back;
	target.r = color / 65536;
	target.g = color / 256 % 256;
	target.b = color % 256;
	dot sizemin, sizemax, x1, x2;
	sizemin.x = a.x < b.x ? a.x : b.x;
	sizemin.y = a.y < b.y ? a.y : b.y;
	sizemax.x = a.x > b.x ? a.x : b.x;
	sizemax.y = a.y > b.y ? a.y : b.y;
	sizemax.x += 5;
	sizemax.y += 5;
	sizemin.x -= 5;
	sizemin.y -= 5;
	if (a.y > b.y) {
		x1.x = a.x;
		x1.y = a.y;
		x2.x = b.x;
		x2.y = b.y;
	}
	else {
		x1.x = b.x;
		x1.y = b.y;
		x2.x = a.x;
		x2.y = a.y;
	}

	for (int i = sizemin.x; i < sizemax.x; i++) {
		for (int j = sizemin.y; j < sizemax.y; j++) {
			g = i - x1.x;
			g = g * k_;
			g += x1.y;
			if (g <= j) {
				q = (i - x1.x)*(i - x1.x);
				w = (j - x1.y)*(j - x1.y);
				q = q + w;
				q = sqrt(q);
			}
			else {
				g = i - x2.x;
				g = g * k_;
				g += x2.y;
				if (g >= j) {
					q = (i - x2.x)*(i - x2.x);
					w = (j - x2.y)*(j - x2.y);
					q += w;
					q = sqrt(q);
				}
				else {
					g = (x2.y - x1.y)*i;
					h = (x2.x - x1.x)*j;
					r = x2.x*x1.y - x1.x*x2.y;
					g = g - h + r;
					g = abs(g);
					h = (x2.y - x1.y)*(x2.y - x1.y);
					r = (x2.x - x1.x)*(x2.x - x1.x);
					h = h + r;
					h = sqrt(h);
					q = g / h;
				}
			}

			q -= (double)width / 2;
			r = 1.25 - q;
			if (r >= 0 && r <= 2.5) {
				r = r / 2.5;
				p = getpixel(i, j);
				back.r = p / 65536;
				back.g = p / 256 % 256;
				back.b = p % 256;
				fin.r = target.r*r + back.r*(1 - r);
				fin.g = target.g*r + back.g*(1 - r);
				fin.b = target.b*r + back.b*(1 - r);
				p = fin.r * 65536 + fin.g * 256 + fin.b;
				putpixel(i, j, p);
			}
			else if (q < -0.1) {
				p = target.r * 65536 + target.g * 256 + target.b;
				putpixel(i, j, p);
			}
		}
	}

}

void line_clear(dot a, dot b, int width) {			//这里用于除去上一秒的指针
	if (a.x < b.x) {
		line_clear(b, a, width);
		return;
	}
	const int ll = 130;
	int temp[16900];
	int p;
	double g, h, q, w, r;
	double k;
	if (a.x != b.x) {
		k = (double)(a.y - b.y) / (double)(a.x - b.x);
	}
	else {
		k = 1.797e308;
	}
	double k_;
	if (a.y != b.y) {
		k_ = -1 / k;
	}
	else {
		k_ = 1.797e308;
	}

	dot sizemin, sizemax, x1, x2;
	sizemin.x = a.x < b.x ? a.x : b.x;
	sizemin.y = a.y < b.y ? a.y : b.y;
	sizemax.x = a.x > b.x ? a.x : b.x;
	sizemax.y = a.y > b.y ? a.y : b.y;
	sizemax.x += 5;
	sizemax.y += 5;
	sizemin.x -= 5;
	sizemin.y -= 5;
	if (a.y > b.y) {
		x1.x = a.x;
		x1.y = a.y;
		x2.x = b.x;
		x2.y = b.y;
	}
	else {
		x1.x = b.x;
		x1.y = b.y;
		x2.x = a.x;
		x2.y = a.y;
	}

	for (int i = sizemin.x; i < sizemax.x; i++) {
		for (int j = sizemin.y; j < sizemax.y; j++) {
			g = i - x1.x;
			g = g * k_;
			g += x1.y;
			if (g <= j) {
				q = (i - x1.x)*(i - x1.x);
				w = (j - x1.y)*(j - x1.y);
				q = q + w;
				q = sqrt(q);
			}
			else {
				g = i - x2.x;
				g = g * k_;
				g += x2.y;
				if (g >= j) {
					q = (i - x2.x)*(i - x2.x);
					w = (j - x2.y)*(j - x2.y);
					q += w;
					q = sqrt(q);
				}
				else {
					g = (x2.y - x1.y)*i;
					h = (x2.x - x1.x)*j;
					r = x2.x*x1.y - x1.x*x2.y;
					g = g - h + r;
					g = abs(g);
					h = (x2.y - x1.y)*(x2.y - x1.y);
					r = (x2.x - x1.x)*(x2.x - x1.x);
					h = h + r;
					h = sqrt(h);
					q = g / h;
				}
			}

			q -= (double)width / 2;
			if (q < 1) {
				p = pixx[i][j].color;
				putpixel(i, j, p);
			}
		}
	}

}

void circle__(int x, int y, int rr, int color) {				//这里画实心圆也采用了距离场方法
	rgb fin, target, back;
	target.r = color / 65536;
	target.g = color / 256 % 256;
	target.b = color % 256;
	int xa, xb, ya, yb, p;
	double r;
	xa = x - rr - 5;
	xb = x + rr + 5;
	ya = y - rr - 5;
	yb = y + rr + 5;
	double q, w;
	for (int i = xa; i <= xb; i++) {
		for (int j = ya; j <= yb; j++) {
			q = (double)(i - x);
			w = (double)(j - y);
			q = q * q;
			w = w * w;
			q = q + w;
			q = sqrt(q);
			q = q - rr;
			q -= 0.5;
			r = 1.25 - q;
			r = r > 2.5 ? 2.5 : r;
			if (r >= 0) {
				r = r / 2.5;
				p = getpixel(i, j);
				back.r = p / 65536;
				back.g = p / 256 % 256;
				back.b = p % 256;
				fin.r = target.r*r + back.r*(1 - r);
				fin.g = target.g*r + back.g*(1 - r);
				fin.b = target.b*r + back.b*(1 - r);
				p = fin.r * 65536 + fin.g * 256 + fin.b;
				putpixel(i, j, p);
			}
		}
	}
}

void shude(dot a, dot b, int color) {
	int width = 4;
	int xl = a.x - width - 4;
	int xr = a.x + width + 4;
	int yt = a.y - width - 4;
	int yb = b.y + width + 4;

	const double sqrt2 = sqrt(2);

	double q, r;
	int w, p;

	rgb back, target, fin;
	target.r = color / 65536;
	target.g = color / 256 % 256;
	target.b = color % 256;

	back.r = 0;
	back.g = 0;
	back.b = 0;

	for (int i = xl; i <= xr; i++) {
		for (int j = yt; j <= yb; j++) {
			if (j < a.y) {
				if (i <= a.x) {
					w = a.x - width;
					w = w + a.y;
					q = (double)(i + j - w);
					q = q / sqrt2;
					q = -q;
				}
				else {
					w = a.x + width;
					w = a.y - w;
					q = (double)(j - i - w);
					q = q / sqrt2;
					q = -q;
				}
			}
			else if (j <= b.y) {
				if (i < a.x) {
					w = a.x - width;
					q = (double)(w - i);
				}
				else {
					w = a.x + width;
					q = (double)(i - w);
				}
			}
			else {
				if (i > a.x) {
					w = a.x + width;
					w = w + b.y;
					q = (double)(i + j - w);
					q = q / sqrt2;
				}
				else {
					w = a.x - width;
					w = b.y - w;
					q = (double)(j - i - w);
					q = q / sqrt2;
				}
			}
			q -= 0.5;
			r = 0.9 - q;
			r = r > 1.8 ? 1.8 : r;
			if (r >= 0) {
				r = r / 1.8;
				fin.r = target.r*r + back.r*(1 - r);
				fin.g = target.g*r + back.g*(1 - r);
				fin.b = target.b*r + back.b*(1 - r);
				p = fin.r * 65536 + fin.g * 256 + fin.b;
				putpixel(i, j, p);
			}
		}
	}
}

void hengde(dot a, dot b, int color) {
	int width = 5;
	int xl = a.x - width - 4;
	int xr = b.x + width + 4;
	int yt = a.y - width - 4;
	int yb = b.y + width + 4;

	const double sqrt2 = sqrt(2);

	double q, r;
	int w, p;

	rgb back, target, fin;
	target.r = color / 65536;
	target.g = color / 256 % 256;
	target.b = color % 256;

	back.r = 0;
	back.g = 0;
	back.b = 0;

	for (int i = xl; i <= xr; i++) {
		for (int j = yt; j <= yb; j++) {
			if (i < a.x) {
				if (j <= a.y) {
					w = a.x - width;
					w = w + a.y;
					q = (double)(i + j - w);
					q = q / sqrt2;
					q = -q;
				}
				else {
					w = a.x - width;
					w = a.y - w;
					q = (double)(j - i - w);
					q = q / sqrt2;
				}
			}
			else if (i <= b.x) {
				if (j < a.y) {
					w = a.y - width;
					q = (double)(w - j);
				}
				else {
					w = a.y + width;
					q = (double)(j - w);
				}
			}
			else {
				if (j > a.y) {
					w = b.x + width;
					w = w + b.y;
					q = (double)(i + j - w);
					q = q / sqrt2;
				}
				else {
					w = b.x + width;
					w = b.y - w;
					q = (double)(j - i - w);
					q = q / sqrt2;
					q = -q;
				}
			}
			q -= 0.5;
			r = 0.9 - q;
			r = r > 1.8 ? 1.8 : r;
			if (r >= 0) {
				r = r / 1.8;
				fin.r = target.r*r + back.r*(1 - r);
				fin.g = target.g*r + back.g*(1 - r);
				fin.b = target.b*r + back.b*(1 - r);
				p = fin.r * 65536 + fin.g * 256 + fin.b;
				putpixel(i, j, p);
			}
		}
	}
}

void dian(dot a, int color) {
	int width = 5;
	int xl = a.x - width - 4;
	int xr = a.x + width + 4;
	int yt = a.y - width - 4;
	int yb = a.y + width + 4;

	const double sqrt2 = sqrt(2);

	double q, r;
	int w, p;

	rgb back, target, fin;
	target.r = color / 65536;
	target.g = color / 256 % 256;
	target.b = color % 256;

	back.r = 0;
	back.g = 0;
	back.b = 0;

	for (int i = xl; i <= xr; i++) {
		for (int j = yt; j <= yb; j++) {
			if (i < a.x) {
				if (j <= a.y) {
					w = a.x - width;
					w = w + a.y;
					q = (double)(i + j - w);
					q = q / sqrt2;
					q = -q;
				}
				else {
					w = a.x - width;
					w = a.y - w;
					q = (double)(j - i - w);
					q = q / sqrt2;
				}
			}
			else {
				if (j > a.y) {
					w = a.x + width;
					w = w + a.y;
					q = (double)(i + j - w);
					q = q / sqrt2;
				}
				else {
					w = a.x + width;
					w = a.y - w;
					q = (double)(j - i - w);
					q = q / sqrt2;
					q = -q;
				}
			}
			q -= 0.5;
			r = 0.9 - q;
			r = r > 1.8 ? 1.8 : r;
			if (r >= 0) {
				r = r / 1.8;
				fin.r = target.r*r + back.r*(1 - r);
				fin.g = target.g*r + back.g*(1 - r);
				fin.b = target.b*r + back.b*(1 - r);
				p = fin.r * 65536 + fin.g * 256 + fin.b;
				putpixel(i, j, p);
			}
		}
	}
}

void draw_number() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 7; j++) {
			if (j % 2 == 0) {
				shude(shu[i].s[j].a, shu[i].s[j].b, shu[i].color[j]);
			}
			else {
				hengde(shu[i].s[j].a, shu[i].s[j].b, shu[i].color[j]);
			}
		}
	}
	for (int i = 0; i < 2; i++) {
		dian(maohao[i].pos, maohao[i].color);
	}
}

void change_num(int k, int n) {
	bool a[7] = {
		false,false,false,false,false,false,false
	};
	switch (n){
		case 0:
			a[0] = a[1] = a[2] = a[4] = a[5] = a[6] = true;
			break;
		case 1:
			a[2] = a[6] = true;
			break;
		case 2:
			a[1] = a[2] = a[3] = a[4] = a[5] = true;
			break;
		case 3:
			a[1] = a[2] = a[3] = a[6] = a[5] = true;
			break;
		case 4:
			a[0] = a[3] = a[2] = a[6] = true;
			break;
		case 5:
			a[1] = a[0] = a[3] = a[6] = a[5] = true;
			break;
		case 6:
			a[1] = a[0] = a[4] = a[5] = a[6] = a[3] = true;
			break;
		case 7:
			a[1] = a[2] = a[6] = true;
			break;
		case 8:
			a[2]= a[1] = a[0] = a[4] = a[5] = a[6] = a[3] = true;
			break;
		case 9:
			a[2] = a[1] = a[0] = a[5] = a[6] = a[3] = true;
			break;
	}
	for (int i = 0; i < 7; i++) {
		if (a[i]) {
			shu[k].color[i] = 0x4763ff;
		}
		else {
			shu[k].color[i] = 0x222272;
		}
	}
}

void init() {
	IMAGE img;

	setbkcolor(0x0f1f2f);
	cleardevice();

	//initgraph(640, 480);	// 创建绘图窗口，大小为 640x480 像素
	loadimage(&img, ".\\images\\lenademo.png");
	putimage(220, 55, &img);

	/*setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 4);
	circle(320, 155, 100);
	setfillcolor(WHITE);
	fillcircle(320, 155, 2);
	settextstyle(24, 12, _T("华文楷体"), 0, 0, 2, true, false, false);
	outtextxy(320 - textwidth(_T("12")) / 2, 58, _T("12"));*/

	maohao[0].pos.x = maohao[1].pos.x = 320;
	maohao[0].pos.y = 332;
	maohao[1].pos.y = 388;

	shu[0].s[0].a.x = shu[0].s[0].b.x = shu[0].s[4].a.x = shu[0].s[4].b.x = 176;
	shu[0].s[1].a.x = shu[0].s[3].a.x = shu[0].s[5].a.x = 183;
	shu[0].s[1].b.x = shu[0].s[3].b.x = shu[0].s[5].b.x = 225;
	shu[0].s[2].a.x = shu[0].s[2].b.x = shu[0].s[6].a.x = shu[0].s[6].b.x = 232;

	shu[1].s[0].a.x = shu[1].s[0].b.x = shu[1].s[4].a.x = shu[1].s[4].b.x = 250;
	shu[1].s[1].a.x = shu[1].s[3].a.x = shu[1].s[5].a.x = 257;
	shu[1].s[1].b.x = shu[1].s[3].b.x = shu[1].s[5].b.x = 299;
	shu[1].s[2].a.x = shu[1].s[2].b.x = shu[1].s[6].a.x = shu[1].s[6].b.x = 306;

	shu[2].s[0].a.x = shu[2].s[0].b.x = shu[2].s[4].a.x = shu[2].s[4].b.x = 334;
	shu[2].s[1].a.x = shu[2].s[3].a.x = shu[2].s[5].a.x = 341;
	shu[2].s[1].b.x = shu[2].s[3].b.x = shu[2].s[5].b.x = 383;
	shu[2].s[2].a.x = shu[2].s[2].b.x = shu[2].s[6].a.x = shu[2].s[6].b.x = 390;

	shu[3].s[0].a.x = shu[3].s[0].b.x = shu[3].s[4].a.x = shu[3].s[4].b.x = 408;
	shu[3].s[1].a.x = shu[3].s[3].a.x = shu[3].s[5].a.x = 415;
	shu[3].s[1].b.x = shu[3].s[3].b.x = shu[3].s[5].b.x = 457;
	shu[3].s[2].a.x = shu[3].s[2].b.x = shu[3].s[6].a.x = shu[3].s[6].b.x = 464;



	shu[0].s[1].a.y = shu[0].s[1].b.y = shu[1].s[1].a.y = shu[1].s[1].b.y = shu[2].s[1].a.y = shu[2].s[1].b.y = shu[3].s[1].a.y = shu[3].s[1].b.y = 304;

	shu[0].s[0].a.y = shu[0].s[2].a.y = shu[1].s[0].a.y = shu[1].s[2].a.y = shu[2].s[0].a.y = shu[2].s[2].a.y = shu[3].s[0].a.y = shu[3].s[2].a.y = 311;
	shu[0].s[0].b.y = shu[0].s[2].b.y = shu[1].s[0].b.y = shu[1].s[2].b.y = shu[2].s[0].b.y = shu[2].s[2].b.y = shu[3].s[0].b.y = shu[3].s[2].b.y = 353;

	shu[0].s[3].a.y = shu[0].s[3].b.y = shu[1].s[3].a.y = shu[1].s[3].b.y = shu[2].s[3].a.y = shu[2].s[3].b.y = shu[3].s[3].a.y = shu[3].s[3].b.y = 360;

	shu[0].s[4].a.y = shu[0].s[6].a.y = shu[1].s[4].a.y = shu[1].s[6].a.y = shu[2].s[4].a.y = shu[2].s[6].a.y = shu[3].s[4].a.y = shu[3].s[6].a.y = 367;
	shu[0].s[4].b.y = shu[0].s[6].b.y = shu[1].s[4].b.y = shu[1].s[6].b.y = shu[2].s[4].b.y = shu[2].s[6].b.y = shu[3].s[4].b.y = shu[3].s[6].b.y = 409;

	shu[0].s[5].a.y = shu[0].s[5].b.y = shu[1].s[5].a.y = shu[1].s[5].b.y = shu[2].s[5].a.y = shu[2].s[5].b.y = shu[3].s[5].a.y = shu[3].s[5].b.y = 416;

	draw_number();

	dot a, b;

	int l;
	for (int i = 0; i < 360; i += 6) {
		if (i % 90 == 0) {
			l = 15;
			a.x = 320 + (int)(100 * sin((double)i / 180 * pi));
			a.y = 155 - (int)(100 * cos((double)i / 180 * pi));
			b.x = 320 + (int)((100 - l)*sin((double)i / 180 * pi));
			b.y = 155 - (int)((100 - l)*cos((double)i / 180 * pi));
			line_(a, b, 3, 0xffffff);
		}
		else if (i % 30 == 0) {
			l = 10;
			a.x = 320 + (int)(100 * sin((double)i / 180 * pi));
			a.y = 155 - (int)(100 * cos((double)i / 180 * pi));
			b.x = 320 + (int)((100 - l)*sin((double)i / 180 * pi));
			b.y = 155 - (int)((100 - l)*cos((double)i / 180 * pi));
			line_(a, b, 3, 0xdddddd);
		}
		else {
			l = 5;
			a.x = 320 + (int)(100 * sin((double)i / 180 * pi));
			a.y = 155 - (int)(100 * cos((double)i / 180 * pi));
			b.x = 320 + (int)((100 - l)*sin((double)i / 180 * pi));
			b.y = 155 - (int)((100 - l)*cos((double)i / 180 * pi));
			line_(a, b, 2, 0xdddddd);
		}
	}

	circle__(320, 155, 6, 0xbbbbbb);

	circle_(320, 155, 100);
	drawpixx(215, 50, 425, 260);



	//_getch();				// 按任意键继续
	//return 0;
}

void shizhen(double k) {
	double q, w, qq;
	dot a, b;
	int ll = 7, lr = 45;
	w = k / 12;
	w = w * pi * 2;
	qq = sin(w);
	q = ll * qq;
	a.x = 320 - (int)q;
	q = lr * qq;
	b.x = 320 + (int)q;

	qq = cos(w);
	q = ll * qq;
	a.y = 155 + (int)q;
	q = lr * qq;
	b.y = 155 - (int)q;

	lines.shi.a.x = a.x;
	lines.shi.a.y = a.y;
	lines.shi.b.x = b.x;
	lines.shi.b.y = b.y;

	int ss = (int)k % 24;
	change_num(0, ss / 10);
	change_num(1, ss % 10);

	line_(a, b, 5, 0xccccff);
}

void fenzhen(double k) {
	double q, w, qq;
	dot a, b;
	int ll = 10, lr = 65;
	w = k / 60;
	w = w * pi * 2;
	qq = sin(w);
	q = ll * qq;
	a.x = 320 - (int)q;
	q = lr * qq;
	b.x = 320 + (int)q;

	qq = cos(w);
	q = ll * qq;
	a.y = 155 + (int)q;
	q = lr * qq;
	b.y = 155 - (int)q;

	lines.fen.a.x = a.x;
	lines.fen.a.y = a.y;
	lines.fen.b.x = b.x;
	lines.fen.b.y = b.y;

	int ss = (int)k % 60;
	change_num(2, ss / 10);
	change_num(3, ss % 10);

	line_(a, b, 3, 0x99cccc);
}

void miaozhen(int k) {
	double q, w, qq;
	dot a, b;
	int ll = 13, lr = 75;
	w = (double)k / 60;
	w = w * pi * 2;
	qq = sin(w);
	q = ll * qq;
	a.x = 320 - (int)q;
	q = lr * qq;
	b.x = 320 + (int)q;

	qq = cos(w);
	q = ll * qq;
	a.y = 155 + (int)q;
	q = lr * qq;
	b.y = 155 - (int)q;

	lines.miao.a.x = a.x;
	lines.miao.a.y = a.y;
	lines.miao.b.x = b.x;
	lines.miao.b.y = b.y;

	if (k % 2 == 1) {
		maohao[0].color = maohao[1].color = 0x4763ff;
	}
	else {
		maohao[0].color = maohao[1].color = 0x222272;
	}

	line_(a, b, 2, 0xffffff);
	draw_number();
}

void shizhen_() {
	line_clear(lines.shi.a, lines.shi.b, 7);
}

void fenzhen_() {
	line_clear(lines.fen.a, lines.fen.b, 5);
}

void miaozhen_() {
	line_clear(lines.miao.a, lines.miao.b, 4);
}

bool flag = false;

void drawnew(tm t, time_t star, time_t last, time_t now) {
	reco a;
	int temp = (int)(now - star);
	a.h = t.tm_hour + temp / 3600;
	a.m = t.tm_min + (temp / 60) % 60;
	a.s = t.tm_sec + temp % 60;

	double bb, nn, mm;

	if (!flag) {
		flag = true;
		time_las.h = a.h;
		time_las.m = a.m;
		time_las.s = a.s;
	}
	else {
		shizhen_();
		fenzhen_();
		miaozhen_();
	}

	bb = (double)a.s / 60.0;
	mm = a.m + bb;
	bb = mm / 60.0;
	nn = a.h + bb;

	shizhen(nn);
	fenzhen(mm);
	miaozhen(a.s);

}

int main()
{
	struct tm t;
	time_t now, last, star;
	time(&now);
	localtime_s(&t, &now);    //获取当地时间

	initgraph(640, 480);      //图形方式初始化
	init();                   //自定义图形初始化函数，用于绘制时钟界面

	star = now;

	while (!_kbhit()) {
		last = now;
		time(&now);
		if (last != now) {
			drawnew(t, star, last, now);
		}
	}
	_getch();                 //按任意键准备退出时钟程序
	closegraph();             //退出图形界面
	return 0;
}