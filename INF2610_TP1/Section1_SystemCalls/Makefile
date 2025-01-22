all:
	gcc -o systemcalls_part1 systemcalls_part1.c && gcc -o systemcalls_part2 systemcalls_part2.c

part1: systemcalls_part1.c
	gcc systemcalls_part1.c -o systemcalls_part1

part2: systemcall_part2.c
	gcc systemcalls_part2.c -o systemcalls_part2

handin:
	mkdir handin
	cp ../Section1_SystemCalls/systemcalls_part1.c handin/
	cp ../Section1_SystemCalls/systemcalls_part2.c handin/
	cp ../Section1_SystemCalls/systemcalls_output1.txt handin/
	cp ../Section1_SystemCalls/systemcalls_output2.txt handin/
	tar -czvf handin.tar.gz handin
	rm -rf handin

clean:
	rm -f *.o systemcalls_part1 systemcalls_part2
