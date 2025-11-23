#!/bin/bash

input_file="compile_commands.json"
output_file="compile_commands_clean.json"

jq 'map(
  .command |=
    (split(" ") |
     map(select(
       (. != "-fdeps-format=p1689r5") and
       (. != "-fmodules-ts") and
       (test("^-fmodule-mapper=") | not)
     )) | join(" "))
)' "$input_file" > "$output_file"
