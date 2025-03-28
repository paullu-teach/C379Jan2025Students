# Variables.  127.0.0.1 is "localhost", which is OK for basic testing.
#	You can replace with other IPs, for further testing.
IP1 = 127.0.0.1
IP2 = 127.0.0.1

# Be careful of these ports.  Two students using the same lab machine
#	can run into a port conflict.  To solve, just change the following
#	numbers accordingly.  Also, if your program exits due to a bug
#	you might see a port already in use error.  Wait a couple of minutes
#	and run again.
PORT1 = 8000
PORT2 = 8081
PORT3 = 8082
PORT4 = 8083

# Set these paths and use testssh/Test00
SSH_PATH = /usr/bin/ssh
REMOTE_AGENT = /home/ubuntu/Assn02Rel01/remoteagent
INPUT1 = /home/ubuntu/Assn02Rel01/commoninput/input01.txt
INPUT2 = /home/ubuntu/Assn02Rel01/commoninput/input02.txt

all: testnow

# Test00:  Not for marks, but make sure this test works first.
#	Basic tests of SSH functionality and pathnames to files
#	M
testssh:
	@echo "************** Testing basic SSH functionality..."
	ls -l $(SSH_PATH)
	$(SSH_PATH) ubuntu@$(IP1) pwd
	$(SSH_PATH) ubuntu@$(IP2) pwd
	@echo "************** Testing path to input files..."
	$(SSH_PATH) ubuntu@$(IP1) ls -l $(INPUT1)
	$(SSH_PATH) ubuntu@$(IP1) ls -l $(INPUT2)
	$(SSH_PATH) ubuntu@$(IP2) ls -l $(INPUT1)
	$(SSH_PATH) ubuntu@$(IP2) ls -l $(INPUT2)
	@echo "************** Testing path to remoteagent..."
	@echo "    ********** May fail if executable does not exist yet..."
	$(SSH_PATH) ubuntu@$(IP1) ls -l $(REMOTE_AGENT)
	$(SSH_PATH) ubuntu@$(IP2) ls -l $(REMOTE_AGENT)

testnow:
	make distpsnotify
	make remoteagent
	make test01
	make test02
	make test03
	make test04

distpsnotify: distpsnotify.c
	gcc distpsnotify.c -g -o distpsnotify

remoteagent: remoteagent.c
	gcc remoteagent.c -g -o remoteagent


expected01: template_expected01.txt
	sed 's/<IP1>/$(IP1)/g; s/<IP2>/$(IP2)/g' template_expected01.txt > expected01.txt

expected02: template_expected02.txt
	sed 's/<IP1>/$(IP1)/g; s/<IP2>/$(IP2)/g' template_expected02.txt > expected02.txt

expected03: template_expected03.txt
	sed 's/<IP1>/$(IP1)/g; s/<IP2>/$(IP2)/g' template_expected03.txt > expected03.txt

expected04: template_expected04.txt
	sed 's/<IP1>/$(IP1)/g; s/<IP2>/$(IP2)/g' template_expected04.txt > expected04.txt


test01: distpsnotify expected01
	./distpsnotify -e "cat $(INPUT1)" -a "$(REMOTE_AGENT)" -s "$(SSH_PATH)" -r "$(IP1)" -q "bash" -n 3 -p $(PORT1) > output01.txt
	cat output01.txt
	-diff -w -s output01.txt expected01.txt

test02: distpsnotify expected02
	./distpsnotify -e "cat $(INPUT1)" -a "$(REMOTE_AGENT)" -s "$(SSH_PATH)" -r "$(IP2)" -r "$(IP1)" -q "bash" -n 2 -p $(PORT2) > output02.txt
	cat output02.txt
	-diff -w -s output02.txt expected02.txt

test03: distpsnotify expected03
	./distpsnotify -e "cat $(INPUT1)" -a "$(REMOTE_AGENT)" -s "$(SSH_PATH)" -r "$(IP1)" -r "$(IP2)" -q "bash" -n 2 -p $(PORT3) > output03.txt
	cat output03.txt
	-diff -w -s output03.txt expected03.txt

test04: distpsnotify expected04
	./distpsnotify -e "cat $(INPUT2)" -a "$(REMOTE_AGENT)" -s "$(SSH_PATH)" -r "$(IP1)" -r "$(IP2)" -q "bash" -n 2 -p $(PORT4) > output04.txt
	cat output04.txt
	-diff -w -s output04.txt expected04.txt


clean:
	-rm -f distpsnotify
	-rm -f remoteagent
	-rm expected*txt
	-rm output*txt

tar:
	(cd ..; tar cvfz ~/assn02c379.tar.gz Assn02/Makefile Assn02/distpsnotify.* Assn02/remoteagent.*)
