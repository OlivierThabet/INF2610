all:
	gcc -O0 -std=gnu11 -Werror -Wall -Wno-unused-result -Wno-unused-variable -o challenges_part1 ./challenges_part1.c -lrt

mac:
	gcc -O0 -std=gnu11 -Werror -Wall -Wno-unused-result -Wno-unused-variable -o challenges_part1 ./challenges_part1.c

test:
	gcc -O0 -std=gnu11 -Werror -Wall -Wno-unused-result -Wno-unused-variable -c challenges_part2.c -o challenges_part2.o
	gcc -O0 -std=gnu11 -Werror -Wall -Wno-unused-result -Wno-unused-variable -o test_part2 challenges_part2.o test_part2.o


handin:
	mkdir handin
	cp challenges_part1.c handin/
	cp challenges_part2.c handin/
	cp challenges_part1.h handin/
	cp challenges_part2.h handin/
	cp challenges_output.txt handin/
	tar -czvf handin.tar.gz handin
	rm -rf handin

clean:
	rm -f handin.tar.gz challenges_part2.o challenges_part1 challenges_part2 test_part2