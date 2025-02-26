TP_NAME=GuerreDesChiffres

C_FILES=$(wildcard *.c)

C_FLAGS=-O2 -std=gnu17 -pthread
C_FLAGS+=-Wall -Wno-unused-result -Wno-unused-variable
C_FLAGS+=-Werror

.PHONY: all clean

all: $(TP_NAME)

$(TP_NAME): $(C_FILES)
	gcc $(C_FLAGS) -o $@ $^

clean:
	rm -f $(TP_NAME)

HANDIN_ARCHIVE=handin.tar.gz

handin:
	@echo "Creating archive $(HANDIN_ARCHIVE) with all .c and .txt files..."
	@find . -name "*.c" -o -name "*.txt" | tar -czf $(HANDIN_ARCHIVE) -T -
	@echo "$(HANDIN_ARCHIVE) has been created."

clean_handin:
	@echo "Cleaning up handin archive..."
	@rm -f $(HANDIN_ARCHIVE)
	@echo "$(HANDIN_ARCHIVE) has been removed."
