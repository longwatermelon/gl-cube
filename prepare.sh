#!/bin/sh
rm -rf .git
rm -rf deps/cglm deps/glfw
rm .gitmodules

git init
git add .
rm README.md
git commit -m "first commit"

git submodule add https://github.com/glfw/glfw deps/glfw
git submodule add https://github.com/recp/cglm deps/cglm
git add .
git commit -m "deps"

