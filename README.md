# EditorConfig Notepad++ Plugin

This is an [EditorConfig][] plugin for Notepad++.

## Installation

### Install from the Binary Package

Binary package is not ready yet. If you want to install from source, see the
`Install from Source` section.


### Install from Source

Before installation, you must have both [cmake][] and [Microsoft Visual C++][]
installed.

1. Download the [EditorConfig core][] and follow the instructions in the README
   and INSTALL files to compile it with Microsoft Visual C++. After compiling
   the [EditorConfig core][], you should have a file named
   `editorconfig_static.lib` in the directory of `src\lib` of the EditorConfig
   core build tree. 

2. Download the [EditorConfig plugin for Notepad++][] and extract it.

3. Copy `editorconfig_static.lib` mentioned in section `1` into the directory
   of `src` of the Notepad++ plugin build tree, and copy the directory
   `src\include\editorconfig` of the EditorConfig core source tree into `src`
   of the Notepad++ plugin source tree.

4. Open your command line prompt, and use `cd` command to switch current
   working directory to the root of the source tree of the Notepad++ plugin.
   Use the following command to generate the project file:

   `C:\path\to\editorconfig-notepad-plus-plus> cmake .`

   After this project file is built, double click on the project file and use
   Microsoft Visual C++ to build it.

5. If the build succeeded, you should have `bin\unicode\NppEditorConfig.dll`
   and `bin\ansi\NppEditorConfig.dll` in your build tree. Copy the
   corresponding one (the unicode one if your Notepad++ is unicode version or
   the ansi one if your Notepad++ is ansi version) to your the plugin directory
   of the Notepad++ directory to complete the installation.




[cmake]: http://www.cmake.org
[EditorConfig]: http://editorconfig.org
[EditorConfig core]: https://github.com/editorconfig/editorconfig
[EditorConfig plugin for Notepad++]: https://github.com/editorconfig/editorconfig-notepad-plus-plus
[Microsoft Visual C++]: http://msdn2.microsoft.com/en-us/visualc/default.aspx
