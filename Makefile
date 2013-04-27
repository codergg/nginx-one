
default:	build

clean:
	rm -rf Makefile objs

build:
	$(MAKE) -f objs/Makefile
	$(MAKE) -f objs/Makefile manpage

install:
	$(MAKE) -f objs/Makefile install

upgrade:
	/home/zhang/nginx/nginx/bin/nginx -t

	kill -USR2 `cat /home/zhang/nginx/nginx/bin/logs/nginx.pid`
	sleep 1
	test -f /home/zhang/nginx/nginx/bin/logs/nginx.pid.oldbin

	kill -QUIT `cat /home/zhang/nginx/nginx/bin/logs/nginx.pid.oldbin`
