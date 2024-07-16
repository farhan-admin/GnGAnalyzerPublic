# GnGAnalyzer #

### What is this repository for? ###

GnGAnalyzer is a "Hello World" equivalent application created to aid understanding of the full cycle of 3D visualization process of seismic data. Application creates a GUI
using the Qt framework, it preforms data I/O using C++ classes and visulization using OpenGL and GLSL.

### How do I get set up for development? ###

Clone or download the repository folder (*Download Repository* from the menu just above the files list on this page) on a Windows 64bit machine. Also, install the Qt
5.15 frame work on the same machine and open the project file "GnGAnalyzer_v01.pro" in the Qt creator. Next, if required adjust the "build" parameters suited for your machine.
Then click the "Run" button on the left toolbar, inside the Qt creator. This will compile the source code and start the application's exe file.

### How to run the application after compilation? ###

1) To start the application, choose one of the following:

- Click the "Run" button inside the Qt creator (recommended)
- After compilation, directly click on the compiled exe file. You might have to copy the supporting dll files in the exe's folder to run the application.

2) Application will start, showing a black sub window, a tree window and a menu bar at the top.

3) Click the "Load Data" button to open the file open dialog. A new dialog box will appear. This needs two files to continue. One SegY template file and a SegY file.

4) A sample SegY template file "SegYRev1Template_ver5.sgyt" is provided with the source code. You can use that in the "Load Data" dialog box.

5) You also need a SegY file. Two sample SegY files are provided with the source code, you can use any one of these files. 

6) After closing the dialog box, the loaded file shows up in the tree window. 

7) You may "Right Click" on the file in the tree window and select the "Display" command. This will display the data in the 3D window.

### Who do I talk to? ###

Farhan Naseer

farhannaseer78@gmail.com

### License ###

Copyright (C) 2023 Farhan Naseer.

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.
