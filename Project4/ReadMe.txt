Adam Heimendinger
ComSci 327
Project 4
Read.me

Source files:

	CS229Object:
		This class serves as an useable and versatile container for the data held within a .cs229 file, or that which
	will soon be written to one. File parsing methods within utilFunctions use this object to store the data, while
	WaveGenerator, for instance, will populate an instance with data representing a clean waveform.
	The variables within this class have simple getter and setter methods for ease of use.
	
	ADSR Envelope:
		This class serves to manipulate pre-generated sample data to conform to specific ADSR values.
	The values are given in the constructor while modify() serves to change the sample data
	of a given cs229Object. The constructor sets basic values, and accounts for a length that is
	shorter than the sum of the attack, decay, and release times, and adjusts the new release
	slope accordingly. The envelope essentially stores the slope-intercept form of each portion 
	of the ADSR cycle.
	
	WaveGenerator:
		This class serves as something similar to an interface in that it will return a cs229Object resembling the
	waveform that was passed in during construction. Thus, new waveforms need only be added as another function
	within WaveGenerator without unecessary object construction. This resulting waveform is considered a "pure" wave,
	in that its properties are unaltered by the ADSREnvelope yet. The call to generate(), which returns a new data
	set, identifies the type of wave being constructed, and calls the corresponding function to generate that wave. 
	For instance, a call to sinGen() by generate() yields a data set resembling a sine wave with amplitude 1, and 
	length and frequency given.
	
	utilFunctions:
		The utilFunctions source file serves as a collection of useful functions that can be shared across sndgen, sndmix,
	sndcat, and sndmix. They involve everything from concatenation of objects, to outputing, to displaying helpful messages.
	This eliminates the redundancy of rewritting code across classes, when such code would be just copied.
	
	SoundInfo:
		Sound info is a basic info program that takes in cs229 filenames as arguments
	and prints their information, either to stdout or a file. This includes such data as the filename,
	sample rate, number of channels, and length, among others.
	
	SoundCat:
		The purpose of Sound concatenation is to take in one or more filenames,
	construct the corresponding cs229Objects for each, then outputting, either
	to a file or stdout, the resulting file information, followed by all of the files' 
	data appended.
	
	SoundMix:
		Sound mix serves to take in one or more files and their corresponding factors.
	From there it multiplies the sample data of each file by the file's factor,
	which then leads to the merging of all the files' data. The resulting object is outputted
	to either stdin or a file

	SoundGen:
		This program serves to produce a sound with specified frequency, waveform, and ADSR specifics.
	It supports numerous switches, many of which are required to generate the sound.
	Once the sound data has been parsed and checked, the initial sample data is generated
	with WaveGenerator and stored in cs229Object initial. From there, the ADSREnvelope modifies the data, and it is
	either outputed to a file or stdout.