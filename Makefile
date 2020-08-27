FLAG=-Werror
STD=-std=gnu99
CC=gcc

all:
	$(CC) $(STD) $(FLAG) client.c tools.c -o client
	$(CC) $(STD) $(FLAG) server.c tools.c -o server
	$(CC) $(STD) $(FLAG) login.c tools.c -o login
	$(CC) $(STD) $(FLAG) open.c tools.c -o open
	$(CC) $(STD) $(FLAG) save.c tools.c -o save
	$(CC) $(STD) $(FLAG) take.c tools.c -o take
	$(CC) $(STD) $(FLAG) transf.c tools.c -o transf
	$(CC) $(STD) $(FLAG) select.c tools.c -o select
	$(CC) $(STD) $(FLAG) change.c tools.c -o change 
	$(CC) $(STD) $(FLAG) destory.c tools.c -o destory
	$(CC) $(STD) $(FLAG) unlock.c tools.c -o unlock	

clean:
	rm client server login open save take transf select change destory unlock
