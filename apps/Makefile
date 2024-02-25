ASM=nasm
ALL: clean hello_world snake

hello_world:
	$(ASM) hello_world.asm -o build/hello_world.bin

snake:
	$(ASM) snake.asm -o build/snake.bin

clean:
	rm -rf build/*
