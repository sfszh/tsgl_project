TSGL
====

Thread Safe Graphics Library

You can generate Doxygen locally using 'make docs', or view the [TSGL API here](http://calvin-cs.github.io/TSGL/html/index.html).

------------
Description
------------
TSGL is a thread-safe graphics library perfect for drawing graphics. You can do a wide variety of things with TSGL, including: image manipulation and rendering (.bmp, .jpeg, and .png image formats supported), 2D polygon drawing (rectangles, circles, triangles, etc.), text rendering, animations with keyboard and/or mouse events, and much more. All drawing and rendering is done with threads and in parallel. This library is currently supported on Windows, Mac OS, and Linux. 3D graphics are currently not supported by this library.

If you would like TSGL in your local git repository, use the following command:

git clone https://github.com/Calvin-CS/TSGL.git

Otherwise, click the "Download zip" button to download a zipped up version.

------------
Goals
------------
The main goal of this library is to provide a thread-safe graphics library for 2D graphics. Other goals include: Helping beginning programming students learn about the complex process of parallelization by giving them hands-on tools to use in order to learn about parallelization without having them to worry about the problems associated with parallelization such as race conditions, mutexes, and more. It also helps educators teach programming students about parallelization through simple visualizations.

------------
Installation help
------------
Linux - https://www.youtube.com/watch?v=htPo-tFSOdE

Mac - https://www.youtube.com/watch?v=qIHrAAxdnmQ

Windows - https://www.youtube.com/watch?v=v38PuL1Khhk

------------
Dependencies
------------
TSGL needs these libraries in order to function properly: GLEW, glfw, GL, freetype and stb. The stb library comes bundled with the TSGL source code, but the other four are not. For platform-specific dependencies, please see the "Installation help" section and then choose the video that shows the installtion of TSGL on the platform.

------------
Tutorials
------------
Want to learn how to utilize the TSGL library? Check out the [wiki](https://github.com/Calvin-CS/TSGL/wiki) pages for some tutorials!

------------
docs and docs-wiki
------------
docs and docs-wiki are both submodules located inside of the TSGL root directory. docs contains the documentation for TSGL classes and docs-wiki contains the wiki pages. See the READMEMISC.txt file for information on how to initialize and update these submodules in your local git repository. 
