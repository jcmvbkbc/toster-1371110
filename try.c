#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define N_D 216558
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

uint8_t c3[] = {
	0x0a, 0x05, 0x4b, 0x48, 0x63, 0x08, 0x1a, 0x05,
	0x0d, 0x21, 0x07, 0x14, 0x00, 0x07, 0x4f, 0x5e,
	0x44, 0x56, 0x16, 0x46, 0x19, 0x10, 0x1b, 0x04,
	0x04, 0x48, 0x17, 0x14, 0x00, 0x12, 0x06, 0x52,
	0x07, 0x5f, 0x1d, 0x57, 0x12, 0x02, 0x00, 0x03,
	0x12, 0x45, 0x11, 0x00, 0x0d, 0x1d, 0x18, 0x10,
	0x0b, 0x0c, 0x57, 0x57, 0x1f, 0x5c, 0x12, 0x1a,
	0x53, 0x11, 0x48, 0x09, 0x1f, 0x45, 0x05, 0x31,
	0x26, 0x3e, 0x08, 0x16, 0x48, 0x02, 0x00, 0x06,
	0x01, 0x55, 0x17, 0x00, 0x0e, 0x0b, 0x16, 0x16,
	0x64, 0x23, 0x22, 0x1d, 0x06, 0x1a, 0x1e,
};

uint8_t a[L];
uint8_t b[L];

char p1[L];
char p2[L];
char p3[L];

static void read_dic(void)
{
	int i;
	FILE *dic_file = fopen("dictionary", "r");

	dic[0] = " ";
	for (i = 1; i < N_D; ++i)
		fscanf(dic_file, "%ms", &dic[i]);
	fclose(dic_file);
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

static bool represent(const char *str, int n)
{
	int i, a, b;

	assert(n > 0);
	find_matching_range(str, n, &a, &b);
	if (a != b) {
		return true;
	}
	for (i = 1; i < n; ++i)
		if (find_match(str, i) && represent(str + i, n - i)) {
			return true;
		}
	return false;
}

static void xorstr(char *out, const char *p, const uint8_t *c, int n)
{
	int i;
	for (i = 0; i < n; ++i)
		out[i] = c[i] ^ p[i];
}

static void print_str(uint8_t *p, int n)
{
	int i;

	for (i = 0; i < n; ++i)
		printf("%02x ", p[i]);
	printf("\n");
}

static void try2(int n, int l)
{
	int i;

	if (1) {
		printf("p1 = %.*s\n"
		       "p2 = %.*s\n"
		       "p3 = %.*s\n", n, p1, n, p2, n, p3);
	}
	for (i = 0; i < N_D; ++i) {
		int li = strlen(dic[i]);

		if (n + li > l)
			continue;
		memcpy(p1 + n, dic[i], li);
		if (n + li < l) {
			p1[n + li] = ' ';
			++li;
		}
		xorstr(p2 + n, p1 + n, a + n, li);
		if (!represent(p2, n + li))
			continue;
		xorstr(p3 + n, p1 + n, b + n, li);
		if (!represent(p3, n + li))
			continue;
		try2(n + li, l);
	}
}

static void self_check(void)
{
	int i, j;
	char s[] = "the quick brown fox jumps over a lazy dog";

	for (i = 0; i < N_D; ++i)
		if (!find_match(dic[i], strlen(dic[i])))
			abort();

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

	assert(represent(s, strlen(s)));
}

int main()
{
	int i;

	read_dic();
	self_check();

	for (i = 0; i < L; ++i) {
		a[i] = c1[i] ^ c2[i];
		b[i] = c1[i] ^ c3[i];
	}
	try2(0, L);
}
