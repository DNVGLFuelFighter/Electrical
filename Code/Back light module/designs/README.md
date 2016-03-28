Using of python scripts
	imageopener.py 
		- its python3 script
		- accepts file as argument (gif and png tested)
		- outputs file for eth sending or file for c int array (currently only setup in code with changing call of the function output_image or output_image_for_eth
			- output_image outputs image to stdout (TODO to file) in const int out_image[] = {}
			- output_image_for_eth outputs image to stdout for ethernet sending
	send.py
		- mbed is currently setup as 
			ip: 	 192.168.1.2
			mask: 	 255.255.255.0
			gateway: 192.168.1.1
		- its python2 script (just because xD)
		- sends data via ethernet 
		- if no arguments it sends predefind picture in src
		- if any argument it sends stdin via ethernet (useful for redirecting output from imageopener.py)

best way to use (in linux terminal):
	python imageopener.py drivelights2.png | python2 send.py 2
