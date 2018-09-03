Terminal Based File Explorer

1. Normal Mode:
	a) In normal mode file names are shown.
	b) The file explorer is able to handle vertical overflow by scrolling up and down with the 		up-down arrow keys.
	c) We can navigate up and down through the file names.
	d) If enter is pressed the corresponding content of the directory will be shown.
	e) If the corresponding entry is a file it will open with its default application.
	f) On pressing left and right arrow key the previous and next directory content will open.
	g) To go to parent directory ".." entry should be pressed.
	h) To go to the application home directory key "h" should be pressed.
2. Command Mode:
	a) The application enters command mode with ":" pressed.
	b) Copy: Copying a entire directory or a file is handled properly. The command for copy 	is "copy <source directory/filename> <destination directory>". If the source is in current 		directory then the directory name is enough. Else the absolute path like /home/user/<path> 		shall be given for source and destination.
	e.g. copy /home/user/Videos /home/user/snap, this will copy the Videos folder to snap.
	c) Move: Moving directory and files is implemented properly. Inorder to move a file or 		directory the comnmand is "move <source path/directory or file name <destination path>
	e.g. move /home/user/Videos /home/user/snap. This will move Videos folder to snap.
	d) Rename: Renaming a file or a directory is done with "rename oldfilename newfilename"
	e.g. rename /home/user/snap /home/user/abc. "snap" folder will renamed as "abc".
	e) Create File and Directory: Use command <create_file filename> or <create_dir dirname>
	to create directory in home or give absolute path.
	f)Delete: use command <delete filename or directoryname>. Use absolute path unless you are 		deleting something in home directory of the application.
	g)Goto: Use goto <path> to go to any directory. U cannot traverse back to previous 		directory if you use goto.
	
Finally type "exit" in command mode to go back to normal mode. 
	

