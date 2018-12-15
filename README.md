# allstar
Tools and scripts related to the Allstar Ham Radio Linking capability

To install or update these tools on an Arch Linux-based Allstar node (circa 2018)
---------------------------------------------------------------------------------

1) Log into your Allstar node, and drop to the BASH shell

IF THIS IS THE FIRST TIME DOING ALL OF THIS (aka "starting from scratch") -----
2A) cd /root
3A) git clone https://github.com/ka9cql/allstar.git

IF YOU'VE DONE SOME OF THIS BEFORE, and only need to update your code to the latest -
2B) cd /root/allstar
3B) git pull

4) pacman -S lynx         (NOTE: If it says "already installed", or "already up-to-date", you can answer "n" (NO), when asked if you want to re-install it)
5) pacman -S graphviz     (NOTE 1of2: Same as above - you don't have to re-install it if it is already installed.)
                          (NOTE 2of2: If it gives you a choice of where to grab this package from, you can accept the default)
6) cd /root/allstar
7) chmod 755 astar bubbles getdb txlog
8) cd /usr/local/bin
9) ln -s /root/allstar/astar .      (It's OK if it says "file already exists")
10) ln -s /root/allstar/bubbles .   (It's OK if it says "file already exists")
11) ln -s /root/allstar/getdb .     (It's OK if it says "file already exists")
12) ln -s /root/allstar/txlog .     (It's OK if it says "file already exists")
13) cd /srv/http/supermon
14) cp /root/allstar/AllstarBubbleDiagram.html .      (Answer "Y" (YES) to overwrite file if it already exists)
15) cp /root/allstar/AllstarBubbleDiagram2.html .     (Answer "Y" (YES) to overwrite file if it already exists)
16) cd /root/allstar
17) getdb
18) bubbles
19) txlog             (NOTE: LEAVE THIS PROCESS  RUNNING! - It is the one that updates the bubble diagram in real-time!)
20) Open a web browser to this URL (substitute your Allstar node's IP address for the one shown here) - http://192.168.1.20/supermon/AllstarBubbleDiagram2.html
21) Watch the web page automatically change when someone keys-up, unkeys, connects or disconnects.  The page will also change if one of the connected nodes reports its statistics in the background (happens sometimes) and that report "looks weird" to my scripts.
21) To compare this drawing with that from Allstarlink.org, try the "AllstarBubbleDiagram.html" webpage (NOTE: Unlike the one in step 20, this webpage doesn't have a "2" in its name)

NOTES:
--------------
*  Your Allstar node must have Internet connectivity in order to use the above procedure
*  "txlog" is the process that updates the diagram mentioned in steps 20/21. If "txlog" stops, so will live updates of that diagram.
*  If you want to keep "txlog" running in the background even if you exit the shell or log off of your node, try this -

    A) cd /root
    B) (nohup /root/allstar/txlog >> /root/txlog.log 2>>/root/txlog.err) &

* To run this software on any "non-Allstar" (or, more correctly, any non-Arch Linux-based Allstar) system, try the following changes -

    A) Substitute your Linux distribution's package manager (yum, apt-get, etc) in steps 4 and 5
    B) Substitute your webserver's content-source directory (/var/www/html, etc.) in step 13, and use a corresponding URL in step 20

- Mike, KA9CQL
