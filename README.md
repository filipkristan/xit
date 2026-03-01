# Xit

*Xit* is a git clone intended for use for only one individual with basic version control needs.  
It can make and reverse commits as well as name them but not much else.  
It's good enough for use on small projects.

NOTE: Commit messages will be implemented in the future.  

### Usage:
Make a new repository:
````
xit init
````

Make a new commit:
````
xit new
````

Revert a commit:
````
xit revert 000X
````
This command will revert all commits back to the version before the specified commit was made.  

Note: "000X" should be replaced with the number of the commit/patch you want to revert to.  
The zeroes are optional. "xit revert 00001" and "xit revert 1" are the same in the eyes of the program.