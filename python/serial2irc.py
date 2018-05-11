#! /usr/bin/env python
#
# Example program using irc.bot.
#
# Joel Rosdahl <joel@rosdahl.net>

"""A simple example bot.
This is an example bot that uses the SingleServerIRCBot class from
irc.bot.  The bot enters a channel and listens for commands in
private messages and channel traffic.  Commands in channel messages
are given by prefixing the text by the bot name followed by a colon.
It also responds to DCC CHAT invitations and echos data sent in such
sessions.
The known commands are:
    stats -- Prints some channel information.
    disconnect -- Disconnect the bot.  The bot will try to reconnect
                  after 60 seconds.
    die -- Let the bot cease to exist.
    dcc -- Let the bot invite you to a DCC CHAT connection.
"""
import ssl
import irc.bot
import irc.strings
from irc.client import ip_numstr_to_quad, ip_quad_to_numstr
import threading
import serial

channel = "#botzone"

def serialWorker(ircbot):
    """thread worker function"""
    with serial.Serial('/dev/ttyUSB0') as ser:
        while True:
            line = ser.readline().decode('utf-8')
            if line.startswith("CLOSED"):
                ircbot.connection.notice(channel, "Hackerspace Nijmegen is CLOSED!")
            elif line.startswith("OPEN"):
                ircbot.connection.notice(channel, "Hackerspace Nijmegen is OPEN!")
            else:
                print("unexpected input from arduino '%s'"%line)



class TestBot(irc.bot.SingleServerIRCBot):
    def __init__(self, channel, nickname, server, port=6667):
        ssl_factory = irc.connection.Factory(wrapper=ssl.wrap_socket)
        irc.bot.SingleServerIRCBot.__init__(
            self, [(server, port)], nickname, nickname, connect_factory=ssl_factory)
        self.channel = channel


    def on_nicknameinuse(self, c, e):
        print("nick '%s' in use, trying '%s'"%(c.get_nickname(), c.get_nickname()+"_"))
        c.nick(c.get_nickname() + "_")

    def on_welcome(self, c, e):
        c.join(self.channel)
        print("bot connected, joining channel '%s'"%self.channel)
        threading.Thread(target=serialWorker, args=(self,)).start()


    def on_privmsg(self, c, e):
        self.do_command(e, e.arguments[0])

    def on_pubmsg(self, c, e):
        a = e.arguments[0].split(":", 1)
        if len(a) > 1 and irc.strings.lower(a[0]) == irc.strings.lower(
                self.connection.get_nickname()):
            self.do_command(e, a[1].strip())
        return

    def do_command(self, e, cmd):
        nick = e.source.nick
        c = self.connection
        c.notice(nick, "This isn't the bot you're looking for... <(o.o)>")


def main():
    import sys

    bot = TestBot(channel, "robot", "ssl.irc.atw-inter.net", 6697)
    print("bot initialized")
    bot.start()


if __name__ == "__main__":
    main()
