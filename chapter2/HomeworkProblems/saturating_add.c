// 2022/6/24
// zhangzhong
// Homework Problem 2.73

// Addition that saturates to TMin or TMax
#include <assert.h>
#include <limits.h>
#include <stdio.h>

int saturating_add(int x, int y)
{
	int sign_mask = INT_MIN;
	int sum = x + y;
	
	// (x & sign_mask): x < 0
	// !(x & sign_mask): x >= 0
	// if x > 0, y > 0, x + y < 0, positive overflow
	int positive_overflow = 0;
	!(x & sign_mask) && !(y & sign_mask) && (sum & sign_mask) && (positive_overflow = 1);
	// if x < 0, y < 0, x + y >= 0, negative overflow
	int negative_overflow = 0;
	(x & sign_mask) && (y & sign_mask) && !(sum & sign_mask) && (negative_overflow = 1);

	positive_overflow && (sum = INT_MAX);
	negative_overflow && (sum = INT_MIN);
	return sum;
}

int tadd_ok(int x, int y)
{
	if (x > 0 && y > 0 && x + y < 0)
	{
		return 1;
	}
	else if (x < 0 && y < 0 && x + y >= 0)
	{
		return -1;
	}
	return 0;
}

int saturating_add_normal(int x, int y)
{
	int r = tadd_ok(x, y);
	if (r > 0)
	{
		return INT_MAX;
	}
	else if (r < 0)
	{
		return INT_MIN;
	}
	return x + y;
}

void test_saturating_add()
{
	int y = 0;
	for (int x = INT_MIN; x < INT_MAX; ++x)
	{
		if (saturating_add(x, y) != saturating_add_normal(x, y))
		{
			printf("saturating_add(%d, %d) = %d, saturating_add_normal(%d, %d) = %d\n",
				x, y, saturating_add(x, y), x, y, saturating_add_normal(x, y));
			return;
		}
	}
	assert(saturating_add(INT_MAX, y) == saturating_add_normal(INT_MAX, y));

	y = INT_MIN;
	for (int x = INT_MIN; x < INT_MAX; ++x)
	{
		if (saturating_add(x, y) != saturating_add_normal(x, y))
		{
			printf("saturating_add(%d, %d) = %d, saturating_add_normal(%d, %d) = %d\n",
				x, y, saturating_add(x, y), x, y, saturating_add_normal(x, y));
			return;
		}
	}
	assert(saturating_add(INT_MAX, y) == saturating_add_normal(INT_MAX, y));

	y = INT_MAX;
	for (int x = INT_MIN; x < INT_MAX; ++x)
	{
		if (saturating_add(x, y) != saturating_add_normal(x, y))
		{
			printf("saturating_add(%d, %d) = %d, saturating_add_normal(%d, %d) = %d\n",
				x, y, saturating_add(x, y), x, y, saturating_add_normal(x, y));
		}
		return;
	}
	assert(saturating_add(INT_MAX, y) == saturating_add_normal(INT_MAX, y));
}

int main(int argc, char* argv[])
{
	test_saturating_add();
}