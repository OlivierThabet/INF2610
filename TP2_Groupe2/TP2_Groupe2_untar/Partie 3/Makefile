TP_NAME1=part3
TP_NAME2=part3_update

C_FILES1=merge_sort.c
C_FILES2=merge_sort_upgrade.c

C_FLAGS=-O2 -std=gnu17 -pthread
C_FLAGS+=-Wall -Wno-unused-result -Wno-unused-variable
C_FLAGS+=-Werror

.PHONY: all clean

all: $(TP_NAME1) $(TP_NAME2)

$(TP_NAME1): $(C_FILES1)
	gcc $(C_FLAGS) -o $@ $^

$(TP_NAME2): $(C_FILES2)
	gcc $(C_FLAGS) -o $@ $^

clean:
	rm -f $(TP_NAME1) $(TP_NAME2) *.o

HANDIN_ARCHIVE=handin.tar.gz

handin:
	@echo "Creating archive $(HANDIN_ARCHIVE) with all .c and .txt files..."
	@find . -name "*.c" -o -name "*.txt" | tar -czf $(HANDIN_ARCHIVE) -T -
	@echo "$(HANDIN_ARCHIVE) has been created."

clean_handin:
	@echo "Cleaning up handin archive..."
	@rm -f $(HANDIN_ARCHIVE)
	@echo "$(HANDIN_ARCHIVE) has been removed."
