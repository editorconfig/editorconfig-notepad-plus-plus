# EditorConfig Notepad++ Plugin

This is an [EditorConfig][] plugin for Notepad++.

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

Before installation, you must have both [cmake][] and [Microsoft Visual C++][]
installed.

1.  Download the [EditorConfig C core][] and follow the instructions in the README
    and INSTALL files to compile and install it with Microsoft Visual C++. Note that the
    EditorConfig C core must be built with the `MT` option.

2.  Download the [EditorConfig plugin for Notepad++][] and extract it.

3.  Open your command line prompt, and use `cd` command to switch current
    working directory to the root of the source tree of the Notepad++ plugin.
    Use the following command to generate the project file:

        C:\path\to\editorconfig-notepad-plus-plus> cmake . -DEDITORCONFIG_CORE_PREFIX="$(The_path_to_EditorConfig_core_prefix)"

    After this project file is built, run `cmake-gui .` to set the variable
    `PCRE_LIB_DIR` to the directory which contains `pcre.lib` (can be downloaded
    from
    https://sourceforge.net/projects/editorconfig/files/EditorConfig-C-Core/3rd-party/pcre-8.38/?upload_just_completed=true
    ). Double click on the project file and use Microsoft Visual C++ to build
    it.

4.  If the build succeeded, you should have `bin\unicode\NppEditorConfig.dll`
    and `bin\ansi\NppEditorConfig.dll` in your build tree. Copy the
    corresponding one (the unicode one if your Notepad++ is unicode version or
    the ansi one if your Notepad++ is ansi version) to your the plugin directory
    of the Notepad++ directory to complete the installation.


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

Feel free to submit bugs, feature requests, and other issues to the main
[EditorConfig issue tracker](https://github.com/editorconfig/editorconfig/issues).


[cmake]: http://www.cmake.org
[EditorConfig]: http://editorconfig.org
[EditorConfig C core]: https://github.com/editorconfig/editorconfig-core
[EditorConfig plugin for Notepad++]: https://github.com/editorconfig/editorconfig-notepad-plus-plus
[Microsoft Visual C++]: http://msdn2.microsoft.com/en-us/visualc/default.aspx
[Notepad++ Plugin Manager]: http://www.brotherstone.co.uk/npp/pm/
[download]: https://sourceforge.net/projects/editorconfig/files/EditorConfig-Notepad%2B%2B-Plugin/
[properties]: http://editorconfig.org/#supported-properties
