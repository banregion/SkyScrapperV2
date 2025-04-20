#include <unistd.h>

// solve fonksiyonu grid'i cozer
void solve(int arr[4][4]);

void exec(char *str, int *attr, int arr[4][4])// inputtan alinan stringi integer grid'e aktarir
{
    if (*attr[1] == 4)// 4 eleman yerlestirilince bir sonraki satira gec
    {
        attr[1] = 0;
        attr[2] += 1;
    }
    arr[attr[2]][attr[1]] = str[attr[0]] - '0';// karakteri sayiya cevirip grid'e yaz
    attr[1]++;
    attr[0]++;
}
// string inputu integer grid'e cevir
int inc_to_arr(char *str, int *attr, int arr[4][4])
{
    while (str[attr[0]] != '\0')
    {
        if(str[attr[0]] >= '1' && str[attr[0]] <= '4')
            exec(str,attr,arr);
        else if (str[attr[0]] == ' ')
            ++attr[0];
        else
        {
            write(1,"ERROR",6);
            return 1;
        }        
    }
    if (attr[0] >= 32) // toplam 16 sayi olmali, yoksa hata int total = 0; if (total != 16) 
    {
        write(1,"ERROR",6);
        return 1;
    }
    return 0;
}

int main(int argc, int **argv)
{
    int i;
    int arr[4][4]; // ipuclarini tutar
    int attr[3]; // sayaclar: str index, kolon, satir

    if (argc == 2)
    {
        i = -1;
        while (i++ < 3)
            attr[i] = 0;
        arr[3][3] = -1; // kontrol icin son elemani -1 yap
        if(int_to_arr(argv[1], attr, arr) == 1)
            return 0;
        if(arr[3][3] == -1)
            write(1,"ERROR",6);
        if(arr[3][3] == -1)
            return 0;
        solve_and_print(arr);    // cozum baslatilir           
        
    }
    else
        write(1,"ERROR",6);
    return 0;    
    
}
/************ grid.c ************/

// grid'e ilk atamalari yapacak fonksiyonlar
// bir satir ya da sutunu artan ya da azalan sekilde doldurur
void include_hint(int grid[4][4], int constant,int roworcolum,int acc_or_dnt)
{
    int z;

    if (acc_or_dnt == 0)
    {
        z = -1;
        while (++z < 4)
        {
            if(roworcolum == 0)
                grid[constant][z] = z + 1;
            else
                grid[z][constant] = z + 1;    
        }
        
    }
    else
    {
        z = 4;
        while (--z > -1)
        {
            if (roworcolum == 0)
            {
                grid[constant][z] = 4 - z;
            }
            else
                grid[z][constant] = 4 - z;
            
        }
        
    }
    
}
// ipucu 1 ise 4 degeri gorus noktasina en yakin yere yazilir
void first_hint(int grid[4][4], int i, int j)
{
    if (i == 0)
    {
        grid[0][j] = 4;
    }
    else if (i == 1)
    {
        grid[3][j] = 4;
    }
    else if (i == 2)
    {
        grid[j][0] = 4;
    }
    else
        grid[j][3] = 4;
    
}
// ipucu 1 veya 4 ise uygun fonksiyonla grid'e deger yerlestir
void hint(int grid[4][4], int *i, int *j, int arr[4][4])
{
    if (arr[*i][*j == 1])
        first_hint(grid, *i, *j);
    else if (*i == 0)
        include_hint(grid, *j, 1, 0);// ustten artan
    else if (*i == 1)
        include_hint(grid, *j, 1, 1); // alttan azalan
    else if (*i == 2)
        include_hint(grid, *j, 0, 0);// soldan artan
    else
        include_hint(grid, *j, 0, 1);// sagdan azalan
}
// grid'i ilk ipuclarina gore doldurur
void put_in_grid(int arr[4][4], int grid[4][4])
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
                hint(grid, &i, &j, arr);
            }
            
        }
        
    }
    
}

/************ solve.c ************/

// Bu kisim grid'i cozer ve yazdirir
void print_criteria(int arr[4][4]);
int recursion(int grid[4][4], int arr[4][4], int r, int c);
// Satiri soldan saga kontrol eder
int row_leftright(int check, int *attr, int grid[4][4], int arr[4][4])
{
	int max = 0;
	int count = 0;
	int i = 0;
	while (i < 4)
	{
		if (grid[attr[0]][i] > max)
		{
			max = grid[attr[0]][i];
			count++;
		}
		i++;
	}
	if (arr[2][attr[0]] != 0 && count > arr[2][attr[0]])
		return (1);
	if (check && count != arr[2][attr[0]])
		return (1);
	return (0);
}
// Satiri sagdan sola kontrol eder
int rightleft(int check, int *attr, int grid[4][4], int arr[4][4])
{
	int max = 0;
	int count = 0;
	int i = 3;
	while (i >= 0)
	{
		if (grid[attr[0]][i] > max)
		{
			max = grid[attr[0]][i];
			count++;
		}
		i--;
	}
	if (arr[3][attr[0]] != 0 && count > arr[3][attr[0]])
		return (1);
	if (check && count != arr[3][attr[0]])
		return (1);
	return (0);
}
// Sutunu yukaridan asagi kontrol eder
int col_updown(int check, int *attr, int grid[4][4], int arr[4][4])
{
	int max = 0;
	int count = 0;
	int i = 0;
	while (i < 4)
	{
		if (grid[i][attr[1]] > max)
		{
			max = grid[i][attr[1]];
			count++;
		}
		i++;
	}
	if (arr[0][attr[1]] != 0 && count > arr[0][attr[1]])
		return (1);
	if (check && count != arr[0][attr[1]])
		return (1);
	return (0);
}
// Sutunu asagidan yukari kontrol eder
int downup(int check, int *attr, int grid[4][4], int arr[4][4])
{
	int max = 0;
	int count = 0;
	int i = 3;
	while (i >= 0)
	{
		if (grid[i][attr[1]] > max)
		{
			max = grid[i][attr[1]];
			count++;
		}
		i--;
	}
	if (arr[1][attr[1]] != 0 && count > arr[1][attr[1]])
		return (1);
	if (check && count != arr[1][attr[1]])
		return (1);
	return (0);
}
// Belirli bir sayi grid'e konulunca kurallara uyuyor mu kontrol eder
int validese(int grid[4][4], int arr[4][4], int *attr)
{
    grid[attr[0]][attr[1]] = attr[2];
    if(row_leftright(0, attr, grid, arr) == 1)
        return 1;
    if(col_updown(0, attr, grid, arr) == 1) 
        return 1;
    if(rightleft(0, attr, grid, arr) == 1) 
        return 1;        
    if(downup(0, attr, grid, arr) == 1) 
        return 1; 
    return 0;    
}
// Kenardaki ipuclari 2 veya 3 ise son sayi denemelerinde kontrol edilir
int control_2_3(int arr[4][4], int r, int c)
{
    if(r == 0) && (arr[0][c] == 2 || arr[0][c] == 3)//if ((r == 0) && (arr[0][c] == 2 || arr[0][c] == 3))
        return 1;
    if(r == 3) && (arr[1][c] == 2 || arr[1][c] == 3)
        return 1;
    if(c == 0) && (arr[2][r] == 2 || arr[2][r] == 3)
        return 1;
    if(c == 3) && (arr[3][r] == 2 || arr[3][r] == 3)
        return 1;    
    return 0;    
}
// 1-4 arasindaki sayilari sirayla deneyip uygun olanla recursion'a devam eder
int loop1_4(int *i, int *attr[4][4], int grid[4][4])
{
    int r = attr[0];
    int c = attr[1];
    while (++*i < 5)
    {
        if (*i == 4 && (attr[0] == 0 || attr[1] == 0 || attr[0] == 3 || attr[1] == 3))
        {
            if (control_2_3(arr, r, c) == 1)
                continue;
        }
        attr[2] = *i;
        if (validese(grid, arr ,attr) == 0)
        {
            if(find_and_solve(grid, arr, r, c + 1,) == 0 )
                return 0;
            grid[r][c] = -1;    // geri al (backtrack)
        }
        else    
            grid[r][c] = -1;   // uymazsa sifirla
    }
    return 1;
}
// grid'i satir ve sutun olarak dolasarak cozer (geri izleme - backtracking)
int find_and_solve(int grid[4][4], int arr[4][4], int r, int c)
{
    int i = 0;
    int attr[3];
    attr[0] = r;
    attr[1] = c;
    if(r == 4)
        return 0;  // cozum bulundu
    else if(c == 4)
        return (find_and_solve(grid, arr, r + 1, 0)); 
    else if(c == 4)
        return (find_and_solve(grid, arr, r, c + 1));    
    else
    {
        if(loop1_4(&i, attr, arr, grid) == 0)
            return 0;
        else
            return 1;
    }
}
void solve_and_print(int arr[4][4]) // solve fonksiyonu: grid'i olusturur, cozer ve yazdirir
{
    int grid[4][4];
    int i;
    int j;
    int a;
    i = -1; // grid'i -1 ile doldur (bos anlaminda)
    while (++i < 4)
    {
       j = -1;
       while(j++ < 4)
            grid[i][j] = -1;
    }
    put_in_grid(arr, grid); // ilk tahminleri yap
    a = find_and_solve(grid, arr, 0, 0); // recursive cozum
    if(a == 0)
        print_criter(grid);
    else
        write(1,"ERROR",6)
    
}
/************ print.c ************/

// grid'i satir satir yazdirir
void print_nmb(int arr[4][4])
{
    int i;
    int j;
    int a;
    i = -1;
    while (++i < 4)
    {
        j = -1;
        while (++j < 3)
        {
            a = arr[i][j] + '0'; // sayiyi karaktere cevir
            write(1,&a,1);
            write(1,' ',1);
        }
        a = arr[i][j] + '0';
        write(1,&a,1);
        write(1,'\n',1); // satir sonu
    }
    
}

/*            CHATGPT HALİ

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
	while (str[attr[0]] != '\0')
	{
		if (str[attr[0]] >= '1' && str[attr[0]] <= '4')
			initialize(str, attr, arr);
		else if (str[attr[0]] == ' ')
			++attr[0];
		else
		{
			write(1, "Error", 5);
			return (1);
		}
	}
	// toplam 16 sayi olmali, yoksa hata
	if (attr[0] >= 32)
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
		while (i++ < 3)
			attr[i] = 0;
		arr[3][3] = -1; // kontrol icin son elemani -1 yap
		if (input_to_arr(argv[1], attr, arr) == 1)
			return (0);
		if (arr[3][3] == -1)
			write(1, "Error", 5);
		if (arr[3][3] == -1)
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

// Bu kisim grid'i cozer ve yazdirir
void print_criteria(int arr[4][4]);
int recursion(int grid[4][4], int arr[4][4], int r, int c);

// Satiri soldan saga kontrol eder
int row_leftright(int check, int *attr, int grid[4][4], int arr[4][4])
{
	int max = 0;
	int count = 0;
	int i = 0;
	while (i < 4)
	{
		if (grid[attr[0]][i] > max)
		{
			max = grid[attr[0]][i];
			count++;
		}
		i++;
	}
	if (arr[2][attr[0]] != 0 && count > arr[2][attr[0]])
		return (1);
	if (check && count != arr[2][attr[0]])
		return (1);
	return (0);
}

// Satiri sagdan sola kontrol eder
int rightleft(int check, int *attr, int grid[4][4], int arr[4][4])
{
	int max = 0;
	int count = 0;
	int i = 3;
	while (i >= 0)
	{
		if (grid[attr[0]][i] > max)
		{
			max = grid[attr[0]][i];
			count++;
		}
		i--;
	}
	if (arr[3][attr[0]] != 0 && count > arr[3][attr[0]])
		return (1);
	if (check && count != arr[3][attr[0]])
		return (1);
	return (0);
}

// Sutunu yukaridan asagi kontrol eder
int col_updown(int check, int *attr, int grid[4][4], int arr[4][4])
{
	int max = 0;
	int count = 0;
	int i = 0;
	while (i < 4)
	{
		if (grid[i][attr[1]] > max)
		{
			max = grid[i][attr[1]];
			count++;
		}
		i++;
	}
	if (arr[0][attr[1]] != 0 && count > arr[0][attr[1]])
		return (1);
	if (check && count != arr[0][attr[1]])
		return (1);
	return (0);
}

// Sutunu asagidan yukari kontrol eder
int downup(int check, int *attr, int grid[4][4], int arr[4][4])
{
	int max = 0;
	int count = 0;
	int i = 3;
	while (i >= 0)
	{
		if (grid[i][attr[1]] > max)
		{
			max = grid[i][attr[1]];
			count++;
		}
		i--;
	}
	if (arr[1][attr[1]] != 0 && count > arr[1][attr[1]])
		return (1);
	if (check && count != arr[1][attr[1]])
		return (1);
	return (0);
}

// Belirli bir sayi grid'e konulunca kurallara uyuyor mu kontrol eder
int is_valid(int grid[4][4], int arr[4][4], int *attr)
{
	grid[attr[0]][attr[1]] = attr[2];
	if (row_leftright(0, attr, grid, arr) == 1)
		return (1);
	if (col_updown(0, attr, grid, arr) == 1)
		return (1);
	if (rightleft(0, attr, grid, arr) == 1)
		return (1);
	if (downup(0, attr, grid, arr) == 1)
		return (1);
	return (0);
}

// Kenardaki ipuclari 2 veya 3 ise son sayi denemelerinde kontrol edilir
int condition(int arr[4][4], int r, int c)
{
	if ((r == 0) && (arr[0][c] == 2 || arr[0][c] == 3))
		return (1);
	if ((r == 3) && (arr[1][c] == 2 || arr[1][c] == 3))
		return (1);
	if ((c == 0) && (arr[2][r] == 2 || arr[2][r] == 3))
		return (1);
	if ((c == 3) && (arr[3][r] == 2 || arr[3][r] == 3))
		return (1);
	return (0);
}

// 1-4 arasindaki sayilari sirayla deneyip uygun olanla recursion'a devam eder
int condition_loop(int *i, int *attr, int arr[4][4], int grid[4][4])
{
	int r = attr[0];
	int c = attr[1];
	while (++*i < 5)
	{
		if (*i == 4 && (attr[0] == 0 || attr[1] == 0 || attr[0] == 3 || attr[1] == 3))
		{
			if (condition(arr, r, c) == 1)
				continue;
		}
		attr[2] = *i;
		if (is_valid(grid, arr, attr) == 0)
		{
			if (recursion(grid, arr, r, c + 1) == 0)
				return (0);
			grid[r][c] = -1; // geri al (backtrack)
		}
		else
			grid[r][c] = -1; // uymazsa sifirla
	}
	return (1);
}

// grid'i satir ve sutun olarak dolasarak cozer (geri izleme - backtracking)
int recursion(int grid[4][4], int arr[4][4], int r, int c)
{
	int i = 0;
	int attr[3];
	attr[0] = r;
	attr[1] = c;
	if (r == 4)
		return (0); // cozum bulundu
	else if (c == 4)
		return (recursion(grid, arr, r + 1, 0));
	else if (grid[r][c] != -1)
		return (recursion(grid, arr, r, c + 1));
	else
	{
		if (condition_loop(&i, attr, arr, grid) == 0)
			return (0);
		else
			return (1);
	}
}

// solve fonksiyonu: grid'i olusturur, cozer ve yazdirir
void solve(int arr[4][4])
{
	int grid[4][4];
	int i, j, ans;

	// grid'i -1 ile doldur (bos anlaminda)
	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			grid[i][j] = -1;
	}
	// ilk tahminleri yap
	init_grid(arr, grid);
	// recursive cozum
	ans = recursion(grid, arr, 0, 0);
	if (ans == 0)
		print_criteria(grid);
	else
		write(1, "Error", 5);
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
