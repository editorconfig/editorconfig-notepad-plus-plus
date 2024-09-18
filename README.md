# EditorConfig Notepad++ Plugin

This is an [EditorConfig][] plugin for [Notepad++](https://notepad-plus-plus.org/).

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

Before installation, you must have [Microsoft PowerShell][PowerShell] 3 or higher, [cmake][] 3.5.0 or higher and [Microsoft Visual Studio][] (2013, 2015 or 2017) installed.

#### Prepare code

Clone the [EditorConfig plugin for Notepad++][] sources or download and extract it.

#### Download dependencies
To download [EditorConfig C core][] and [PRCE2][pcre] use the `init.ps1` script.

```powershell
~> ./init.ps1 [-prce 10.32] [-edc 0.12.3]
```

Arguments:

    - pcre Optional, pcre2 version to download.
    - edc  Optional, editorconfig core version to download.

#### Build all
To build all in one step use the ` -proj all`, `-init` and `-install` arguments with the `build.ps1` script at the same time.
You will find the `NppEditorConfig.dll` in the `bin/x64/` folder.

```powershell
~> ./build.ps1 -proj all -init -install
```

The `-init` argument will generate the required cmake build files for Visual Studio. This is required after initial checkout or `CMakeLists.txt` changes.
The `-install` argument will put the binaries to a location (`bin/$(ARCH)/build`) that the project can find and link the libraries.


For the other arguments please see below.

```powershell
~> ./build.ps1 [-proj all | core | pcre2, npp] [-init] [-install] [-vsver 15 | 14 | 12] [-arch x64 | x86] [-config Release | Debug]
```

Arguments:

    -proj Project to build.
    -init Optional; (Re)Generate cmake build files, required first time or on `CMakeLists.txt` changes.
    -install Optional; Install to `bin/$(ARCH)/build` folder.
    -vsver Optional; Visual Studio version (major version number) to use.
    -arch Optional; Architecture to build for.
    -config Optional; Debug or release build.


#### Build pcre2 library

```powershell
~> ./build.ps1 -proj pcre2 -init -install
```

#### Build editorconfig core library

```powershell
~> ./build.ps1 -proj core -init -install
```

#### Build editorconfig notepad++ plugin

```powershell
~> ./build.ps1 -proj npp -init -install
```

## Supported properties

The EditorConfig Notepad++ plugin supports the following EditorConfig
[properties][]:

* indent_style
* indent_size
* tab_width
* end_of_line
* trim_trailing_whitespace
* insert_final_newline
* charset
* root (only used by EditorConfig core)

## Bugs and Feature Requests

Feel free to submit bugs, feature requests, and other issues to the
[issue tracker](https://github.com/editorconfig/editorconfig-notepad-plus-plus/issues).


[cmake]: https://www.cmake.org
[EditorConfig]: http://editorconfig.org
[EditorConfig C core]: https://github.com/editorconfig/editorconfig-core-c
[EditorConfig plugin for Notepad++]: [https://github.com/editorconfig/editorconfig-notepad-plus-plus](https://github.com/bruderstein/nppPluginManager)
[Microsoft Visual Studio]: https://www.visualstudio.com/
[Notepad++ Plugin Manager]: https://bruderste.in/npp/pm/
[download]: https://sourceforge.net/projects/editorconfig/files/EditorConfig-Notepad%2B%2B-Plugin/
[pcre]: https://ftp.pcre.org/pub/pcre
[PowerShell]: https://docs.microsoft.com/en-us/powershell
[properties]: http://editorconfig.org/#supported-properties
