all:
	gcc -O0 -std=gnu11 -Werror -Wall -Wno-unused-result -Wno-unused-variable -o processlab ./libprocesslab/libprocesslab.c  processes_part1.c processes_part2.c ./libprocesslab/processlab.c -lrt 

mac:
	gcc -O0 -std=gnu11 -Werror -Wall -Wno-unused-result -Wno-unused-variable -o processlab ./libprocesslab/libprocesslab.c  processes_part1.c processes_part2.c ./libprocesslab/processlab.c

handin:
	mkdir handin
	cp ../Section2_Processes/processes_part1.c handin/
	cp ../Section2_Processes/processes_part2.c handin/
	cp ../Section2_Processes/processes_output.txt handin/
	tar -czvf handin.tar.gz handin
	rm -rf handin

clean:
	rm -f processlab
