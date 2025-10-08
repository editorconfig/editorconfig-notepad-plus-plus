param(
    [ValidateSet("pcre2", "core", "npp", "all")]
    [string] $proj,

    [switch] $init,

    [switch] $install,

    [ValidateSet("Debug", "Release")]
    [string] $config = "Release",

    [ValidateSet("x86", "x64", "arm64")]
    [string] $arch = "x64",


    [ValidateSet("17","16","15","14")]
    [int] $vsver = 15
)

$ErrorActionPreference = "stop"


function exec {
    param
    (
        [ScriptBlock] $ScriptBlock,
        [int[]] $AllowedExitCodes = @(0)
    )

    $backupErrorActionPreference = $script:ErrorActionPreference

    $script:ErrorActionPreference = "Continue"
    try {
        & $ScriptBlock
        if ($AllowedExitCodes -notcontains $LASTEXITCODE) {
            throw "Execution failed with exit code $LASTEXITCODE"
        }
    }
    finally {
        $script:ErrorActionPreference = $backupErrorActionPreference
    }
}

if ($proj -eq "all") {
    .\build.ps1 -proj pcre2 -init:$init -install:$install -arch $arch -config $config
    .\build.ps1 -proj core -init:$init -install:$install -arch $arch -config $config
    .\build.ps1 -proj npp -init:$init -install:$install -arch $arch -config $config
    return
}

$PREFIX = "../build"

$dest = "bin"

if ((Test-Path $dest) -ne $true) {
    throw "Missing build path! Used init?"
}

if ($init) {

    $gen = "Visual Studio "

    switch ($vsver) {
        17 {
            $gen += "17 2022"
        }
        16 {
            $gen += "16 2019"
        }
        15 {
            $gen += "15 2017"
        }
        14 {
            $gen += "14 2015"
        }
        12 {
            $gen += "12 2013"
        }
        default {
            throw "Visual Studio version $vsver not supported!"
        }
    }

    $cmake_arch=""
    if($arch -eq "x64"){
        $cmake_arch += "x64"
    }
    elseif($arch -eq "x86"){
        $cmake_arch += "Win32"
    }
    elseif($arch -eq "arm64"){
        $cmake_arch += "ARM64"
    }

    New-Item $dest\$arch\$proj -ItemType Directory -ErrorAction SilentlyContinue | Out-Null
    Push-Location $dest\$arch\$proj
    try {
        switch ($proj) {
            pcre2 {
                exec { cmake -G "$gen" -A $cmake_arch -DCMAKE_INSTALL_PREFIX="$PREFIX" `
                        -DPCRE2_STATIC_RUNTIME=ON `
                        -DPCRE2_BUILD_PCRE2GREP=OFF `
                        -DPCRE2_BUILD_TESTS=OFF `
                        "../../pcre2" }
            }
            core {
                exec { cmake -G "$gen" -A $cmake_arch -DCMAKE_INSTALL_PREFIX="$PREFIX" `
                        -DPCRE2_STATIC=ON `
                        "../../editorconfig-core-c" }
            }
            npp {
                exec { cmake -G "$gen" -A $cmake_arch -DEDITORCONFIG_CORE_PREFIX="$(Resolve-Path $PREFIX)" "../../../." }
            }
        }
    }
    finally {
        Pop-Location
    }
}

if ((Test-Path $dest\$arch\$proj) -ne $true) {
    throw "Missing build path! Used init?"
}

exec { cmake --build $dest\$arch\$proj `-- /p:Configuration=$config }

if ($install) {
    switch ($proj) {
        pcre2 {
            exec { cmake --build $dest\$arch\$proj --target install `-- /p:Configuration=$config }
        }
        core {
            exec { cmake --build $dest\$arch\$proj --target install `-- /p:Configuration=$config }
        }
        npp {
            Copy-Item $dest\$arch\$proj\bin\unicode\$config\NppEditorConfig.dll -Destination $dest\$arch\ -Force -Confirm:$false
            Copy-Item $dest\$arch\$proj\bin\unicode\$config\NppEditorConfig.pdb -Destination $dest\$arch\ -Force -Confirm:$false
        }
    }
}
