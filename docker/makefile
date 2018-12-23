.PHONY:	build compile run stop punch

SRC=/root/puncher
IMAGE = builder
TAG = 1-0
TESTDIR=/opt/thp/tests/.libs
INSTALLBINDIR=/usr/local/bin

TESTPROGR?=unittests
INSTALLPROGR?=punchctl

build:	
	sudo docker build -t $(IMAGE):$(TAG) .

compile:
	sudo docker exec -i $(IMAGE) /opt/build.sh

run: stop 
	sudo docker run --rm -d -i --name $(IMAGE) --net=host -v ${PWD}/../:/opt/thp $(IMAGE):$(TAG)

stop:
	-sudo docker stop $(IMAGE) 

punch: 
	sudo docker exec -i -t $(IMAGE) /usr/local/bin/punchctl
	sudo docker exec -i -t $(IMAGE) /opt/thp/tests/setuptest

leaks-test:
	# Can be used by passing the program name by the command line
	# make leaks-test TESTPROGR=unittests
	sudo docker exec -t -i $(IMAGE) valgrind --leak-check=full --show-reachable=yes $(TESTDIR)/$(TESTPROGR)
leaks-bin:		
	# Can be used by passing the program name by the command line
	# make leaks-bin INSTALLPROGR=puncher
	sudo docker exec -t -i $(IMAGE) valgrind --leak-check=full --show-reachable=yes -v $(INSTALLBINDIR)/$(INSTALLPROGR)

debugger-test:
	# Can be used by passing the program name by the command line
	# make debugger-test INSTALLPROGR=puncher
	sudo docker exec -t -i $(IMAGE) cgdb $(TESTDIR)/$(TESTPROGR)

indent:
	#indent -st -bap -bli0 -i4 -l79 -ncs -npcs -npsl -fca -lc79 -fc1 -ts4 $(SRCFILE)
	indent -nbad -bap -nbc -bbo -hnl -br -brs -c33 -cd33 -ncdb -ce -ci4 -cli0 -d0 -di1 -nfc1 -i8 -ip0 -l80 -lp -npcs -nprs -sai -saf -saw -ncs -nsc -sob -nfca -cp33 -ss -ts8 -il1

-nbad --no-blank-lines-after-declarations
-bap --blank-lines-after-procedures
-nbc --no-blank-lines-after-commas
-br -braces-on-if-line
-brs --braces-on-struct-decl-line
-c33 --comment-indentation 33
-cd33 --declaration-comment-columnn 33
-ncdb --no-comment-delimiters-on-blank-lines
-ce 
-ci4 
-cli0 
-d0 
-di1 
-nfc1 
-i8 
-ip0 
-l80 
-nbbo 
-nhnl 
-lp 
-npcs 
-nprs 
-sai 
-saf 
-saw 
-ncs 
-nsc 
-sob 
-nfca 
-cp33 
-ss 
-ts8 
-il1 
-nut 
-bfda 
-lp