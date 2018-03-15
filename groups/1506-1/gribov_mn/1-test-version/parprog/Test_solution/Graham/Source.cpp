#include <stdio.h>
#include <iostream>
#include <time.h> 
using namespace std;

class CPoint
{
private:
	int determ(CPoint q, CPoint w)
	{
		return (q.x * w.y - w.x * q.y);
	}
public:
	int x;
	int y;

	bool operator <(CPoint &temp)
	{
		if ((determ(*this, temp) > 0) || ((determ(*this, temp) == 0) && (x * x + y * y < temp.x * temp.x + temp.y * temp.y)))
			return true;
		return false;
	}

	bool operator >(CPoint &temp)
	{
		if (temp < *this)
			return true;
		return false;
	}

	bool operator !=(CPoint &temp)
	{
		return !(temp == *this);
	}

	bool operator ==(CPoint &temp)
	{
		if ((x == temp.x) && (y == temp.y))
			return true;
		return false;
	}

	CPoint& operator=(CPoint &temp)
	{
		x = temp.x;
		y = temp.y;
		return *this;
	}

	bool operator <=(CPoint &temp)
	{
		if ((determ(*this, temp) > 0) || ((determ(*this, temp) == 0) && (x * x + y * y <= temp.x * temp.x + temp.y * temp.y)))
			return true;
		return false;
	}

	bool operator >=(CPoint &temp)
	{
		if (temp <= *this)
			return true;
		return false;
	}

	CPoint(void)
	{
	}

	CPoint(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	CPoint(CPoint &point)
	{
		x = point.x;
		y = point.y;
	}

	~CPoint(void)
	{
		x = 0;
		y = 0;
	}
};

static int det(CPoint a, CPoint b)
{
	return (a.x * b.y - a.y * b.x);
}

static void QuickSort(CPoint* a, int i, int j)
{
	int i1, j1;
	if (i < j)
	{
		int k = (i + j) / 2;
		CPoint b = a[k];
		i1 = i;
		j1 = j;
		do
		{
			while (a[j1] < b && j1 > i)
				j1--;
			while (a[i1] > b && i1 < j)
				i1++;
			if (i1 <= j1)
			{
				if (i1 < j1)
				{
					CPoint temp = a[i1];
					a[i1] = a[j1];
					a[j1] = temp;
				}
				i1++;
				j1--;
			}
		} while (i1 < j1);
		if (i1 < j)
			QuickSort(a, i1, j);
		if (i < j1)
			QuickSort(a, i, j1);
	}
}

static void Conv(CPoint *a, int n, int &m)
{
	CPoint c = a[0];
	m = 0;
	for (int i = 1; i < n; i++)
		if (a[i].x < c.x)
		{
			c = a[i];
			m = i;
		}
		else
		{
			if (a[i].x == c.x)
				if (a[i].y < c.y)
				{
					c = a[i];
					m = i;
				}
		}

	CPoint temp = a[0];
	a[0] = a[m];
	a[m] = temp;
	m = 0;

	for (int i = 0; i < n; i++)
	{
		a[i].x -= c.x;
		a[i].y -= c.y;
	}

	QuickSort(a, 0, n - 1);

	for (int i = 1; i < n; i++)
		if (a[i] != a[m])
		{
			if (m >= 1)
			{
				while ((m >= 1) && (det(CPoint(a[m].x - a[m - 1].x, a[m].y - a[m - 1].y), CPoint(a[i].x - a[m].x, a[i].y - a[m].y)) >= 0)) m--;
			}
			m++;
			CPoint temp = a[i];
			a[i] = a[m];
			a[m] = temp;
		}
	m++;

	for (int i = 0; i < n; i++)
	{
		a[i].x += c.x;
		a[i].y += c.y;
	}
}

int main()
{
	int n;
	CPoint *mainArrOfPoints = 0;
	int amountOfPoints = 10;
	int linCountConv = 0;
	srand(time(0));
	cout << "Array size: " << amountOfPoints << endl;

	mainArrOfPoints = new CPoint[amountOfPoints];
	for (int i = 0; i < amountOfPoints; i++)
	{
		mainArrOfPoints[i].x = rand() % 100;
		mainArrOfPoints[i].y = rand() % 100;
	}
	for (int i = 0; i < amountOfPoints; i++)
		cout << "(" << mainArrOfPoints[i].x << "," << mainArrOfPoints[i].y << ")" << endl;
	cout << endl;
	Conv(mainArrOfPoints, amountOfPoints, linCountConv);
	for (int i = 0; i < linCountConv; i++)
		cout << "(" << mainArrOfPoints[i].x << "," << mainArrOfPoints[i].y << ")" << endl;
}