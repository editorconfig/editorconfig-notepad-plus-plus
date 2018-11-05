# EditorConfig Notepad++ Plugin

This is an [EditorConfig][] plugin for Notepad++.

## Buildstatus
[![Appveyor build status](https://ci.appveyor.com/api/projects/status/github/xuhdev/editorconfig-notepad-plus-plus?branch=master&svg=true)](https://ci.appveyor.com/project/xuhdev/editorconfig-notepad-plus-plus)
[![GitHub release](https://img.shields.io/github/tag/editorconfig/editorconfig-notepad-plus-plus.svg)](https://github.com/editorconfig/editorconfig-notepad-plus-plus/tags)

## Installation

### Install from the Plugin Manager

If you have [Notepad++ Plugin Manager][] installed (usually installed by
default in Notepad++), you can install in this way.

Launch Notepad++, click on the `Plugins` menu, then
`Plugin Manager` -> `Show Plugin Manager`. In the `Available` tab, find
`EditorConfig` in the list, check the checkbox and click on the `Install`
button.

### Install from the Binary Package

1.  Download the binary package from
    [the SourceForge downloads page][download].

2.  Extract the compressed archive and copy the corresponding dll into your
    Notepad++ plugin directory(the unicode dll if your Notepad++ version is
    unicode or the ansi one if you're using the ansi version Notepad++).


### Install from Source

Before installation, you must have both [cmake][] and [Microsoft Visual Studio][] installed.

1.  Download the [EditorConfig C core][] and follow the instructions in the README and INSTALL files to compile and install it with Microsoft Visual C++. Note that the EditorConfig C core must be built with the `MT` option. To build the x64 version of the plugin, the x64 version of EditorConfig C core is needed.

2.  Download the [EditorConfig plugin for Notepad++][] and extract it.

3.  Open `cmake-gui`, select the extracted folder as source code directory, and create a directory to build the binaries. Then click the `Configure` button. When asked for a generator, select a generator without suffix to build the x86 version, or a generator with `Win64` suffix to build the x64 version.

    Build options will appear after the configure process has finished. Set `EDITORCONFIG_CORE_PREFIX` to the install directory of EditorConfig C core, and `PCRE_LIB_DIR` to the directory which contains `pcre.lib` (x86 prebuild lib can be downloaded from https://sourceforge.net/projects/editorconfig/files/EditorConfig-C-Core/3rd-party/pcre-8.38/ . For newer version of pcre, or to build the x64 version, you can find the source at https://ftp.pcre.org/pub/pcre/ and build it using CMake).

    Click the `Configure` button again, and then the `Generate` button and the `Open Project` button. This will open the generated project in Visual Studio. If you don't need the debug features you may change the solution configuration type to `Release` in the toolbar. Then build the project with VS.

4.  If the build succeeded, you should have `bin\unicode\Release\NppEditorConfig.dll` in your build tree. Copy the dll to your the plugin directory of the Notepad++ directory to complete the installation.


## Supported properties

The EditorConfig Notepad++ plugin supports the following EditorConfig
[properties][]:

* indent_style
* indent_size
* tab_width
* end_of_line
* trim_trailing_whitespace
* insert_final_newline
* root (only used by EditorConfig core)

## Bugs and Feature Requests

Feel free to submit bugs, feature requests, and other issues to the
[issue tracker](https://github.com/editorconfig/editorconfig-notepad-plus-plus/issues).


[cmake]: https://www.cmake.org
[EditorConfig]: http://editorconfig.org
[EditorConfig C core]: https://github.com/editorconfig/editorconfig-core
[EditorConfig plugin for Notepad++]: https://github.com/editorconfig/editorconfig-notepad-plus-plus
[Microsoft Visual Studio]: https://www.visualstudio.com/
[Notepad++ Plugin Manager]: https://bruderste.in/npp/pm/
[download]: https://sourceforge.net/projects/editorconfig/files/EditorConfig-Notepad%2B%2B-Plugin/
[properties]: http://editorconfig.org/#supported-properties
