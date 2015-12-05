#!/bin/zsh

git add ../sources/*
git add ../scripts/*
git add ../misc/*
git add ../resources/*
git commit -m $1
git push
