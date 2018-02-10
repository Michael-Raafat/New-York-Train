HEADERS = caltrain.h
OBJECTS = caltrain.o caltrain-runner.o

default: train

%.o: %.c $(HEADERS)
	gcc -c $< -o $@ 

train: $(OBJECTS)
	gcc $(OBJECTS) -o $@  -pthread

clean:
	-rm -f $(OBJECTS)
	-rm -f train
