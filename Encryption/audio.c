#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <fcntl.h>
#include <signal.h>

int isfile(char* path)
{
	FILE * fichier = fopen(path, "r+");
 
	if (fichier == NULL)
{
    	//	printf(" Le fichier n'existe pas ");
		return 0;
}
	else
{
    	//	printf(" Le fichier existe et on le referme aussitôt");
    		fclose(fichier);
		return 1;
}
	}

void encode ()
{	system("ffmpeg -i Test.mp3 -f segment -segment_time 0.4 -c copy out%01d.mp3");
//	system("mkdir truc");
//	system("mv out* truc");
	int s = 0;
	char y[1000] = "out";
	char s2[1000];
	sprintf(s2,"%d",s);
	char* k = y;
	k = strcat(k, s2);
	k = strcat(k, ".mp3");
	while (isfile(k) == 1)
	{
		char y[1000] = "out";
		k = y;
		s = s + 1;
		sprintf(s2,"%d",s);

		k = strcat(k,s2);

		k = strcat(k, ".mp3");

		}

  int i = 0;
  int tot = 0;
//  char s2[1000];
  char s3[1000];
  while (i != s)
  {
	  if (s%2 == 0)
		  tot = (2*i + 3)%(s+1);
	  else
	  	  tot = (2*i + 3)%s;
	  char j[1000] = "mv out";
	  char* k = j;
	  sprintf(s2,"%d",i);
	  k = strcat(k,s2);
	  k = strcat(k, ".mp3 ");
	  sprintf(s3,"%d",tot);
	  k = strcat(k,s3);
	  k = strcat(k,".mp3");
	  system(k);
	  i = i + 1;
	  }
//	system("rename 's/\\d+/sprintf(\"%05d\", $&)/e' truc/*.mp3");
//	system("cat *.mp3 > encode.mp3");

	}

void decode ()
{
	//system("ffmpeg -i encode.mp3 -f segment -segment_time 1 -c copy out%01d.mp3");
	system("mkdir truc");
	system("mv *.mp3 truc");
	int s = 5;
	char y[1000] = "truc/";
	char s2[1000];
	sprintf(s2,"%d",s);
	char* l = y;
	l = strcat(l, s2);
	l = strcat(l, ".mp3");
	while (isfile(l) == 1)
	{
		char y[1000] = "truc/";
		l = y;
		s = s + 1;
		sprintf(s2,"%d",s);

		l = strcat(l,s2);

		l = strcat(l, ".mp3");

		}

	int i = 0;
	int tot = 0;
	system("mkdir final");
	char j[1000] = "cp truc/";
	char* k = j;
//	char s2[1000];
	char s3[1000];
	if (s%2 == 0)
	{
		tot = (i-2+s)/2;
		sprintf(s2, "%d",i);
		k = strcat(k,s2);
		k = strcat(k, ".mp3 final/");
		sprintf(s3,"%d",tot);
		k = strcat(k,s3);
		k = strcat(k,".mp3");
		system(k);
		i = i + 2;
		while(i != (s+1))
		{
			if (i%2 == 0)
			{
				tot = (i - 2 + s)/2;
			}
			else
			{
				tot = (i - 3)/2;
				if (tot < 0)
				{
					tot = tot + s;
					}

				}	
			char j[1000] = "cp truc/";
			k = j;
			sprintf(s2, "%d",i);
			k = strcat(k,s2);
			k = strcat(k, ".mp3 final/");
			sprintf(s3,"%d",tot);
			k = strcat(k,s3);
			k = strcat(k,".mp3");
			system(k);
			i = i +1;

			}
		}
	else
	{
		while(i != s)
		{
			if (i%2 == 0)
			{
				tot = (i - 2 + s)/2;
			}
			else
			{
				tot = (i - 3)/2;
				if (tot < 0)
				{
					tot = tot + s;
					}

				}	
			char j[1000] = "cp truc/";
			k = j;
			sprintf(s2, "%d",i);
			k = strcat(k,s2);
			k = strcat(k, ".mp3 final/");
			sprintf(s3,"%d",tot);
			k = strcat(k,s3);
			k = strcat(k,".mp3");
			system(k);
			i = i +1;
		}
		}
	system("rename 's/\\d+/sprintf(\"%05d\", $&)/e' final/*.mp3");
	system("cat final/*.mp3 > decode.mp3");
	system("rm -r final");
	system("rm -r truc");


}


