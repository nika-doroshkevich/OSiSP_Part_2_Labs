# My labs for the subject "Operating systems and system programming (Part 2)" :computer:

:mortar_board: Third year of university, fifth semester.

These programs were written using Visual Studio 2022, __C++__.

_______________________________________________________________________________________

__Lab 1__ - Studying the event architecture of Windows applications, message processing mechanism, window redrawing mechanism

Requirements:

- Develop a program that allows you to move a sprite (a colored rectangle or ellipse) inside the working area of the window using the keyboard and mouse.
- Ensure the operation of the mouse wheel. Scrolling moves the sprite vertically. With the Shift key held down, scrolling the wheel moves the sprite horizontally.
- Give the sprite a movement with a bounce from the window borders.

_______________________________________________________________________________________

__Lab 2__ - Learning the output of text and fonts

Requirements:

- Develop a program that fits a text table (N rows by M columns) into the window in such a way that all the columns of the table are evenly 
distributed across the width of the window, and the height of the table rows is selected in such a way as to accommodate the text of each cell.
- When the window is resized, the table is recalculated and redrawn.

_______________________________________________________________________________________

__Lab 3__ - The study of the creation and use of dynamically loaded libraries (DLL), the method of embedding DLL in another process using a remote stream

Requirements:

- Develop a DLL with a function that searches through the entire virtual memory of a given string and replaces it with another string.
- Develop a program that performs static DLL import and calls its function.
- Develop a program that performs dynamic DLL import and calls its function.
- Develop a program that implements a DLL into a given process using a remote stream and calls the DLL function.

_______________________________________________________________________________________

__Lab 4__ - Study of the creation and use of threads and synchronization mechanisms

Requirements:

- Develop a task queue into which multiple threads can insert items atomically.
- Develop a handler for this queue that extracts tasks from it and distributes them to a given number of threads.
- Develop a program that uses a task queue and a queue handler to sort strings in a text file.
