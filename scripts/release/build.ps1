# This script is used for building fima

Write-Host ""

Set-Location -Path (Split-Path -Path $MyInvocation.MyCommand.Definition -Parent)

if (-not (Test-Path -Path ../../build -PathType Container)) {
    New-Item -ItemType Directory -Path ../../build | Out-Null
}

Set-Location -Path ../../build

try {
    conan install .. --output-folder=. --build=missing -s compiler.cppstd=23 -s build_type=Release
} catch {
    Write-Error "Conan setup failed"
    exit 1
}

try {
    cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
} catch {
    Write-Error "CMake configuration failed"
    exit 1
}

try {
    $jobs = [Environment]::ProcessorCount
    cmake --build . -- -j $jobs
} catch {
    Write-Error "Build failed"
    exit 1
}

Write-Host ""
