#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define N_D 216555
char *dic[N_D];

#define L 87

uint8_t c1[] = {
	0x15, 0x0c, 0x50, 0x5d, 0x2f, 0x4c, 0x1c, 0x42,
	0x4e, 0x24, 0x1f, 0x09, 0x11, 0x06, 0x0a, 0x11,
	0x46, 0x5b, 0x01, 0x03, 0x48, 0x11, 0x1d, 0x47,
	0x0e, 0x48, 0x06, 0x13, 0x02, 0x08, 0x0d, 0x00,
	0x41, 0x54, 0x04, 0x0e, 0x45, 0x1e, 0x01, 0x00,
	0x02, 0x45, 0x03, 0x1b, 0x01, 0x49, 0x0c, 0x5f,
	0x1b, 0x00, 0x10, 0x1b, 0x1a, 0x4f, 0x03, 0x05,
	0x0a, 0x53, 0x5c, 0x1d, 0x01, 0x45, 0x1a, 0x2b,
	0x3a, 0x2c, 0x49, 0x1b, 0x0d, 0x09, 0x19, 0x15,
	0x16, 0x14, 0x0d, 0x00, 0x4f, 0x0b, 0x1b, 0x1a,
	0x23, 0x3d, 0x67, 0x12, 0x06, 0x1a, 0x1d,
};

uint8_t c2[] = {
	0x11, 0x0c, 0x54, 0x5f, 0x63, 0x05, 0x1b, 0x42,
	0x59, 0x23, 0x17, 0x59, 0x13, 0x1b, 0x1a, 0x43,
	0x41, 0x56, 0x44, 0x09, 0x0e, 0x45, 0x1a, 0x12,
	0x02, 0x09, 0x1b, 0x46, 0x0a, 0x13, 0x0d, 0x1c,
	0x15, 0x43, 0x45, 0x57, 0x0c, 0x1e, 0x49, 0x0d,
	0x12, 0x06, 0x0d, 0x04, 0x10, 0x1a, 0x41, 0x5e,
	0x0c, 0x06, 0x55, 0x04, 0x06, 0x58, 0x14, 0x10,
	0x53, 0x15, 0x52, 0x01, 0x45, 0x0a, 0x19, 0x21,
	0x68, 0x39, 0x0c, 0x00, 0x18, 0x08, 0x0c, 0x50,
	0x10, 0x1a, 0x59, 0x01, 0x06, 0x0a, 0x07, 0x00,
	0x28, 0x23, 0x22, 0x45, 0x4f, 0x54, 0x59,
};

uint8_t a[L];

char p1[L];
char p2[L];

static inline int min(int a, int b)
{
	return a < b ? a : b;
}

static void read_dic(void)
{
	int i;
	FILE *dic_file = fopen("dictionary", "r");

	for (i = 0; i < N_D; ++i)
		fscanf(dic_file, "%ms", &dic[i]);
	fclose(dic_file);
}

static bool match(const char *p1, const char *p2, const uint8_t *a, int n)
{
	int i;

	for (i = 0; i < n; ++i) {
		uint8_t x = p1[i] ^ p2[i];
		if (a[i] != x)
			return false;
	}
	return true;
}

static bool find_match(const char *w, int n)
{
	int a = 0, b = N_D;

	while (a + 1 < b) {
		int c = (a + b) / 2;
		int cmp = strncmp(w, dic[c], n);

		if (cmp < 0)
			b = c;
		else if (cmp > 0)
			a = c;
		else if (dic[c][n])
			b = c;
		else
			return true;
	}
	return strncmp(w, dic[a], n) == 0 && dic[a][n] == 0;
}

static void find_matching_range(const char *w, int n, int *first, int *last)
{
	int a = 0, b = N_D;

	while (a + 1 < b) {
		int c = (a + b) / 2;
		int cmp = strncmp(w, dic[c], n);

		if (cmp < 0)
			b = c;
		else if (cmp > 0)
			a = c;
		else {
			for (a = c - 1; a >= 0; --a)
				if (strncmp(w, dic[a], n))
					break;
			*first = a + 1;
			for (++c; c < b; ++c)
				if (strncmp(w, dic[c], n))
					break;
			*last = c;
			return;
		}
	}
	if (strncmp(w, dic[a], n)) {
		*first = 0;
		*last = 0;
	} else {
		*first = a;
		*last = b;
	}
}

static void try(char *p1, int n1, char *p2, int n2, int l)
{
	static int oldmin;
	int i, j, k;

	assert(n1 >= n2 && n1 <= l);
	if (min(n1, n2) > oldmin) {
		oldmin = min(n1, n2);
		printf("Candidates:\n"
		       "[%.*s]\n"
		       "[%.*s]\n", n1, p1, n2, p2);
	}
	if (n1 == n2) {
		for (i = 0; i < N_D; ++i) {
			int l1 = strlen(dic[i]);

			if (n1 + l1 > l)
				continue;
			memcpy(p1 + n1, dic[i], l1);
			try(p1, n1 + l1, p2, n2, l);
		}
	} else {
		int first, last;

		for (i = 0; i < n1 - n2; ++i) {
			int l2 = i + 1;

			p2[n2 + i] = p1[n2 + i] ^ a[n2 + i];
			if (find_match(p2 + n2, l2))
				if (n2 + l2 <= n1)
					try(p1, n1, p2, n2 + l2, l);
				else
					try(p2, n2 + l2, p1, n1, l);
		}

		find_matching_range(p2 + n2, n1 - n2, &first, &last);

		for (i = first; i < last; ++i) {
			int l2 = strlen(dic[i]);

			if (n2 + l2 > l)
				continue;
			memcpy(p2 + n2, dic[i], l2);
			if (n2 + l2 <= n1)
				try(p1, n1, p2, n2 + l2, l);
			else
				try(p2, n2 + l2, p1, n1, l);
		}
	}
}

int main()
{
	int i;

	read_dic();


	{
		int i;
		for (i = 0; i < N_D; ++i)
			if (!find_match(dic[i], strlen(dic[i])))
				abort();
	}
	{
		int i, j;
		for (i = 0; i < N_D; ++i) {
			int a, b, n = strlen(dic[i]);
			find_matching_range(dic[i], n, &a, &b);
			for (j = a; j < b; ++j) {
				assert(strlen(dic[j]) >= n);
				assert(strncmp(dic[i], dic[j], n) == 0);
			}
			assert(a == 0 || strncmp(dic[i], dic[a - 1], n) != 0);
			assert(b == N_D || strncmp(dic[i], dic[b], n) != 0);
		}
	}

	for (i = 0; i < L; ++i)
		a[i] = c1[i] ^ c2[i];
	try(p1, 0, p2, 0, L);
}
