psnotify: psnotify.c
	gcc -Wall -g psnotify.c -o psnotify

test:
	# Run it with "ps" (despite dynamic behaviour) to test popen()+ps
	./psnotify -e "ps -eo pid=,ppid=,user=,args=" "sshd"
	@echo "*** Test 01 **********************************"
	make test01
	@echo "*** Test 02 **********************************"
	make test02
	@echo "*** Test 03 **********************************"
	make test03
	@echo "*** Test 04 **********************************"
	make test04

test01:
	./psnotify -e "cat ./input01.txt" "sshd" > output01.txt
	cat output01.txt
	-diff -w -s output01.txt expected01.txt

test02:
	./psnotify -e "cat ./input02.txt" "login" > output02.txt
	cat output02.txt
	-diff -w -s output02.txt expected02.txt

test03:
	# NOTE:  Only matching the query to the *first* field of the command
	#	Therefore, missing PID=846 "startups, which is OK for now.
	# Matching the expected03.txt output meets the specification.
	#	We will augment this functionality in Assignment #2
	./psnotify -e "cat ./input02.txt" "ps" > output03.txt
	cat output03.txt
	-diff -w -s output03.txt expected03.txt

test04:
	./psnotify -e "cat ./input02.txt" "scsi" > output04.txt
	cat output04.txt
	-diff -w -s output04.txt expected04.txt

clean:
	-rm psnotify
	-rm output*txt

capture:
	ps -eo pid=,ppid=,user=,args= > out.X
