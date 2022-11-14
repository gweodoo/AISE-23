#include <stdio.h>


int main(int argc, char ** argv){


	if(argc != 3)
	{
		return 1;
	}
	else {
		FILE * fdin = fopen(argv[1], "r");

		if(!fdin){
			perror("fopen");
			return 1;
		}

		FILE * fdout = fopen(argv[2], "w");
		
		if(!fdout){
			perror("fopen");
			return 1;
		}

		char buff[500];
		int cnt;
		while( (cnt = fread(buff, sizeof(char),
						    500, fdin)) != 0 )
		{

			if(cnt < 0)
			{
				perror("fread");
				return 1;
			}

			int ret = fwrite(buff, sizeof(char),
					cnt, fdout);

			if(ret < 0)
			{
				perror("fwrite");
				return 1;
			}

		}



		fclose(fdin);
		fclose(fdout);
	}

	return 0;
}



