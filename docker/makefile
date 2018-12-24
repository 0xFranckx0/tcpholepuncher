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

--no-blank-lines-after-declarations \#-nbad
--blank-lines-after-procedures \#-bap 
--no-blank-lines-after-commas \#-nbc 
--braces-on-if-line \#-br
--braces-on-struct-decl-line \#-brs
--comment-indentation33 \#-c33
--declaration-comment-columnn33 \#-cd33
--no-comment-delimiters-on-blank-lines \#-ncdb
--cuddle-else \#-ce
--continuation-indentation4 \#-ci4
--case-indentation10 \#-cli0
--line-comments-indentation0 \#-d0
--declaration-indentation1 \#-di1
--dont-format-first-column-comments \#-nfc1
--indent-level8 \#-i8
--parameter-indentation0 \#-ip0
--line-length80 \#-l80
--break-after-boolean-operator \#-nbbo
--ignore-newlines \#-nhnl
--continue-at-parentheses \#-lp
--no-space-after-function-call-names \#-npcs
--no-space-after-parentheses \#-nprs
--space-after-if \#-sai
--space-after-for \#-saf
--space-after-while \#-saw
--no-space-after-casts \#-ncs
--dont-star-comments \#-nsc
--swallow-optional-blank-lines \#-sob
--dont-format-comments \#-nfca
--else-endif-column33 \#-cp33
--space-special-semicolon \#-ss
--tab-size8 \#-ts8
--indent-label1 \#-il1
--no-tabs \#-nut
--break-function-decl-args \#-bfda
