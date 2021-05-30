# aementor

A simple toy project to explore C++ in the context of the AE Mentor program.

## Before starting
To make sure that everything is compatible with WSL or VM shared drives, pay attention to line endings when using Windows based editors. Make sure to us 'LF' instead of the default 'CRLF'. In Visual Studio Code, this can be done in the bottom right bar. It indicates the current mode, and can be clicked to change. Another strategy is to always create new files from within WSL or the VM by writing some dummy text to a file using `echo` like this: `echo 'some placeholder' > some_file.c`

A few tools and libraries are needed to successfully compile and run this project. For Ubuntu based platform, the script `install_dev_packages_ubuntu` is provided to automatically install the required packages. For other Linux flavors or different platforms, the list can be used to figure out what is needed. 

Here's a quick description of what we need:
- build-essential: The basic build utilities (`gcc`, `g++`, `make`, etc)
- gdb: The command line debugger
- CMake: A build system generator. Rather than writing Makefiles manually (or even worse, use Autoconf), CMake provides a powerful way of generating cross-platform build systems based on a simple (but quirky) scripting language.
- Boost: The powerful addition to the C++ STL. A massive collection of high quality libraries, but it can be a bit intimidating.
- libcurl: A library to conveniently talk to all things web
- dos2unix: For those time line endings get messed up, quickly convert everything all line endings to LF

When using WSL, CMake might have some issues on the first run due to some file system related configuration settings. Follow the instructions at [this stackoverflow issue](https://stackoverflow.com/a/62879722) and reboot (in my case, a PC reboot was required) to avoid `configure_file` issues.

## Configure, Compile, and Run
Start by creating a build directory outside the source directory. My preferred structure is as follows:
```
aementor/
├── aementor
└── build
```
I check out the repository in a project folder (typically with the same name if the repo has a sensible name) and put the build directory next to the repo/source directory in the project directory. Later on, we might want to build the project in different configuration (e.g. Debug and Release). At that point, we end up with multiple directories and it is nice to keep them grouped in a single project folder.

Move into the build directory and run CMake as follows: `cmake -DCMAKE_BUILD_TYPE=Debug ../aementor`. This will locate the compilers, find some libraries and generate the Makefiles. If all required packages have been installed, and WSL has been configured properly, this should run without errors.

To compile the simple application, run: `make`. This will compile two source files and link them into an executable. Again, this should run without any errors.

Next, we can simply run the application with `./simple` to get the default weather, or by passing in a city like this: `./simple London`.
