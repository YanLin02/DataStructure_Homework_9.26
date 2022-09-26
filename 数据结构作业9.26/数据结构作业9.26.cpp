
#include <string>
#include <iostream>

using namespace std;

void FailureFunction(const string& str, int f[]) {
	int LengthP = str.length();
	f[0] = -1;
	for (size_t j = 1; j < LengthP; j++)
	{
		int i = f[j - 1];
		while ((str[j] != str[i + 1]) && (i >= 0))i = f[i];
		if (str[j] == str[i + 1])f[j] = i + 1;
		else f[j] = -1;
	}
}

int FastFind(const string& str, const string& pat) {
	int i = 0, j = 0;
	int LengthS = str.length(), LengthP = pat.length();
	int* f = new int[LengthP];
	FailureFunction(pat, f);
	while ((j < LengthP) && (i < LengthS))
	{
		if (pat[j] == str[i])
		{
			i++;
			j++;
		}
		else
		{
			if (j == 0)i++;
			else j = f[j - 1] + 1;
		}
	}
	delete[]f;
	if ((j < LengthP) || (LengthP == 0))return-1;
	else return i - LengthP;
}

int main()
{
	string test("1341119991215");
	string pat("12");
	cout << FastFind(test, pat);






	return 0;
}