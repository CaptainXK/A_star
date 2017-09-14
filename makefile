.PHONY:test,clean

test.app:main.cpp
	@g++ main.cpp -o test.app -g
test:test.app
	@$(EXEC) ./test.app
clean:
	@rm *.app
