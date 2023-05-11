CC = gcc
FLAGS = -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG
END_FLAGS = -lm -g
OBJS = CourseQueue.o Hacker.o HackerEnrollment.o IsraeliQueue.o Item.o main.o Student.o 
EXEC = IsraeliQueue

OPTIONAL_FLAG = -i

BASE_PATH := /home/refael.hadad/Matam/ex1/ex1/ExampleTest/Tests/Tests/Test
STUDENTS := /students.txt
COURSES := /courses.txt
HACKERS := /hackers.txt
QUEUES := /queues.txt
OUT := /out.txt

TEST_RANGE := $(shell seq 1 16)

.PHONY: program clean


$(EXEC): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(EXEC) $(END_FLAGS)

CourseQueue.o: CourseQueue.c CourseQueue.h Student.h IsraeliQueue.h Hacker.h
	$(CC) -c CourseQueue.c CourseQueue.h Student.h IsraeliQueue.h Hacker.h

Hacker.o: Hacker.c Hacker.h Student.h
	$(CC) -c Hacker.c Hacker.h Student.h

HackerEnrollment.o: HackerEnrollment.c HackerEnrollment.h CourseQueue.h Student.h IsraeliQueue.h Hacker.h Item.h
	$(CC) -c HackerEnrollment.c HackerEnrollment.h CourseQueue.h Student.h IsraeliQueue.h Hacker.h Item.h

IsraeliQueue.o: IsraeliQueue.c Item.h IsraeliQueue.h Student.h
	$(CC) -c IsraeliQueue.c Item.h IsraeliQueue.h Student.h

Item.o: Item.c Item.h
	$(CC) -c Item.c Item.h

main.o: main.c HackerEnrollment.h CourseQueue.h Student.h IsraeliQueue.h Hacker.h
	$(CC) -c main.c HackerEnrollment.h CourseQueue.h Student.h IsraeliQueue.h Hacker.h

Student.o: Student.c Student.h
	$(CC) -c Student.c Student.h


program: $(EXEC)
	@for i in $(TEST_RANGE); do \
		./$(EXEC) $(BASE_PATH)$$i$(STUDENTS) $(BASE_PATH)$$i$(COURSES) $(BASE_PATH)$$i$(HACKERS) $(BASE_PATH)$$i$(QUEUES) $(BASE_PATH)$$i$(OUT); \
	done

	
clean: 
	rm -f $(OBJECTS) $(EXECUTABLE)
