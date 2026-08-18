<#
.SYNOPSIS
    Configure and build the game with CMake + MinGW (msys2).
.EXAMPLE
    .\build.ps1            # build
    .\build.ps1 -Run       # build, then launch the game
    .\build.ps1 -Clean     # wipe build/ first
#>
param(
    [switch]$Run,
    [switch]$Clean,
    [ValidateSet('Debug', 'Release')]
    [string]$Config = 'Debug',
    [string]$MinGWBin = 'C:\msys64\mingw64\bin'
)

$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot
$buildDir = Join-Path $root 'build'

# Git for Windows ships its own mingw64\bin full of older DLLs. If it wins the
# PATH race, gcc's cc1.exe loads those instead of msys2's and dies with no
# message at all. Putting the real toolchain first keeps DLL resolution sane.
if (Test-Path $MinGWBin) {
    $env:PATH = "$MinGWBin;$env:PATH"
} else {
    Write-Warning "MinGW bin not found at $MinGWBin - relying on PATH as-is."
}

if ($Clean -and (Test-Path $buildDir)) {
    Remove-Item $buildDir -Recurse -Force
}

cmake -S $root -B $buildDir -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=$Config
if ($LASTEXITCODE -ne 0) { throw "CMake configure failed" }

cmake --build $buildDir
if ($LASTEXITCODE -ne 0) { throw "Build failed" }

$exe = Join-Path $buildDir 'bin\first_raylib_game.exe'
Write-Host "Built $exe" -ForegroundColor Green

if ($Run) { & $exe }
