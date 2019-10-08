# syx2mid
Convert raw MIDI System Exclusive files to standard MIDI file format

This is a very simple application which takes a single command line parameter of the filename of the raw sysex file and wraps the data in a standard (format 0) MIDI file called sysex.mid. There is much room for improvement and it is currently untested but that's what you get when you knock something up in an hour on the train!
To compile you should be able to type:

    make syx2mid

To use, provide the name of the raw sysex file as the only command line paramter, e.g.

    syx2mid myfile.syx

This should create a file called "sysex.mid" which should be playable from a standard MIDI file  player. Leading 0xF0 and trailing 0xF7 are appended if required, i.e. it supports raw sysex files with and without the sysex MIDI commands within it. The standard MIDI file created is of type / format 0 with just one track and one sysex event at time offset 0.
