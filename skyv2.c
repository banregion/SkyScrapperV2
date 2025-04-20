#include <unistd.h>

// solve fonksiyonu grid'i cozer
void solve(int arr[4][4]);

// inputtan alinan stringi integer grid'e aktarir
void initialize(char *str, int *attr, int arr[4][4])
{
	// 4 eleman yerlestirilince bir sonraki satira gec
	if (attr[1] == 4)
	{
		attr[1] = 0;
		attr[2] += 1;
	}
	// karakteri sayiya cevirip grid'e yaz
	arr[attr[2]][attr[1]] = str[attr[0]] - '0';
	attr[1]++;
	attr[0]++;
}

// string inputu integer grid'e cevir
int input_to_arr(char *str, int *attr, int arr[4][4])
{
	int count = 0;
	while (str[attr[0]] != '\0')
	{
		if (str[attr[0]] >= '1' && str[attr[0]] <= '4')
		{
			initialize(str, attr, arr);
			count++;
		}
		else if (str[attr[0]] == ' ')
			++attr[0];
		else
		{
			write(1, "Error", 5);
			return (1);
		}
	}
	// tam olarak 16 sayi olmali
	if (count != 16)
	{
		write(1, "Error", 5);
		return (1);
	}
	return (0);
}

// programin giris noktasi
int main(int argc, char **argv)
{
	int i;
	int arr[4][4];     // ipuclarini tutar
	int attr[3];       // sayaclar: str index, kolon, satir

	if (argc == 2)
	{
		i = -1;
		while (i++ < 2)
			attr[i] = 0;
		if (input_to_arr(argv[1], attr, arr) == 1)
			return (0);
		solve(arr); // cozum baslatilir
	}
	else
		write(1, "Error", 5);
	return (0);
}

/************ grid.c ************/

// grid'e ilk atamalari yapacak fonksiyonlar

// bir satir ya da sutunu artan ya da azalan sekilde doldurur
void inc_asign(int grid[4][4], int constant, int roworcolumn, int inc_or_dec)
{
	int z;
	if (inc_or_dec == 0) // artan
	{
		z = -1;
		while (++z < 4)
		{
			if (roworcolumn == 0)
				grid[constant][z] = z + 1;
			else
				grid[z][constant] = z + 1;
		}
	}
	else // azalan
	{
		z = 4;
		while (--z > -1)
		{
			if (roworcolumn == 0)
				grid[constant][z] = 4 - z;
			else
				grid[z][constant] = 4 - z;
		}
	}
}

// ipucu 1 ise 4 degeri gorus noktasina en yakin yere yazilir
void asign_one(int grid[4][4], int i, int j)
{
	if (i == 0)
		grid[0][j] = 4;
	else if (i == 1)
		grid[3][j] = 4;
	else if (i == 2)
		grid[j][0] = 4;
	else
		grid[j][3] = 4;
}

// ipucu 1 veya 4 ise uygun fonksiyonla grid'e deger yerlestir
void asign(int grid[4][4], int *i, int *j, int arr[4][4])
{
	if (arr[*i][*j] == 1)
		asign_one(grid, *i, *j);
	else if (*i == 0)
		inc_asign(grid, *j, 1, 0); // ustten artan
	else if (*i == 1)
		inc_asign(grid, *j, 1, 1); // alttan azalan
	else if (*i == 2)
		inc_asign(grid, *j, 0, 0); // soldan artan
	else
		inc_asign(grid, *j, 0, 1); // sagdan azalan
}

// grid'i ilk ipuclarina gore doldurur
void init_grid(int arr[4][4], int grid[4][4])
{
	int i;
	int j;
	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			if (arr[i][j] == 4 || arr[i][j] == 1)
			{
				asign(grid, &i, &j, arr);
			}
		}
	}
}

/************ solve.c ************/

// ... önceki solve.c kodu aynı kalır ...

// Kenardaki ipuclari 2 veya 3 ise son sayi denemelerinde kontrol edilir
int condition(int arr[4][4], int r, int c)
{
	if (r == 0 && (arr[0][c] == 2 || arr[0][c] == 3))
		return (1);
	if (r == 3 && (arr[1][c] == 2 || arr[1][c] == 3))
		return (1);
	if (c == 0 && (arr[2][r] == 2 || arr[2][r] == 3))
		return (1);
	if (c == 3 && (arr[3][r] == 2 || arr[3][r] == 3))
		return (1);
	return (0);
}

/************ print.c ************/

// grid'i satir satir yazdirir
void print_criteria(int arr[4][4])
{
	int i;
	int j;
	char ans;
	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 3)
		{
			ans = arr[i][j] + '0'; // sayiyi karaktere cevir
			write(1, &ans, 1);
			write(1, " ", 1);
		}
		ans = arr[i][j] + '0';
		write(1, &ans, 1);
		write(1, "\n", 1); // satir sonu
	}
}
