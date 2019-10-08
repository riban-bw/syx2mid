# syx2mid
Convert raw MIDI System Exclusive files to standard MIDI file format

This is a very simple application which takes a single command line parameter of the filename of the raw sysex file and wraps the data in a standard (format 0) MIDI file called sysex.mid. There is much room for improvement and it is currently untested but that's what you get when you knock something up in an hour on the train!
To compile you should be able to type:

    make syx2mid

