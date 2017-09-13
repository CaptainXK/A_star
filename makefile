.PHONY:test,clean

test.app:main.cpp
	@g++ main.cpp -o test.app -g
test:
	@$(EXEC) ./test.app
clean:
	@rm *.app
