# This script is used for building fima with tests

Write-Host ""

Set-Location -Path (Split-Path -Path $MyInvocation.MyCommand.Definition -Parent)

Get-Content utils/ascii-art.txt | ForEach-Object { Write-Host $_ -ForegroundColor Green }

if (-not (Test-Path -Path ../build -PathType Container)) {
    New-Item -ItemType Directory -Path ../build | Out-Null
}

Set-Location -Path ../build

try {
    conan install .. --output-folder=. --build=missing -s compiler.cppstd=23 --settings=build_type=Debug
} catch {
    Write-Error "Conan setup failed"
    exit 1
}

try {
    cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug -DFIMA_BUILD_TESTS=ON -DFIMA_CODE_COVERAGE=ON
} catch {
    Write-Error "CMake configuration failed"
    exit 1
}

try {
    $jobs = [Environment]::ProcessorCount
    cmake --build . --config Debug -- -j $jobs
} catch {
    Write-Error "Build failed"
    exit 1
}

Write-Host ""

$jobs = [Environment]::ProcessorCount
ctest -j $jobs --output-on-failure -T Test -T Coverage
