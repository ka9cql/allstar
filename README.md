# allstar
Tools and scripts related to the Allstar Ham Radio Linking capability


NOTE: As of 2021 Allstar has made infrastructure changes that cause this codebase's real-time bubble display and txlog functions to misbehave. I will work to fix them ASAP. For now, know that it's not your system, it's my code that's the problem. Thanks for your patience!




To install or update these tools on an Arch Linux-based Allstar node (circa 2018)
---------------------------------------------------------------------------------

1. Log into your Allstar node, and drop to the BASH shell

# IF YOU'VE DONE SOME OF THIS BEFORE, and only need to update your code to the latest -
2. cd /root/allstar
3. git pull
#


# IF THIS IS THE FIRST TIME DOING ALL OF THIS (aka "starting from scratch") -
2. pacman -S git           (NOTE: If it says "already installed", or "already up-to-date", you can answer "n" (NO), when asked if you want to re-install it)
3. pacman -S lynx         (NOTE: If it says "already installed", or "already up-to-date", you can answer "n" (NO), when asked if you want to re-install it)
4. pacman -S graphviz     (NOTE 1of2: Same as above - you don't have to re-install it if it is already installed.)
                          (NOTE 2of2: If it gives you a choice of where to grab this package from, you can accept the default)
5. cd /root
6. git clone https://github.com/ka9cql/allstar.git
7. cd /root/allstar
8. chmod 755 astar bubbles findsplit freqdetect.py getdb gmtdate newscut pitemp txlog downloadnews playnewstest
9. cd /usr/local/bin
10. Make commands available to all users -
#
    a) ln -s /root/allstar/astar .         (It's OK if it says "file already exists")
#
    b) ln -s /root/allstar/bubbles .       (It's OK if it says "file already exists")
#
    c) ln -s /root/allstar/findsplit .     (It's OK if it says "file already exists")
#
    d) ln -s /root/allstar/freqdetect.py . (It's OK if it says "file already exists")
#
    e) ln -s /root/allstar/getdb .         (It's OK if it says "file already exists")
#
    f) ln -s /root/allstar/gmtdate .       (It's OK if it says "file already exists")
#
    g) ln -s /root/allstar/newscut .       (It's OK if it says "file already exists")
#
    h) ln -s /root/allstar/pitemp .        (It's OK if it says "file already exists")
#
    i) ln -s /root/allstar/txlog .         (It's OK if it says "file already exists")
#
    j) ln -s /root/allstar/playnewstest .  (It's OK if it says "file already exists")
#
    k) ln -s /root/allstar/playnews.ini .  (It's OK if it says "file already exists")
#

11. mkdir -p /etc/asterisk/local/playnews
12. cd /etc/asterisk/local/playnews
13. wget https://hamvoip.org/downloads/playnews-0.11.tar.gz
14. tar xvf playnews-0.11.tar.gz
15. mv playnews.ini playnews.ini.ORIG
16. Make files available to Allstar/cron -
#
    a) ln -s /root/allstar/downloadnews .     (It's OK if it says "file already exists")
#
    b) ln -s /root/allstar/newscut .          (It's OK if it says "file already exists")
#
    c) ln -s /root/allstar/playnewstest .     (It's OK if it says "file already exists")
#
    d) ln -s /root/allstar/playnews.ini .     (It's OK if it says "file already exists")
#
17. cd /srv/http/supermon
18. Make files available to Apache -
#
    a) cp /root/allstar/AllstarBubbleDiagram.html .      (Answer "Y" (YES) to overwrite file if it already exists)
#
    b) cp /root/allstar/AllstarBubbleDiagram2.html .     (Answer "Y" (YES) to overwrite file if it already exists)
#
19. cd /root/allstar
20. txlog             (NOTE: LEAVE THIS PROCESS  RUNNING! - It is the one that updates the bubble diagram in real-time!)
21. Open a web browser to this URL (substitute your Allstar node's IP address for the one shown here) - http://192.168.1.20/supermon/AllstarBubbleDiagram2.html
22. Watch the web page automatically change when someone keys-up, unkeys, connects or disconnects.  The page will also change if one of the connected nodes reports its statistics in the background (happens sometimes) and that report "looks weird" to these scripts.
23. To compare this drawing with that from Allstarlink.org, open this URL - http://192.168.1.20/supermon/AllstarBubbleDiagram.html (NOTE: No "2" after "Diagram")


NOTES:
--------------
*  Your Allstar node must have Internet connectivity in order to use the above procedure
*  "txlog" is the process that updates the diagram displayed in the "AllstarBubbleDiagram2.html" webpage. If "txlog" stops, so will live updates of that diagram.
*  If you want to keep "txlog" running in the background even if you exit the shell or log off of your node, try this -

A) cd /root
#
B) (nohup /root/allstar/txlog >> /root/txlog.log 2>>/root/txlog.err) &
#

* If you want to run the "downloadnews" or "playnewstest" files, read the notes in their top portions. These notes specify how to use them.

* To run this software on any "non-Allstar" (or, more correctly, any non-Arch Linux-based Allstar) system, try the following changes -

a) Substitute your Linux distribution's package manager (yum, apt-get, etc) for the Arch Linux "pacman" package manager mentioned in steps 2-4, above.
#
b) Substitute your webserver's content-source directory (/var/www/html, etc.) in step 17, and adjust the URLs used to display the webpages in steps 21 and 23 to match
#






HOW TO DOWNLOAD AND PLAY AR NEWSLINE AND/OR ARRL AUDIO NEWS ON YOUR LOCAL ALLSTAR NODE -
--------------

Download and prepare the audio files(s) for later playing -

1) Execute "newscut" from the command line in the /root/allstar directory like this -
#
    newscut ARN      (to download and prepare the AR Newsline audio file)
#
    newscut ARRL     (to download and prepare the ARRL Audio News audio file)
#


After running "newscut", you can then play the audio file(s) over your Allstar node.  The tool below will "warn" your users that the news is about to be played, and then play the news.

NOTE: YOU MUST ensure that all of your timeout timers are set sufficiently long (5 minutes or so) before playing the news.  Check both Allstar/Asterisk and your node's radio!!


2) Execute "playnewstest" from the command line in the /root/allstar directory when you want the audio news to begin playing like this -
#
    playnewstest ARN NOW L       (to play the AR Newsline)
#
    playnewstest ARRL NOW L      (to play the ARRL Audio News)
#

In the above examples, the argument "L" means that the audio file will ONLY play on the local Allstar node - it will *NOT* play on any other Allstar node that your node is currently connected to.  This is known as playing "locally".
Similarly, in the above examples, the argument "G" means that the audio files will be played on your local Allstar node as well as EVERY Allstar node that your node is currently connected to.  This is know as playing "globally".
#
Most people chose to play "globally", and only have their Allstar node connected to those wishing to listen in to the news.
#
#
Generally, people put the above two commands in CRON (a Linux tool to execute scripts at pre-determined times).  You would need to create two entries, with "newscut" occurring at least an hour before "playnewstest", to be safe.
#
#

cron examples (I used a variety of options, to give you examples of each) -
#
Run script to download and prepare (cut) the ARN news file every Monday at 4:00 PM, logging all actions to /root/allstar/newscut.log
#
00 16 * * 1 /etc/asterisk/newscut ARN &> /root/allstar/newscut.log 2>&1

#
Run script to download and prepare (cut) the ARRL news file every Friday at 2:30 PM, logging all actions to /root/allstar/newscut.log
#
30 14 * * 5 /etc/asterisk/newscut ARRL &> /root/allstar/newscut.log 2>&1

#
Run script at 8:30 PM to play ARRL news "globally" every Tuesday at 9:00 PM (warnings to users will occur 10 and 5 minutes prior to commencement of playing)
#
30 20 * * 2 /etc/asterisk/playnewstest ARRL 21:00 G &> /dev/null 2>&1

#
Run script at 6:00 PM to play ARN news "locally" every Thursday at 7PM (again, warnings will occur 10 and 5 minutes before playing starts)
#
00 18 * * 4 /etc/asterisk/playnewstest ARN 19:00 L &> /dev/null 2>&1
#
#


- Mike, KA9CQL
