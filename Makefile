SERVER: netfileserver.c
	gcc -lpthread -lm -g -fsanitize=address -fno-omit-frame-pointer netfileserver.c -o SERVER
clean: 
	rm SERVER
