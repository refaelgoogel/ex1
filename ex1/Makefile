CC = gcc
FLAGS = -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG
END_FLAGS = -lm -g
OBJS = CourseQueue.o Hacker.o HackerEnrollment.o IsraeliQueue.o Item.o main.o Student.o 
EXEC = IsraeliQueue
TESTFILES = C:\Users\refae\Desktop\mtm\ex1\ExampleTest\Tests\Tests\Test12\students.txt \
	C:\Users\refae\Desktop\mtm\ex1\ExampleTest\Tests\Tests\Test12\courses.txt \
	C:\Users\refae\Desktop\mtm\ex1\ExampleTest\Tests\Tests\Test12\hackers.txt \
	C:\Users\refae\Desktop\mtm\ex1\ExampleTest\Tests\Tests\Test12\queues.txt \
	C:\Users\refae\Desktop\mtm\ex1\ExampleTest\Tests\Tests\Test12\out.txt
OPTIONAL_FLAG = -i

#.PHONY: program clean

#program: $(EXEC)
#	.\$(EXEC) $(TESTFILES)
	
#clean: 
#	rm -f $(OBJECTS) $(EXECUTABLE)

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
