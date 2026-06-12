# This script is used for building fima (windows)

$ErrorActionPreference = "Stop"

Write-Host ""

Set-Location -Path (Split-Path -Path $MyInvocation.MyCommand.Definition -Parent)

Get-Content utils/ascii-art.txt | ForEach-Object { Write-Host $_ -ForegroundColor Green }

if (-not (Test-Path -Path "../build" -PathType Container)) {
    New-Item -ItemType Directory -Path "../build" | Out-Null
}

Set-Location -Path "../build"

try {
    conan install .. --output-folder=. --build=missing -s compiler.cppstd=23
} catch {
    Write-Host "Conan setup failed"
    exit 1
}

if ($args[0] -eq "tests") {
    try {
        cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -DFIMA_BUILD_TESTS=True
    } catch {
        Write-Host "CMake configuration failed"
        exit 1
    }
} else {
    try {
        cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
    } catch {
        Write-Host "CMake configuration failed"
        exit 1
    }
}

try {
    cmake --build .
} catch {
    Write-Host "Build failed"
    exit 1
}

Set-Location -Path ".."

Write-Host ""
