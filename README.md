# security-fundamentals
Fundamental of Security Autumn 2018

Installation guide

Instructions to install openssl developer libraries:
Follow the commands below through a Terminal in the VM image provided:
Get the library:
➢ wget http://www.openssl.org/source/openssl-1.0.1g.tar.gz
Extract The library:
➢ tar -xvzf openssl-1.0.1g.tar.gz
Go into folder:
➢ cd openssl-1.0.1g
Configure Openssl:
➢ ./config --prefix=/usr/local/openssl --openssldir=/usr/local/openssl
Compile:
➢ sudo make
Install openssl:
➢ sudo make install
Install openssl development libraries:
➢ sudo apt-get install libssl-dev
