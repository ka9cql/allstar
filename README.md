# allstar
Tools and scripts related to the Allstar Ham Radio Linking capability

To install or update these tools on an Arch Linux-based Allstar node (circa 2018)
---------------------------------------------------------------------------------

1) Log into your Allstar node, and drop to the BASH shell

# IF THIS IS THE FIRST TIME DOING ALL OF THIS (aka "starting from scratch") -
2. cd /root
3. git clone https://github.com/ka9cql/allstar.git
4. Continue with 5, below


# IF YOU'VE DONE SOME OF THIS BEFORE, and only need to update your code to the latest -
2. cd /root/allstar
3. git pull
4. Continue with 5, below


Continuing...
--------------
5) pacman -S lynx         (NOTE: If it says "already installed", or "already up-to-date", you can answer "n" (NO), when asked if you want to re-install it)
6) pacman -S graphviz     (NOTE 1of2: Same as above - you don't have to re-install it if it is already installed.)
                          (NOTE 2of2: If it gives you a choice of where to grab this package from, you can accept the default)
7) cd /root/allstar
8) chmod 755 astar bubbles getdb txlog
9) cd /usr/local/bin
10) ln -s /root/allstar/astar .     (It's OK if it says "file already exists")
11) ln -s /root/allstar/bubbles .   (It's OK if it says "file already exists")
12) ln -s /root/allstar/getdb .     (It's OK if it says "file already exists")
13) ln -s /root/allstar/gmtdate .   (It's OK if it says "file already exists")
14) ln -s /root/allstar/txlog .     (It's OK if it says "file already exists")
15) cd /etc/asterisk
16) ln -s /root/allstar/downloadnews .     (It's OK if it says "file already exists")
17) ln -s /root/allstar/playnewstest .     (It's OK if it says "file already exists")
18) cd /srv/http/supermon
19) cp /root/allstar/AllstarBubbleDiagram.html .      (Answer "Y" (YES) to overwrite file if it already exists)
20) cp /root/allstar/AllstarBubbleDiagram2.html .     (Answer "Y" (YES) to overwrite file if it already exists)
21) cd /root/allstar
22) txlog             (NOTE: LEAVE THIS PROCESS  RUNNING! - It is the one that updates the bubble diagram in real-time!)
23) Open a web browser to this URL (substitute your Allstar node's IP address for the one shown here) - http://192.168.1.20/supermon/AllstarBubbleDiagram2.html
24) Watch the web page automatically change when someone keys-up, unkeys, connects or disconnects.  The page will also change if one of the connected nodes reports its statistics in the background (happens sometimes) and that report "looks weird" to my scripts.
25) To compare this drawing with that from Allstarlink.org, try the "AllstarBubbleDiagram.html" webpage (NOTE: Unlike the previous one, this webpage doesn't have a "2" in its name)

NOTES:
--------------
*  Your Allstar node must have Internet connectivity in order to use the above procedure
*  "txlog" is the process that updates the diagram displayed in the "AllstarBubbleDiagram2.html" webpage. If "txlog" stops, so will live updates of that diagram.
*  If you want to keep "txlog" running in the background even if you exit the shell or log off of your node, try this -

1) cd /root
2) (nohup /root/allstar/txlog >> /root/txlog.log 2>>/root/txlog.err) &

* If you want to run the "downloadnews" or "playnewstest" files, read the notes in their top portions. These notes specify how to use them.

* To run this software on any "non-Allstar" (or, more correctly, any non-Arch Linux-based Allstar) system, try the following changes -

1) Substitute your Linux distribution's package manager (yum, apt-get, etc) for the Arch Linux "pacman" package manager mentioned above.
2) Substitute your webserver's content-source directory (/var/www/html, etc.) in step 18, and adjust the URLs used to display the webpages above to match


- Mike, KA9CQL
