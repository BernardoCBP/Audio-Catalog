# Audio-Catalog
A C program to catalog a set of audio track files, stored in MP3 format files, based on their tag (MPEG Audio format
ID3v1 or ID3v1.1), allowing ordered listing, with different criteria.

The program receives, as a command line argument, the name of a text file containing a list with the names of the MP3 files to catalogue. 
After reading the list and the files that it indicates, cyclically, displays a prompt and waits, as standard input, for one of the following commands:

  a – (artists) displays the list of audio tracks sorted by Artist. If there are multiple tracks from the
  same interpreter, successively applies the order by Album and Title;

  t – (titles) displays the audio track list sorted by Title. If there are multiple tracks with
  identical title, successively applies the order by Artist and Album;

  q – (quit) finish.

The lists are produced in standard output, by executing the commands, and their order is alphabetical growing. 
The information of each audio track is displayed in one line, containing the tag fields and the name of the corresponding MP3 file.
