#include <dirent.h>
#include <stdio.h>


void showfile(char* direct){
	DIR *d;
	struct dirent *dir;
	d = opendir(direct);
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			printf("%s\n", dir->d_name);
		}
		closedir(d);
	}
}


int main(void)
{
	showfile("Stock");
	return(0);

}
