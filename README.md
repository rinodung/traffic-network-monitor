## Overview
Network Traffic Monitor is an application to monitor traffice all nodes in a network and display with a visualization interfaces.



## How to install

### Environment
    Ubuntu 14.04
    MongoDB 3.2
    
### Sniffer at target node

    $ sudo apt-get install libpcap-dev
    $ sudo cd sniffer
    $ sudo make
    $ sudo ./sniffer
### Receiver at Monitor 

    $ sudo apt-get install libpcap-dev
    $ sudo cd receiver
    $ sudo make
    $ sudo ./receiver
### Show Data   
https://network-traffic-monitor-rinodung.c9users.io

### Test data:
https://network-traffic-monitor-rinodung.c9users.io/receive/getTestData


## Support & Documentation

Visit http://docs.c9.io for support, or to learn more about using Cloud9 IDE. 
To watch some training videos, visit http://www.youtube.com/user/c9ide
## Install MongoDB
```
sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv EA312927
echo "deb http://repo.mongodb.org/apt/ubuntu trusty/mongodb-org/3.2 multiverse" | sudo tee /etc/apt/sources.list.d/mongodb-org-3.2.list
sudo apt-get install -y mongodb-org

Add smallfiles = true end of  /etc/mongod.conf
sudo service mongod start
OR
mongod --dbpath /data/db --smallfiles
```
## Export to json
```
mongoexport --db network_monitor --collection traffic --limit 500 --out traffic.json --jsonArray
```

http://stackoverflow.com/questions/14584393/why-getting-error-mongod-dead-but-subsys-locked-and-insufficient-free-space-for

Fail on Docker
I forgot one important detail: I tried installing inside a Docker container with an ubuntu 14.04 base image. Cause for the problem is the way docker messes up the init system. I was able to install using the following commands. So this ticket can be closed.
RUN dpkg-divert --local --rename --add /sbin/initctl
RUN ln -s /bin/true /sbin/initctl
RUN dpkg-divert --local --rename --add /etc/init.d/mongod
RUN ln -s /bin/true /etc/init.d/mongod
RUN \
apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv EA312927 && \
echo 'deb http://repo.mongodb.org/apt/ubuntu trusty/mongodb-org/3.2 multiverse' > /etc/apt/sources.list.d/mongodb.list && \
apt-get update && \
apt-get install -yq mongodb-org
https://jira.mongodb.org/browse/SERVER-21812
## Install MongoDB C Driver
``` 
    $ sudo apt-get update
    $ sudo apt-get install pkg-config libssl-dev libsasl2-dev
    $ sudo apt-get install git gcc automake autoconf libtool
    $ git clone https://github.com/mongodb/mongo-c-driver.git
    $ cd mongo-c-driver
    $ git checkout x.y.z  # To build a particular release
    $ ./autogen.sh --with-libbson=bundled
    $ make
    $ sudo make install
```      
Add /usr/local/lib to /etc/ld.so.conf file
Build file:  gcc -o receiver receiver.c $(pkg-config --cflags --libs libmongoc-1.0)
Run file: sudo ./receiver


## Insall MongoDB PHP Perl
```
sudo apt-get install php-pear php5-dev
sudo pecl install mongo

# Ubuntu 14.04
sudo vi /etc/php5/apache2/conf.d/mongo.ini

Add line: extension=mongo.so
```
## MongoDB Controller 
Detail: http://www.tutorialspoint.com/mongodb/index.htm
```
// Run MongoDb Server(--dbpath ~/application/data/db)
$ mongod --dbpath /data/db --smallfiles

//Connect to MongoDB Server
$ mongo

//Use MongoDB 
$ help;
$ show dbs;
$ use network_monitor;
$ show colecctions;
$ db.createCollection("traffic");
$ db.traffic.find();
$ db.traffic.find().pretty();
$ db.traffic.count();
$ db.traffic.insert({"ip_src": "127.0.0.1", "port_src": 80, "ip_dest": "127.0.0.2", "port_dest": "80", "time": "1473405353"});
$ db.traffic.remove({"ip_src": "127.0.0.1", "port_src": 80, "ip_dest": "127.0.0.2", "port_dest": "80"});
$ db.traffic.remove({});
```

## Install Json-c
```
$ git clone https://github.com/json-c/json-c.git
$ cd json-c
$ sh autogen.sh

$ ./configure
$ make
$ make install

OR
sudo apt-get install libjson0 libjson0-dev
```
## Edit link getJson
edit link getJson in /assets/keylight.js line 316




[{ "_id" : "57e29cb1eb0fdd0fea4b5086", "ip_src" : "127.0.0.1", "ip_dest" : "10.240.0.222", "port_src" : "51511", "port_dest" : "7891", "protocol" : "udp", "count" : "2", "time" : 1474469036, "position":{"x":15.240511885906038,"y":293.36268613689884}}{ "_id" : "57e29cb7eb0fdd0fea4b508d", "ip_src" : "10.240.0.222", "ip_dest" : "127.0.0.1", "port_src" : "22", "port_dest" : "51090", "protocol" : "tcp", "count" : "7", "time" : 1474469043, "position":{"x":725.5440990615924,"y":198.58707620302724}}]
