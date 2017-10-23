# remote-audiorec
Remote Audio Recorder using Win32-API.

This application creates a socket server listening for commands on port 50000.

Supported commands are simple strings: `start` and `stop`.

`start`: Starts audio recording and writes to file `audio-N.raw`.

`stop`: Stops audio recording and closes file.

I am using it with my Telegram bot [elfibot-0](https://github.com/aelfimow/elfibot-0).
